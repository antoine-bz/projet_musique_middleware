#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>

#include "reqRep.h"

#define TRUE 1
#define FALSE 0


// on cree la memoire partagee
extern int *elapsedTime;
extern char *currentMusic;
extern int *isPlaying;
extern int *isChoosing;
extern int shm_id, shm_size;
extern void *shm_ptr;

void server (char *addrIPsrv, short server_port);

void handle_client(socket_t *client_socket);
void myRadio();
static void signalHandler(int sig);

int sendPlaylist(socket_t *client_socket, buffer_t request);
int sendCurrentMusic(socket_t *client_socket, buffer_t request);