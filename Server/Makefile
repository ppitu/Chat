CC=gcc
INCLUDE = -I./include/lib
SOURCE1=$(wildcard include/src/*.c)
SOURCE=$(wildcard src/*.c)


server: $(SOURCE)
	$(CC) -pthread -o server $(SOURCE) $(SOURCE1) $(INCLUDE)
