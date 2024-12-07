CC = gcc 
CFLAGS = -Wall -Wextra 
LIBS = -lrt -lncurses

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SERVER_SRC = $(SRC_DIR)/serv.c
CLIENT_SRC = $(SRC_DIR)/cli.c

SERVER_OBJ = $(OBJ_DIR)/serv.o
CLIENT_OBJ = $(OBJ_DIR)/cli.o

SERVER_BIN = $(BIN_DIR)/serv
CLIENT_BIN = $(BIN_DIR)/cli

BINS = $(SERVER_BIN) $(CLIENT_BIN)

all: $(BINS)

$(BINS): $(BIN_DIR)/%: $(OBJ_DIR)/%.o | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) 

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR) 	

.PHONY: all clean