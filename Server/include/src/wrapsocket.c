#include "lib.h"

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
    int n;

    if((n = accept(fd, sa, salenptr)) < 0)
    {
        printf("accept error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return(n);
}

int Getpeername(int fd, struct sockaddr * addr, socklen_t *addrlen)
{
    int n;

    if((n = getpeername(fd, addr, addrlen)) == -1)
    {
        printf("getpeername error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return(n);
}

int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    int n;

    if((n = setsockopt(sockfd, level, optname, optval, optlen)) == -1)
    {
        printf("setsockopt error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return(n);
}

int Listen(int sockfd, int backlog)
{
    int n;

    if((n = listen(sockfd, backlog)) == -1)
    {
        printf("listen error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return(n);
}