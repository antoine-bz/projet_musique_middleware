#include "protoClient.h"
socket_t sockDial;  
int *mute;
int shm_id, shm_size;


void client(char *addrIPsrv, short port) {
    char reponse[MAX_BUFF];
    char musicName[MAX_BUFF];
    MusicMessage buffer;
    int choix, tailleTableau;

    pid_t pid;

    signal(SIGINT, signalHandler);

    // on cree la memoire 
    CHECK(shm_id = shm_open("maZoneClient", O_CREAT | O_RDWR, S_IRWXU), "shm_open error");

    // Calculate file size
    shm_size = sizeof(int);

    // Set the size of the shared memory object
    CHECK(ftruncate(shm_id, shm_size) == 0, "ftruncate error");

    // Map the shared memory
    // on initialise les variables
    mute = (int *) mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_id, 0);
    CHECK_MAP(mute, "mmap error");

    // on creer un processus fils qui verifie que le client appuie sur m pour mute ou unmute

    pid = fork();
    CHECK(pid, "fork error");
    if (pid == 0) {
        while (1) {
            char c = getchar();
            if (c == 'm') {
                *mute = TRUE;
            } else if (c == 'u') {
                *mute = FALSE;
            }
        }
    }
    


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
        tailleTableau = sizeof(buffer.playlist) / sizeof(buffer.playlist[0]);
        //strlen(buffer.playlist) a voir si ca marche
        for (int i = 0; i < tailleTableau; i++)
        {
            if (strlen(buffer.playlist[i]) > 3)
                printf("%d - %s\n", i, buffer.playlist[i]);
        }
        printf("\nChoisir une musique:\n");
        scanf("%d", &choix);
        strcpy( buffer.current_music,buffer.playlist[choix]);
        buffer.type = SEND_MUSIC_CHOICE;
        buffer.current_time = 0;
        
        envoyer(&sockDial, &buffer, (pFct) serializeMusicMessage);
        recevoir(&sockDial, &reponse, NULL);

        if (strcmp(reponse, "OK") != 0) {
            printf("Erreur de reception du choix\n");
            exit(EXIT_FAILURE);
        }
        
        buffer.type = SEND_MUSIC_REQUEST ;
        buffer.current_time = 0;
        buffer.current_music[0] = '\0';
        envoyer(&sockDial, &buffer, (pFct) serializeMusicMessage);
        recevoir(&sockDial, &buffer, (pFct) deserializeMusicMessage);
    }

    if(buffer.type == MUSIC_RETURN){
        
        recevoirMusique(&sockDial, buffer.current_music);
    }
    else{
        printf("Erreur de reception de la musique\n");
        exit(EXIT_FAILURE);
    }

   
    //while (1)
    //{

        strcpy(musicName, "current_");
        strcat(musicName, buffer.current_music);
        buffer.type = SEND_CURRENT_TIME_REQ ;
        envoyer(&sockDial, &buffer, (pFct) serializeMusicMessage);
        recevoir(&sockDial, &buffer, (pFct) deserializeMusicMessage);




        lancerMusique(musicName, buffer.current_time);

        /*sleep(2);
        buffer.type = SEND_MUSIC_REQUEST ;
        envoyer(&sockDial, &buffer, (pFct) serializeMusicMessage);
        recevoir(&sockDial, &buffer, (pFct) deserializeMusicMessage);
    }*/
    // Fermeture de la connexion
    fermerSocket(&sockDial);
}




void recevoirMusique(socket_t *client_socket, char * nomMusique){
    char buffer[MAX_BUFF];
    char new_file_name [MAX_BUFF];
    

    printf("Reception de la musique\n");
    envoyer(client_socket, "OK", NULL);

    strcpy(new_file_name, "current_");
    strcat(new_file_name, nomMusique);

    // supprimer les anciens fichiers
    system("rm -f current_*.mp3");   

    // Recevoir et sauvegarder le fichier MP3
    remove(new_file_name);
    do {
        // recevoir le contenu du fichier mp3
        recevoir(client_socket, buffer, NULL);
        // ecrire dans le fichier octet par octet
        FILE *file = fopen(new_file_name, "ab");
        CHECK_FILE(file, "fopen");
        if (strcmp(buffer, EXIT) != 0) {
            // ecrire dans le fichier octet par octet
            fwrite(buffer, 1, MAX_BUFF, file);
        }
        fclose(file);
    } while (strcmp(buffer, EXIT) != 0);
}

static void signalHandler(int sig) {
    switch (sig) {
        case SIGINT:
            printf("Signal SIGINT reçu\n");
            // si l'utilisateur appuie sur ctrl+c, on lui demande s'il veut quitter
            printf("Voulez-vous quitter ? (y/n)\n");
            char c = getchar();
            if (c == 'y') {
                fermerSocket(&sockDial);
                CHECK(munmap(mute, shm_size) == 0, "munmap error");
                CHECK(close(shm_id) == 0, "close error"); 
                exit(EXIT_SUCCESS);
            }

            break;

        default:
            printf("Signal inconnu\n");
            break;
    }
}

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

    // Démarrer la lecture à partir du temps écoulé
    long startingFrame = (long)((tempsEcoule / 1000.0) * rate);
    mpg123_seek_frame(mh, startingFrame, SEEK_SET);

    printf("Lecture du fichier MP3\n");
    
    *mute = 0; // Variable pour le statut de sourdine

    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
        
        if (!(*mute)) { // Si la sourdine n'est pas activée, jouer le son
            ao_play(device, buffer, done);
        }
    }

    // Nettoyer et fermer
    free(buffer);
    ao_close(device);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();
}