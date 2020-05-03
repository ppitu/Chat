CC=gcc
INCLUDE = -I./include/lib
SOURCE1=$(wildcard include/src/*.c)
SOURCE=$(wildcard src/*.c)


client: $(SOURCE)
	$(CC) -pthread -o client $(SOURCE) $(SOURCE1) $(INCLUDE)
