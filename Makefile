CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/ui.c src/services.c src/database.c src/utils.c
TARGET = bank

all:
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET)