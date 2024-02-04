#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdbool.h>

#include "reqRep.h"



FILE *currentMusicFile;
int shared_variable=0;

// on cree la memoire partagee
int *elapsedTime;
char *currentMusic;
bool *isPlaying;
int shm_id, shm_size;

void server (char *addrIPsrv, short server_port);

void handle_client(socket_t *client_socket);
void myRadio();
static void signalHandler(int sig);

int sendPlaylist(socket_t *client_socket, buffer_t request);
int sendCurrentMusic(socket_t *client_socket, buffer_t request);