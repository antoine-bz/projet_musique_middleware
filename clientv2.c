#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "libUtils.h"

#define EXIT "exit"

socket_t client_socket;

int getCurrentMusic(socket_t *client_socket);
void playMusic(const char *file_name, float tempsEcoule);
static void signalHandler(int sig);

int main(int argc, char *argv[]) {
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

    // Obtenir la musique actuelle
    getCurrentMusic(&client_socket);

    // Fermer la connexion
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
    sendMessage(client_socket, request);

    // Recevoir la confirmation du serveur
    receiveMessage(client_socket, buffer, MAX_BUFF);
    if (strcmp(buffer, "OK") != 0) {
        exit(EXIT_FAILURE);
    }

    // Recevoir le nom du fichier
    receiveMessage(client_socket, buffer, MAX_BUFF);
    strcpy(new_file_name, "current_");
    strcat(new_file_name, buffer);

    // Supprimer les anciens fichiers
    system("rm -f current_*.mp3");

    // Recevoir et enregistrer le fichier MP3
    FILE *file = openFile(new_file_name, "wb");
    while (1) {
        receiveData(client_socket, buffer, MAX_BUFF);
        if (strcmp(buffer, EXIT) == 0) {
            break;
        }
        fwrite(buffer, 1, MAX_BUFF, file);
    }
    closeFile(file);

    // Recevoir le temps écoulé du serveur
    receiveMessage(client_socket, buffer, MAX_BUFF);
    tempsEcoule = atof(buffer);

    // Envoyer un message de confirmation au serveur
    sendMessage(client_socket, "OK");

    printf("Fichier reçu et enregistré sous : %s\n", new_file_name);

    // Lancer la musique
    playMusic(new_file_name, tempsEcoule);

    return 0;
}

void playMusic(const char *file_name, float tempsEcoule) {
    printf("\nLancement de la musique %s\n", file_name);
    printf("Temps écoulé : %f secondes\n", tempsEcoule);

    // Lancer la musique avec le temps écoulé
    int seconds = (int)(tempsEcoule * 3); // Approximation de la durée en secondes
    char command[MAX_BUFF];
    sprintf(command, "mpg123 -k %d %s", seconds, file_name);
    printf("Commande : %s\n", command);
    system(command);
}

static void signalHandler(int sig) {
    switch (sig) {
        case SIGINT:
            // Fermer la connexion et quitter le programme lors de la détection de CTRL+C
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
