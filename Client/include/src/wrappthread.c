#include "lib.h"

void Pthread_create(pthread_t *tid, const pthread_attr_t *attr, void *(*func)(void*), void *arg)
{
    int n;

    if((n = pthread_create(tid, attr, func, arg)) == 0)
        return;

    printf("pthread_create error: %s\n", gai_strerror(n));
    exit(EXIT_FAILURE);
}

int Pthread_detach(pthread_t thread)
{
    int n;

    if((n = pthread_detach(thread)) == 0)
        return(n);

    printf("pthread_detach error: %s\n", gai_strerror(n));
    exit(EXIT_FAILURE);
}