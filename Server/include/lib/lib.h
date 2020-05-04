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

void *checkedMalloc(size_t);
void *checkedCalloc(size_t, size_t);
ssize_t checkedRead(int, void *, size_t);
ssize_t checkedWrite(int, const void *, size_t);
int checkedClose(int);

void checkedPthread_create(pthread_t *, const pthread_attr_t *, void *(*)(void *), void*);
int checkedPthread_detach(pthread_t);

int checkedAccept(int, struct sockaddr *, socklen_t *);
int checkedGetpeername(int, struct sockaddr *, socklen_t *);
int checkedSetsockopt(int, int, int, const void *, socklen_t);
int checkedListen(int, int);

#endif //LIB_H_
