#ifndef LIB_UTILS_H
#define LIB_UTILS_H



#include "data.h"

// Déclarations des fonctions

void sendMessage(socket_t *sock, const char *message);
void receiveMessage(socket_t *sock, char *buffer, size_t buffer_size);
FILE *openFile(const char *filename, const char *mode);
void receiveData(socket_t *sock, char *buffer, size_t buffer_size);
void closeFile(FILE *file);

void sendCatalog(socket_t *client_socket);
void requestCurrentSong(socket_t *client_socket);
void sendCurrentSong(socket_t *client_socket);
void requestCurrentSongFromRadio();
void requestCurrentTimeFromRadio();

#endif /* LIB_UTILS_H */
