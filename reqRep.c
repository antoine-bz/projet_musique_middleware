
#include "reqRep.h"

void serializeMusicMessage(buffer_t buffer, MusicMessage *msg)
{
    ////////////////DEFINI TYPE/////////////////////
    char type[MAX_BUFF];
    strcpy(type,strtok(buffer, "|"));

    if (strcmp(type, "PLAYLIST_RETURN") == 0) {
        msg->type = 1;
    }
    else if (strcmp(type, "MUSIC_RETURN") == 0) {
        msg->type = 2;
    }
    else if (strcmp(type, "CURRENT_TIME_RETURN") == 0) {
        msg->type = 3;
    }
    else if (strcmp(type, "SEND_MUSIC_CHOICE") == 0) {
        msg->type = 4;
    }
    else if (strcmp(type, "SEND_MUSIC_REQUEST") == 0) {
        msg->type = 5;
    }
    else if (strcmp(type, "SEND_CURRENT_TIME_REQ") == 0) {
        msg->type = 6;
    }
    else {
        printf("Erreur de type de message");
    }
    ///////////////////SWITCH TYPE/////////////////////

    switch (msg->type)
    {
    case 1:
        char music[MAX_BUFF];
        strcpy(music,strtok(NULL, "/"));
        while (music != NULL) {
        strcpy(music,strtok(NULL, " / "));
        strcpy(msg->playlist[0], music);
        }
        break;
    case 2:
        strcpy(msg->current_music, strtok(NULL, "|"));
        break;
    case 3:
        msg->current_time = atoi(strtok(NULL, "|"));
        break;
    case 4:
        strcpy(msg->current_music, strtok(NULL, "|"));
        break;
    case 5:
        strcpy(msg->current_music, strtok(NULL, "|"));
        break;
    case 6:
        strcpy(msg->current_music, strtok(NULL, "|"));
        break;
    default:
        break;
    }

}

void deserializeMusicMessage(buffer_t buffer, MusicMessage *msg) {
        char time_str[MAX_BUFF];
    switch (msg->type)
    {
    case 1:
        strcpy(buffer, "PLAYLIST_RETURN|");
        for (int i = 0; i < MAX_BUFF; i++) {
            strcat(buffer, msg->playlist[i]);
            strcat(buffer, "/");
        }
        break;
    case 2:
        strcpy(buffer, "MUSIC_RETURN|");
        strcat(buffer, msg->current_music);
        break;
    case 3:
        strcpy(buffer, "CURRENT_TIME_RETURN|");
        sprintf(time_str, "%d", msg->current_time);
        strcat(buffer, time_str);
        break;
    case 4:
        strcpy(buffer, "SEND_MUSIC_CHOICE|");
        strcat(buffer, msg->current_music);
        break;
    case 5:
        strcpy(buffer, "SEND_MUSIC_REQUEST|");
        strcat(buffer, msg->current_music);
        break;
    case 6:
        strcpy(buffer, "SEND_CURRENT_TIME_REQ|");
        sprintf(time_str, "%d", msg->current_time);
        strcat(buffer, time_str);
        break;
    default:
        break;
    }
}