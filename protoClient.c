#include "protoClient.h"


int main() {
    char addrIPsrv[] = "127.0.0.1";  // Adresse IP du serveur
    short port = 5000;  // Port du serveur
    client(addrIPsrv, port);
    return 0;
}

/*
int getCurrentSong(socket_t *client_socket) {
    buffer_t buffer;
    char request[MAX_BUFF];
    char new_file_name[MAX_BUFF];
    float tempsEcoule = 0;

    // Envoyer la requête GET avec le nom du fichier
    strcpy(request, "GET_CURRENT_SONG");
    envoyer(client_socket, request, NULL);

    // Recevoir la réponse du serveur
    recevoir(client_socket, buffer, NULL);
    if (strcmp(buffer, "OK") != 0) {
        fprintf(stderr, "Erreur lors de la demande de la musique en cours.\n");
        return -1; // Retourner une valeur d'erreur
    }

    // Recevoir le nom du fichier
    recevoir(client_socket, buffer, NULL);
    strcpy(new_file_name, "current_");
    strcat(new_file_name, buffer);

    // Supprimer les anciens fichiers s'ils existent
    if (access(new_file_name, F_OK) != -1) {
        remove(new_file_name);
    }

    // Recevoir et sauvegarder le fichier MP3
    FILE *file = fopen(new_file_name, "ab");
    if (file == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier pour écriture.\n");
        return -1; // Retourner une valeur d'erreur
    }

    do {
        // Recevoir le contenu du fichier MP3
        recevoir(client_socket, buffer, NULL);
        if (strcmp(buffer, EXIT) != 0) {
            // Écrire dans le fichier octet par octet
            fwrite(buffer, 1, MAX_BUFF, file);
        }
    } while (strcmp(buffer, EXIT) != 0);

    // Lancer la musique
    lancerMusique(new_file_name, client_socket);

    // Envoyer un message de confirmation
    envoyer(client_socket, "OK", NULL);

    printf("Fichier reçu et enregistré sous : %s\n", new_file_name);

    return 0; // Succès
}

// Fonction pour demander la playlist au serveur
void getPlaylist(socket_t *client_socket) {

    printf("Demande de la playlist...\n");
    // Envoyer la requête au serveur 
    envoyer(client_socket, "GET_PLAYLIST", NULL);

    // Recevoir et afficher la playlist
    buffer_t buffer;
    recevoir(client_socket, buffer, NULL);
    printf("Playlist reçue : %s\n", buffer);
}

// Fonction pour demander le temps courant de la musique en cours
void getCurrentTime(socket_t *client_socket) {
    printf("Demande du temps courant de la musique en cours...\n");
    // Envoyer la requête au serveur (par exemple "GET_CURRENT_TIME")
    envoyer(client_socket, "GET_CURRENT_TIME", NULL);

    // Recevoir et afficher la réponse du serveur
    buffer_t buffer;
    recevoir(client_socket, buffer, NULL);
    printf("Temps courant : %s secondes\n", buffer);
}

// Fonction pour lancer la musique avec le temps écoulé
void lancerMusique(char *file_name, socket_t *client_socket) {
    printf("\nLancement de la musique %s\n", file_name);

    // Obtenir le temps écoulé depuis le serveur
    getCurrentTime(client_socket);

    // Construire la commande pour lancer la musique
    char command[MAX_BUFF];
    sprintf(command, "mpg123 %s", file_name);

    // Lancer la musique
    system(command);
}

//types: MusicMessage.types: PLAYLIST_RETURN ou MUSIC_RETURN
//vérifier après la récéption de musique ou playlist
//renvoyer msg avec le choix de la musique

void verifMusicOrPlayList(FILE* file, const char* type) {
    char buffer[MAX_BUFF];

    // Initialise le buffer avec des caractères nuls
    memset(buffer, '\0', sizeof(buffer));

    // Lit une ligne (jusqu'à MAX_BUFF caractères) du fichier et stocke dans le buffer
    fgets(buffer, MAX_BUFF, file);

    // Vérifie le type demandé (music ou playlist) et compare avec le contenu du buffer
    if (strcmp(type, PLAYLIST_RETURN) == 0 && strncmp(buffer, "[playlist]", 11) != 0) {
        // Affiche une erreur et termine le programme si le type attendu ne correspond pas
        perror("Le fichier n'est pas une playlist.\n");
        exit(EXIT_FAILURE);
    } else if (strcmp(type, MUSIC_RETURN) == 0 && strncmp(buffer, "[music]", 7) != 0) {
        // Affiche une erreur et termine le programme si le type attendu ne correspond pas
        perror("Le fichier n'est pas une musique.\n");
        exit(EXIT_FAILURE);
    }

}*/

//fct pour lancer une musique
void lancerMusique(char *file_name, int tempsEcoule) {
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    // Initialiser mpg123
    mpg123_init();
    mh = mpg123_new(NULL, &err);

    // Ouvrir le fichier MP3
    mpg123_open(mh, file_name);

    // Configurer la sortie audio avec la bibliothèque 'ao'
    int driver;
    ao_device *device;
    ao_sample_format format;
    long rate;
    int channels, encoding;

    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_getformat(mh, &rate, &channels, &encoding);
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    device = ao_open_live(driver, &format, NULL);

    // Allouer le tampon de lecture
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*)malloc(buffer_size * sizeof(unsigned char));

    // Lire et jouer le fichier MP3 à partir du temps écoulé
    long startingFrame = (long)((tempsEcoule / 1000.0) * rate);
    mpg123_seek_frame(mh, startingFrame, SEEK_SET);

    printf("Lecture du fichier MP3\n");
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
        ao_play(device, buffer, done);
    }

    // Nettoyer et fermer
    free(buffer);
    ao_close(device);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}


void client(char *addrIPsrv, short port) {
    socket_t sockDial;
    char reponse[MAX_BUFF];
    MusicMessage buffer;

    // Demande d’une connexion au service
    sockDial.mode = SOCK_STREAM;
    sockDial.sock = connecter(addrIPsrv, port);
    sockDial.type = SOCK_CLIENT;
    ouvrirSocket(&sockDial, SOCK_STREAM, addrIPsrv, port);

    buffer.type = SEND_MUSIC_REQUEST ;

    envoyer(&sockDial, &buffer, (pFct) serializeMusicMessage);
    recevoir(&sockDial, &buffer, (pFct) deserializeMusicMessage);
    // Gestion de la musique courante
    if (buffer.type == PLAYLIST_RETURN) {
        printf("\nPlaylist:\n");
    int tailleTableau = sizeof(buffer.playlist) / sizeof(buffer.playlist[0]);
        //strlen(buffer.playlist) a voir si ca marche
        for (int i = 0; i < tailleTableau; i++)
        {
            printf("%ls - %s\n", &i, buffer.playlist[i]);
        }
        printf("\nChoisir une musique:\n");
        int choix;
        scanf("%d", &choix);
        strcpy(buffer.playlist[choix], buffer.current_music);
        buffer.type = SEND_MUSIC_CHOICE;
        envoyer(&sockDial, &buffer, (pFct) serializeMusicMessage);

        buffer.type = SEND_MUSIC_REQUEST ;
        envoyer(&sockDial, &buffer, (pFct) serializeMusicMessage);
        recevoir(&sockDial, &buffer, (pFct) deserializeMusicMessage);
    }

   
    while (1)
    {

        // SEND_CURRENT_TIME_REQ

        buffer.type = SEND_CURRENT_TIME_REQ ;
        envoyer(&sockDial, &buffer, (pFct) serializeMusicMessage);
        recevoir(&sockDial, &buffer, (pFct) deserializeMusicMessage);

        lancerMusique(buffer.current_music, buffer.current_time);

        sleep(2);
        buffer.type = SEND_MUSIC_REQUEST ;
        envoyer(&sockDial, &buffer, (pFct) serializeMusicMessage);
        recevoir(&sockDial, &buffer, (pFct) deserializeMusicMessage);
    }


    // Fermeture de la connexion
    fermerSocket(&sockDial);
}


