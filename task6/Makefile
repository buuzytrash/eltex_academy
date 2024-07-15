CC = gcc
CFLAGS = -Wall -Wextra

SRC = main.c
OBJ = main.o
BIN = abonents

$(BIN) : $(OBJ)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ) 

%.o: %.c
	$(CC) $(CFLAGS) -c $<


clean:
	rm -f $(BIN) $(OBJ)

run:
	./$(BIN)

.PHONY: all clean