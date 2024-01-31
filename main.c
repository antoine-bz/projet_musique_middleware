#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "proto.h"
#include "reqRep.h"
//#include "session.h"


#define PORT_SVC 5000
#define INADDR_SVC "127.0.0.1"


int main(int argc, char *argv[]) {
    // gcc useLiNet.c -DCLIENT -o client
    #ifdef CLIENT
        
        // Vérification des arguments
        if(argc != 3) {
                printf("Usage : %s <IP> <PORT>\n", argv[0]);
                exit(-1);
        }
        
    #endif
    // gcc useLiNet.c -DSERVEUR -o serveur
    #ifdef SERVEUR
        // Vérification des arguments
        if(argc != 3) {
                printf("Usage : %s <IP> <PORT>\n", argv[0]);
                exit(-1);
        }
        
        
    #endif
}