#include "reqRep.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <mpg123.h>
#include <ao/ao.h>

#define BITS 8


void client (char *addrIPsrv, short port);
void lancerMusique(char *file_name, int tempsEcoule) ;
void recevoirMusique(socket_t *client_socket);
void client (char *addrIPsrv, short port);