#include <mpg123.h>
#include <ao/ao.h>
#include <stdio.h>
#include <stdlib.h>

#define BITS 8

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <mp3 file>\n", argv[0]);
        return 1;
    }

    printf("Début du programme\n");

    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    // Initialiser mpg123
    printf("Initialisation de mpg123\n");
    mpg123_init();
    mh = mpg123_new(NULL, &err);

    // Ouvrir le fichier MP3
    printf("Ouverture du fichier MP3 : %s\n", argv[1]);
    mpg123_open(mh, argv[1]);

    // Configurer la sortie audio avec la bibliothèque 'ao'
    int driver;
    ao_device *device;
    ao_sample_format format;
    long rate;  // Modifier le type de la variable
    int channels, encoding;

    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_getformat(mh, &rate, &channels, &encoding);  // Ajuster les types ici
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    device = ao_open_live(driver, &format, NULL);

    // Allouer le tampon de lecture
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*)malloc(buffer_size * sizeof(unsigned char));

    // Lire et jouer le fichier MP3
    printf("Lecture du fichier MP3\n");
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {
        ao_play(device, buffer, done);
    }

    // Nettoyer et fermer
    printf("Nettoyage et fermeture\n");
    free(buffer);
    ao_close(device);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

    printf("Fin du programme\n");

    return 0;
}

