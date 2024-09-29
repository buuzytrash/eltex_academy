CC = gcc 
CFLAGS = -Wall -Wextra 

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BINS = pipe_fork serv cli shabash_V2 

all: $(BINS)

$(BINS): %: $(OBJ_DIR)/%.o
	$(CC) $(CFLAGS) -o $@ $< -pthread

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ -pthread

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) 

clean:
	rm -rf $(BINS) $(OBJ_DIR) my_fifo

.PHONY: all clean