
#include "data.h"


#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}
#define CHECK_FILE(sts,msg) if ((sts) == NULL) {perror(msg);exit(-1);}
#define CHECK_SHM(sts,msg) if ((sts) == (void *) -1) {perror(msg);exit(-1);}


#define MAX_BUFF 1024
#define EXIT "exit"

typedef char buffer_t[MAX_BUFF];

// Définir des constantes pour les types de messages
#define PLAYLIST_RETURN       1
#define SEND_MUSIC_CHOICE     2
#define SEND_MUSIC_REQUEST    3
#define MUSIC_RETURN          4
#define SEND_CURRENT_TIME_REQ 5
#define CURRENT_TIME_RETURN   6

typedef struct MusicMessage {
    int type; // type de message
    char current_music[MAX_BUFF]; // nom de la chanson courante
    int current_time; // temps courant de la chanson en millisec
    char playlist[MAX_BUFF][MAX_BUFF]; // nom de la chanson courante
} MusicMessage;


void serializeMusicMessage(generic quoi, buffer_t buff);

void deserializeMusicMessage(buffer_t buff, generic quoi);
