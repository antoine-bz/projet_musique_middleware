
#include "data.h"

#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}
#define CHECK_FILE(sts,msg) if ((sts) == NULL) {perror(msg);exit(-1);}
#define CHECK_SHM(sts,msg) if ((sts) == (void *) -1) {perror(msg);exit(-1);}


#define MAX_BUFF 1024

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

//“PLAYLIST_RETURN|music1.mp3/music2.mp3”
/*
void serializeMusicMessage(buffer_t buffer, MusicMessage *msg)
{
    char *type = strtok(buffer, "|");
    switch(type) {
        case "PLAYLIST_RETURN":
          msg->type = 1;
            break;
        case "SEND_MUSIC_CHOICE":
          msg->type = 2;
            break;
        case "SEND_MUSIC_REQUEST":
            msg->type = 3;
                break;
        case "MUSIC_RETURN":
            msg->type = 4;
                break;
        case "SEND_CURRENT_TIME_REQ":
            msg->type = 5;
                break;
        case "CURRENT_TIME_RETURN":
            msg->type = 6;
                break;
    }
    if msg->type == 1 {
        char *music = strtok(NULL, "/");
        strcpy(msg->current_music, music);

    }
    elseif msg->type == 2 {
        char *music = strtok(NULL, "|");
        strcpy(msg->current_music, music);

    }



}

void deserializeMusicMessage(buffer_t buffer, MusicMessage *msg);

*/