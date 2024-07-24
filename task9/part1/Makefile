CC = gcc
CFLAGS = -Wall -Wextra

SRC = main.c
OBJ = main.o
BIN = app

$(BIN) : $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f $(BIN) $(OBJ) output.txt

run:
	./$(BIN)

.PHONY: all clean run