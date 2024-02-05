
#include "reqRep.h"

void deserializeMusicMessage(buffer_t buff, generic quoi)
{
    MusicMessage *msg = (MusicMessage *)quoi;
    ////////////////DEFINI TYPE/////////////////////
    char type[MAX_BUFF];
    strcpy(type,strtok(buff, "|"));

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
        char music[MAX_BUFF]; // Stocker temporairement chaque nom de musique
        int index = 0;
        char *ptr = token;
        while (*ptr != '\0' && index < MAX_BUFF) {
            int i = 0;
            // Copier chaque caractère jusqu'à '/' ou la fin de la chaîne
            while (*ptr != '/' && *ptr != '\0' && i < MAX_BUFF - 1) {
                music[i] = *ptr;
                ptr++;
                i++;
            }
            music[i] = '\0'; // Assurer une terminaison nulle
            strncpy(msg->playlist[index], music, sizeof(msg->playlist[index]) - 1);
            msg->playlist[index][sizeof(msg->playlist[index]) - 1] = '\0'; // Assurer une terminaison nulle
            index++;
            if (*ptr == '/') {
                ptr++; // Passer au prochain nom de musique s'il y en a un
            }
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

void serializeMusicMessage(generic quoi, buffer_t buff){

    MusicMessage *msg = (MusicMessage *)quoi;
        char time_str[MAX_BUFF];
    switch (msg->type)
    {
    case 1:
        strcpy(buff, "PLAYLIST_RETURN|");
        for (int i = 0; i < MAX_BUFF; i++) {
            strcat(buff, msg->playlist[i]);
            strcat(buff, "/");
        }
        break;
    case 2:
        strcpy(buff, "MUSIC_RETURN|");
        strcat(buff, msg->current_music);
        break;
    case 3:
        strcpy(buff, "CURRENT_TIME_RETURN|");
        sprintf(time_str, "%d", msg->current_time);
        strcat(buff, time_str);
        break;
    case 4:
        strcpy(buff, "SEND_MUSIC_CHOICE|");
        strcat(buff, msg->current_music);
        break;
    case 5:
        strcpy(buff, "SEND_MUSIC_REQUEST|");
        strcat(buff, msg->current_music);
        break;
    case 6:
        strcpy(buff, "SEND_CURRENT_TIME_REQ|");
        sprintf(time_str, "%d", msg->current_time);
        strcat(buff, time_str);
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