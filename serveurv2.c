#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "data.h"

#define EXIT "exit"

socket_t client_socket;

int getCurrentMusic(socket_t *client_socket);
void lancerMusique(char *file_name, float tempsEcoule);
static void signalHandler(int sig);

int main(int argc, char *argv[]) {
    buffer_t buffer;
    char *server_ip;
    short server_port;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <ip> <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    server_ip = argv[1];
    server_port = atoi(argv[2]);

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

int getCurrentMusic(socket_t *client_socket) {
    buffer_t buffer;
    char request[MAX_BUFF];
    char new_file_name[MAX_BUFF];
    float tempsEcoule = 0;

    // Envoyer la requête GET avec le nom du fichier
    strcpy(request, "GET current");
    envoyer(client_socket, request, NULL);

    // Recevoir la réponse
    recevoir(client_socket, buffer, NULL);
    if (strcmp(buffer, "OK") != 0) {
        exit(EXIT_FAILURE);
    }

    // Recevoir le nom du fichier
    recevoir(client_socket, buffer, NULL);
    strcpy(new_file_name, "current_");
    strcat(new_file_name, buffer);

    // Recevoir le contenu du fichier
    FILE *file = fopen(new_file_name, "wb");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    do {
        recevoir(client_socket, buffer, NULL);
        if (strcmp(buffer, EXIT) != 0) {
            fwrite(buffer, 1, MAX_BUFF, file);
        }
    } while (strcmp(buffer, EXIT) != 0);

    // Recevoir tempsEcoule du serveur
    recevoir(client_socket, buffer, NULL);
    tempsEcoule = atof(buffer);

    // Envoyer un message de confirmation
    envoyer(client_socket, "OK", NULL);

    printf("Fichier reçu et enregistré sous : %s\n", new_file_name);

    // Lancer la musique
    lancerMusique(new_file_name, tempsEcoule);
    fclose(file);
    return 0;
}

void lancerMusique(char *file_name, float tempsEcoule) {
    printf("\nLancement de la musique %s\n", file_name);
    printf("Temps écoulé: %f secondes\n", tempsEcoule);

    // Lancer la musique avec le temps écoulé
    char command[MAX_BUFF];
    int i = (int)(3 * tempsEcoule);
    sprintf(command, "mpg123 -k %d %s ", i, file_name);
    printf("Commande: %s\n", command);
    system(command);
}

static void signalHandler(int sig) {
    switch (sig) {
        case SIGINT:
            // Fermer le fichier et terminer le programme lorsque CTRL+C est détecté
            printf("\nFermeture du client...\n");
            if (client_socket.sock != -1) {
                fermerSocket(&client_socket);
            }
            exit(EXIT_SUCCESS);
            break;

        default:
            printf("Signal inconnu\n");
            break;
    }
}
