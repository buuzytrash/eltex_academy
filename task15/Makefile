CC = gcc 
CFLAGS = -Wall -Wextra 

SRC_DIR = src
OBJ_DIR = obj

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)
BINS = sender sigusr_rcvr sigblock sigwait_exp

all: $(BINS)

$(BINS): %: $(OBJ_DIR)/%.o
	$(CC) $(CFLAGS) -o $@ $< 

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR) 

clean:
	rm -rf $(BINS) $(OBJ_DIR) my_fifo

.PHONY: all clean