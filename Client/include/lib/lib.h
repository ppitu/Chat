#ifndef LIB_H_
#define LIB_H_

/**************INCLUDES*****************/

#include	<sys/types.h>
#include	<sys/socket.h>
#include	<sys/time.h>
#include	<time.h>
#include	<netinet/in.h>
#include	<arpa/inet.h>
#include	<errno.h>
#include	<fcntl.h>
#include	<netdb.h>
#include	<signal.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<sys/stat.h>
#include	<sys/uio.h>	
#include	<unistd.h>
#include	<sys/wait.h>
#include	<sys/un.h>
#include 	<pthread.h>
#include 	<signal.h>

/*********DEFINES************************/

#define MAXLINE 1024
#define NAMELENGTH 31

/********FUNCTIONS***********************/

int tcp_listen(const char *, const char* , socklen_t *); //implementation in tcp_lisetn.c

int tcp_connect(const char*, const char* ); //implementation in tcp_connect.c

ssize_t Readline(int , void *, size_t);

/********WRAPPING FUNCTIONS***************/

void *Malloc(size_t);
void *Calloc(size_t, size_t);
ssize_t Read(int, void *, size_t);
ssize_t Write(int, const void *, size_t);
int Close(int);

void Pthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void*);
int Pthread_detach(pthread_t);

int Accept(int, struct sockaddr *, socklen_t *);
int Getpeername(int, struct sockaddr *, socklen_t *);
int Setsockopt(int, int, int, const void *, socklen_t);
int Listen(int, int);

#endif //LIB_H_
