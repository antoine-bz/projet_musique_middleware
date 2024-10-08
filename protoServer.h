/**
 * \file protoServer.h
 * \brief Fichier d'entête de protoServer.c
 * \version 0.1
*/

#include "reqRep.h"


// on cree la memoire partagee
/**
 * \var int *elapsedTime
 * \brief Pointeur sur la variable elapsedTime pour savoir combien de temps la musique est en train d'être jouée
 * \var char *currentMusic
 * \brief Pointeur sur la variable currentMusic pour savoir quelle musique est en train d'être jouée
 * \var int *isPlaying
 * \brief Pointeur sur la variable isPlaying pour savoir si le client est en train de jouer une musique
 * \var int *isChoosing
 * \brief Pointeur sur la variable isChoosing pour savoir si le client est en train de choisir une musique
 * \var int shm_id
 * \brief Identifiant de la mémoire partagée
 * \var int shm_size
 * \brief Taille de la mémoire partagée
 * \var void *shm_ptr
 * \brief Pointeur sur la mémoire partagée
 * \brief
 * \details Ces variables sont utilisées pour la communication entre le serveur et son processus fils
*/
extern int *elapsedTime;
extern char *currentMusic;
extern int *isPlaying;
extern int *isChoosing;
extern int shm_id;
extern int shm_size;
extern void *shm_ptr;


/**
 * \fn void server (char *addrIPsrv, short server_port)
 * \brief Fonction qui crée un serveur pour écouter les demandes des clients
 * \param addrIPsrv Adresse IP du serveur
 * \param server_port Port du serveur
 * \return void
 * \details Cette fonction crée un serveur pour écouter les demandes des clients
*/
void server (char *addrIPsrv, short server_port);


/**
 * \fn void handle_client(socket_t *client_socket)
 * \brief Fonction qui gère les demandes des clients
 * \param client_socket Pointeur sur le socket du client
 * \return void
 * \details Cette fonction gère les demandes des clients
*/
void handle_client(socket_t *client_socket);

/**
 * \fn void myRadio()
 * \brief Fonction qui crée un processus fils pour jouer la musique
 * \return void
 * \details Cette fonction crée un processus fils pour jouer la musique et gerer les memoires partagees
*/
void myRadio();

/**
 * \fn void signalHandler(int sig)
 * \brief Fonction qui gère les signaux
 * \param sig Signal
 * \return void
 * \details Cette fonction gère les signaux
*/
static void signalHandler(int sig);

/**
 * \fn void sendPlaylist(socket_t *client_socket)
 * \brief Fonction qui envoie la playlist au client
 * \param client_socket Pointeur sur le socket du client
 * \return void
 * \details Cette fonction envoie la playlist au client
*/
void sendPlaylist(socket_t *client_socket);

/**
 * \fn void sendCurrentMusic(socket_t *client_socket)
 * \brief Fonction qui envoie la musique en cours de lecture au client
 * \param client_socket Pointeur sur le socket du client
 * \return void
 * \details Cette fonction envoie la musique en cours de lecture au client
*/
void sendCurrentMusic(socket_t *client_socket);