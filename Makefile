CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = -lm
SRC_DIR = .
OBJ_DIR = .
BIN_DIR = .

# Liste des fichiers source
CLIENT_SRC = $(SRC_DIR)/clientv2.c
SERVER_SRC = $(SRC_DIR)/serveurv2.c
UTILS_SRC = $(SRC_DIR)/libUtils.c
SESSION_SRC = $(SRC_DIR)/session.c
DATA_SRC = $(SRC_DIR)/data.c

# Liste des fichiers objet
CLIENT_OBJ = $(OBJ_DIR)/clientv2.o
SERVER_OBJ = $(OBJ_DIR)/serveurv2.o
UTILS_OBJ = $(OBJ_DIR)/libUtils.o
SESSION_OBJ = $(OBJ_DIR)/session.o
DATA_OBJ = $(OBJ_DIR)/data.o

# Noms des exécutables
CLIENT_EXE = $(BIN_DIR)/client
SERVER_EXE = $(BIN_DIR)/server

all: client server

client: $(CLIENT_OBJ) $(UTILS_OBJ) $(SESSION_OBJ) $(DATA_OBJ)
	$(CC) $(CFLAGS) -o $(CLIENT_EXE) $(CLIENT_OBJ) $(UTILS_OBJ) $(SESSION_OBJ) $(DATA_OBJ) $(LDFLAGS)

server: $(SERVER_OBJ) $(UTILS_OBJ) $(SESSION_OBJ) $(DATA_OBJ)
	$(CC) $(CFLAGS) -o $(SERVER_EXE) $(SERVER_OBJ) $(UTILS_OBJ) $(SESSION_OBJ) $(DATA_OBJ) $(LDFLAGS)

$(CLIENT_OBJ): $(CLIENT_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SERVER_OBJ): $(SERVER_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

$(UTILS_OBJ): $(UTILS_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

$(SESSION_OBJ): $(SESSION_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

$(DATA_OBJ): $(DATA_SRC)
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(CLIENT_EXE) $(SERVER_EXE)
