#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include "data.h"
#include "session.h"

#define EXIT "exit"

socket_t client_socket;

int getCurrentMusic(socket_t *client_socket );
void lancerMusique(char *file_name, float tempsEcoule);
static void signalHandler(int sig);

int main(int argc, char *argv[]) {
    buffer_t buffer;
    int bytesRead;
    int i=0;
    char *server_ip = argv[1];
    short server_port = atoi(argv[2]);
    char request[MAX_BUFF];
    char new_file_name[MAX_BUFF];



    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);        
    }

    signal(SIGINT, signalHandler);
    

    // Initialiser la connexion
    client_socket.type = SOCK_CLIENT;
    client_socket.mode = SOCK_STREAM;
    ouvrirSocket(&client_socket, SOCK_STREAM, server_ip, server_port);


    // la musique courante
    getCurrentMusic(&client_socket);

    // Fermer les ressources
    fermerSocket(&client_socket);
    return 0;
}


int getCurrentMusic(socket_t *client_socket ){
    buffer_t buffer;
    int bytesRead;
    int i=0;
    char request[MAX_BUFF];
    char new_file_name[MAX_BUFF];
    float tempsEcoule=0;

    // Envoyer la requête GET avec le nom du fichier
    strcpy(request, "GET current");
    envoyer(client_socket, request, NULL);

    recevoir(client_socket, buffer, NULL);
    if (strcmp(buffer, "OK") != 0) {
        exit(EXIT_FAILURE);
    }

    // recevoir le nom du fichier
    recevoir(client_socket, buffer, NULL);

    strcpy(new_file_name, "current_");
    strcat(new_file_name, buffer);

    // supprimer les anciens fichiers
    system("rm -f current_*.mp3");   

    // Recevoir et sauvegarder le fichier MP3
    remove(new_file_name);
    do {
        // ecrire dans le fichier octet par octet
        FILE *file = fopen(new_file_name, "ab");
        CHECK_FILE(file, "fopen");
        // recevoir le contenu du fichier mp3
        recevoir(client_socket, buffer, NULL);
        if (strcmp(buffer, EXIT) != 0) {
            // ecrire dans le fichier octet par octet
            fwrite(buffer, 1, MAX_BUFF, file);
        }
        fclose(file);
    } while (strcmp(buffer, EXIT) != 0);

    // recevoir tempsEcoule du serveur
    recevoir(client_socket, buffer, NULL);
    tempsEcoule = atof(buffer);

    // Envoyer un message de confirmation
    envoyer(client_socket, "OK", NULL);

    printf("Fichier reçu et enregistrer sous : %s\n", new_file_name);

    // lancer la musique
    lancerMusique(new_file_name, tempsEcoule);
    return 0;
}


void lancerMusique(char *file_name, float tempsEcoule) {

    printf("\nLancement de la musique %s\n", file_name);
    printf("Temps ecoule: %f secondes\n", tempsEcoule);

    // lancer la musique avec le temps ecoule
    char command[MAX_BUFF];
    // float to int 
    //int i = (int) ( tempsEcoule * 128)/8 ; 
    int i = (int) (3 * tempsEcoule);
    sprintf(command, "mpg123 -k %d %s ",i  , file_name);
    printf("Commande: %s\n", command);
    system(command);
    
}


static void signalHandler(int sig) {
    switch (sig) {
        case SIGINT:
            // Fermer le fichier et terminer le programme lorsque CTRL+C est détecté
            printf("\nFermeture du client...\n");
            if (client_socket.sock != -1)
                fermerSocket(&client_socket);
            
            exit(EXIT_SUCCESS);
            break;

        default:
            printf("Signal inconnu\n");
            break;
    }
}