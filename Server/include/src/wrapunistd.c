#include "lib.h"

void *checkedMalloc(size_t size)
{
    void *ptr;

    if((ptr = malloc(size)) == NULL)
    {
        printf("malloc error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);

        return ptr;
    }
}

ssize_t checkedRead(int fd, void *buf, size_t count)
{
    ssize_t n;

    if((n = read(fd, buf, count)) == -1)
    {
        printf("read error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return(n);
}

void *checkedCalloc(size_t nmemb, size_t size)
{
    void *ptr;

    if((ptr = calloc(nmemb, size)) == NULL)
    {
        printf("calloc error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return(ptr);
}

ssize_t checkedWrite(int fd, const void *buf, size_t count)
{
    ssize_t n;

    if((n = write(fd, buf, count)) == -1)
    {
        printf("write error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return(n);

}

int checkedClose(int fd)
{
    int n;

    if((n = close(fd)) == - 1)
    {
        printf("close error: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    return(n);
}