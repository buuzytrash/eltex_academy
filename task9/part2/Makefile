CC = gcc 
CFLAGS = -Wall -Wextra

SRC = main.c 
OBJ = main.o
BIN = manager

$(BIN) : $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) -lncurses

%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f $(BIN) $(OBJ)

run:
	./$(BIN)

.PHONY: all clean