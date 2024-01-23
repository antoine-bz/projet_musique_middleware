#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include "data.h"
#include "session.h"

#define EXIT "exit"

char currentMusic[MAX_BUFF];
FILE *currentMusicFile;
int shared_variable=0;

// on cree la memoire partagee
float *tempsEcoule;
int shm_id, shm_size;

int sendCurrentMusic(socket_t *client_socket, buffer_t request);
void handle_client(socket_t *client_socket);
void myRadio();
static void signalHandler(int sig);


int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    short server_port;
    socket_t server_socket;
    pid_t pid;


    // on cree la memoire partagee
    CHECK(shm_id = shm_open("maZone", O_CREAT | O_RDWR, S_IRWXU), "shm_open error");

    // etalonnage du fichier
    shm_size = sizeof(int);
    CHECK(ftruncate(shm_id, shm_size), "ftruncate error"); 
    tempsEcoule = (float *) mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    CHECK_MAP(tempsEcoule, "mmap error");
    *tempsEcoule = 0;

    server_port = atoi(argv[1]);
    signal(SIGINT, signalHandler);

    // Initialiser le serveur
    server_socket.type = SOCK_SERVEUR;
    server_socket.mode = SOCK_STREAM;
    ouvrirSocket(&server_socket, SOCK_STREAM, "127.0.0.1", server_port);

    // on lance la radio en parallèle
     pid = fork();
    if (pid == -1) {
        perror("Erreur lors de la création du processus fils");
        exit(EXIT_FAILURE);
    } else
    if (pid == 0) {
        myRadio();
    }


    while (1) {
        // Attendre une connexion entrante
        socket_t client_socket = accepterConnection(&server_socket);

        // Gérer la requête du client
        handle_client(&client_socket);

        // Fermer la connexion avec le client
        fermerSocket(&client_socket);
    }

    // Le code ici ne sera jamais atteint, car le serveur est en boucle infinie
    fermerSocket(&server_socket);

    return 0;
}


void handle_client(socket_t *client_socket) {
    buffer_t request;

    recevoir(client_socket, request, NULL);

    // on fait un switch pour savoir quel commande a ete envoye par le client
    switch (request[0]) {
        case 'G':
            // on appelle la fonction telecharger
            sendCurrentMusic(client_socket, request);
            break;
        default:
            // on envoie un message d'erreur au client
            envoyer(client_socket, "Commande inconnue", NULL);
            break;
    }
}


int sendCurrentMusic(socket_t *client_socket, buffer_t request) {
    buffer_t buffer;
    char *file_name;
    int bytesRead;
    int i=0;

    // on recupere le nom du fichier a telecharger
    FILE *currentMusicFile2 = fopen("currentMusic.txt", "r");
    
    CHECK_FILE(currentMusicFile2, "Error opening file");

    while ((buffer[i] = fgetc(currentMusicFile2)) != EOF) {
        i++;
    }
    buffer[i] = '\0';
    fclose(currentMusicFile2);


    strcpy(currentMusic, buffer);

    // on recupere le nom du fichier a telecharger et on ajoute le dossier dans lequel il se trouve devant
    strcpy(file_name, "playlist/");
    strcat(file_name, currentMusic);
    FILE *file = fopen(file_name, "rb");

    CHECK_FILE(file, "Error opening file");

    envoyer(client_socket, "OK", NULL);
    envoyer(client_socket, currentMusic, NULL);

    printf("Sending %s...\n", currentMusic);

    // Envoyer le contenu du fichier mp3 au client
    while ((bytesRead = fread(buffer, 1, MAX_BUFF, file)) > 0) {
        envoyer(client_socket, buffer, NULL);
    }
    
    envoyer(client_socket, EXIT, NULL);

    // envoyer tempsEcoule au client
    sprintf(buffer, "%f", *tempsEcoule);
    envoyer(client_socket, buffer, NULL);    

    recevoir(client_socket, buffer, NULL);
    if (strcmp(buffer, "OK") != 0) {
        exit(EXIT_FAILURE);
    }


    fclose(file);

    printf("Sent %s to client\n\n", currentMusic);
    return 1;
}

void myRadio(){
    char *line = malloc(MAX_BUFF);
    size_t len = 0;
    ssize_t read;
    char * token = malloc(MAX_BUFF);
    char *file_name = malloc(MAX_BUFF);
    char *minutes = malloc(MAX_BUFF);
    char *seconds = malloc(MAX_BUFF);
    int totalSeconds;
    FILE *file = fopen("playlist.txt", "r");

    CHECK_FILE(file, "Error opening file");
    
    // on lit le fichier ligne par ligne
    while ((read = getline(&line, &len, file)) != -1) {
        *tempsEcoule=0;
        
        // on recupere le nom du fichier et la durée
        token = strtok(line, ";");
        file_name = token;
        token = strtok(NULL, ";");
        
        //on met le nom du fichier dans  currentMusic.txt
        currentMusicFile = fopen("currentMusic.txt", "w");
        if (currentMusicFile == NULL) {
            perror("Error opening file");
            return;
        }
        fprintf(currentMusicFile, "%s", file_name);
        fclose(currentMusicFile);
        
        // on formate la durée pour pouvoir l'utiliser dans sleep
        minutes = strtok(token, ":");
        seconds = strtok(NULL, ":");
        totalSeconds = atoi(minutes)*60 + atoi(seconds);

        sleep(2);
        printf("Playing %s for %d seconds\n\n", file_name, totalSeconds);
        // on attend la durée de la musique et on incremente tempsEcoule de 0.0001s sachant que totalSeconds est en secondes
        for (int i=0; i<totalSeconds*10000; i++) {
            usleep(100);
            *tempsEcoule += 0.0001;
        }
        
    }

    CHECK(munmap(tempsEcoule, shm_size), "munmap error");
    CHECK(close(shared_variable), "close error");
    CHECK(shm_unlink("maZone"), "shm_unlink error");
    fclose(currentMusicFile);
    fclose(file);
    if (line)
        free(line);

    exit(EXIT_SUCCESS);    
}


static void signalHandler(int sig) {
    switch (sig) {
        case SIGINT:
            // Fermer le fichier et terminer le programme lorsque CTRL+C est détecté
            printf("\nFermeture du serveur...\n");
            if (currentMusicFile != NULL) {
                fclose(currentMusicFile);
            }
            CHECK(munmap(tempsEcoule, shm_size), "munmap error");
            CHECK(close(shared_variable), "close error");
            CHECK(shm_unlink("maZone"), "shm_unlink error");
            exit(EXIT_SUCCESS);
            break;

        default:
            printf("Signal inconnu\n");
            break;
    }
}
