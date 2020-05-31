CC=gcc
INCLUDE = -I./include/lib
SOURCE1=$(wildcard include/wrapfunction/*.c)
SOURCE=$(wildcard src/*.c)


client: $(SOURCE)
	$(CC) -pthread -o client $(SOURCE) $(SOURCE1) $(INCLUDE) -ljson-c
