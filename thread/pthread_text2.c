#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

void *fun(void *arg)
{
    printf("pthread_New = %lu\n", (unsigned long)pthread_self());
}

int main()
{
    pthread_t tid1;
    int ret = pthread_create(&tid1, NULL, fun, NULL);
    if (ret != 0)
    {
        perror("pthread_create");
        return -1;
    }

    printf("tid_main = %lu tid_new = %lu\n", (unsigned long)pthread_self(), (unsigned long)tid1);

    sleep(1);
    
    return 0;
}