#include "protoClient.h"


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


