CC = gcc
INCLUDE_DIR = include
LIB_DIR = lib
OBJ_DIR = obj
CFLAGS = -Wall -Wextra -I$(INCLUDE_DIR)

LIB_SRC = $(wildcard $(LIB_DIR)/*.c)
LIB_OBJ = $(patsubst $(LIB_DIR)/%.c, $(OBJ_DIR)/%.o, $(LIB_SRC))
LIB_NAME = libcalc.a

APP_SRC = main.c
APP_OBJ = $(patsubst %.c, $(OBJ_DIR)/%.o, $(APP_SRC))
BIN = calc

all: $(BIN)

$(BIN) : $(APP_OBJ) $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $(APP_OBJ) -L. -lcalc 

$(LIB_NAME): $(LIB_OBJ)
	ar rcs $@ $^

$(OBJ_DIR)/%.o: $(LIB_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: %.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BIN) $(OBJ_DIR) $(LIB_NAME)

run:
	./$(BIN)

.PHONY: all clean run