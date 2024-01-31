
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
        char *token = strtok(NULL, "|"); // Récupérer la partie de la chaîne après le délimiteur '|'
        if (token != NULL) {
            char *music = strtok(token, "/"); // Extraire le premier nom de musique
            int index = 0;
            while (music != NULL && index < MAX_BUFF) {
                strncpy(msg->playlist[index], music, sizeof(msg->playlist[index]) - 1);
                msg->playlist[index][sizeof(msg->playlist[index]) - 1] = '\0'; // Assurer une terminaison nulle
                music = strtok(NULL, "/"); // Extraire le nom de musique suivant
                index++;
            }
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

/*

int main( int argc, char *argv[])
{
    buffer_t buffer;
    MusicMessage msg;
    switch (atoi(argv[1]))
    {
    case 1 :
        strcpy(buffer, "PLAYLIST_RETURN|playlist1.mp3/playlist2.mp3/playlist3.mp3");
    serializeMusicMessage(buffer, &msg);
    printf("Type: %d\n", msg.type);
    printf("Playlist: %s\n", msg.playlist[0]);
    printf("Playlist: %s\n", msg.playlist[1]);
    printf("Playlist: %s\n", msg.playlist[2]);
    printf("Current music: %s\n", msg.current_music);
    printf("Current time: %d\n", msg.current_time);
        break;
    case 2 :
        msg.type = 2;
        strcpy(msg.current_music, "music1.mp3");
        deserializeMusicMessage(buffer, &msg);
        printf("Type: %d\n", msg.type);
        printf("Playlist: %s\n", msg.playlist[0]);
        printf("Playlist: %s\n", msg.playlist[1]);
        printf("Playlist: %s\n", msg.playlist[2]);
        printf("Current music: %s\n", msg.current_music);
        printf("Current time: %d\n", msg.current_time);
        break;
    case 3 :
        msg.type = 3;
        msg.current_time = 10;
        deserializeMusicMessage(buffer, &msg);
        printf("Type: %d\n", msg.type);
        printf("Playlist: %s\n", msg.playlist[0]);
        printf("Playlist: %s\n", msg.playlist[1]);
        printf("Playlist: %s\n", msg.playlist[2]);
        printf("Current music: %s\n", msg.current_music);
        printf("Current time: %d\n", msg.current_time);
        break;

    default:
        break;
    }


    return 0;
}
*/