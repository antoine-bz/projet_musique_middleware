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
socket_t client_socket;
void lancerMusique(char *file_name, int tempsEcoule) ;
static void signalHandler(int sig);
void client (char *addrIPsrv, short port);