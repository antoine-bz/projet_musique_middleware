// Function to send the catalog to the client
void sendCatalog(socket_t *client_socket) {
    FILE *catalogFile = fopen("playlist.txt", "r");
    buffer_t buffer;

    CHECK_FILE(catalogFile, "Error opening catalog file");

    // Read the contents of the catalog file and send them to the client
    while (fgets(buffer, MAX_BUFF, catalogFile) != NULL) {
        envoyer(client_socket, buffer, NULL);
    }

    fclose(catalogFile);

    // Send an exit signal to indicate the end of the catalog
    envoyer(client_socket, EXIT, NULL);
}

// Function to request the CurrentSong from the client to server
void requestCurrentSong(socket_t *client_socket) {
    buffer_t request = "G";
    buffer_t response;

    // Send the request to the server
    envoyer(client_socket, request, NULL);
}

// Function to send the CurrentSong from the server to client
void sendCurrentSong(socket_t *client_socket) {
    buffer_t request = "G";
    buffer_t response;

    // Send the request to the client
    envoyer(client_socket, request, NULL);
}

// Function to request the CurrentSong from the radio
void requestCurrentSongFromRadio() {
    buffer_t request = "R";
    buffer_t response;

    // Send the request to the radio
    envoyer(&radio_socket, request, NULL);

    // Receive the response from the radio
    recevoir(&radio_socket, response, NULL);

    // Print or use the response as needed
    printf("CurrentSong from radio: %s\n", response);
}

// Function to request the CurrentTime from the radio
void requestCurrentTimeFromRadio() {
    buffer_t request = "T";
    buffer_t response;

    // Send the request to the radio
    envoyer(&radio_socket, request, NULL);

    // Receive the response from the radio
    recevoir(&radio_socket, response, NULL);

    // Print or use the response as needed
    printf("CurrentTime from radio: %s\n", response);
}