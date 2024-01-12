CC = gcc
CFLAGS = -I./include -I./sql -Wall
LDFLAGS = -lsqlite3

# Kaynak dosyaları ve nesne dosyaları
SRC = $(wildcard src/server/*.c) $(wildcard src/client/*.c) $(wildcard sql/*.c)
OBJ = $(SRC:.c=.o)

# Hedefler
all: server client

server: src/server/main.o src/server/database.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

client: src/client/main.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

sql: sql/shell.o sql/sqlite3.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f src/server/*.o src/client/*.o server client
