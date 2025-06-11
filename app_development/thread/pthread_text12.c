#define _GNU_SOURCE 
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

sem_t sem1, sem2, sem3;

void *fun1(void *arg)
{
    sem_wait(&sem1);
	printf("%s:Pthread Come!\n",__FUNCTION__);
	sem_post(&sem2);
    pthread_exit(NULL);
}

void *fun2(void *arg)
{
    sem_wait(&sem2);
	printf("%s:Pthread Come!\n",__FUNCTION__);
	sem_post(&sem3);
    pthread_exit(NULL);
}

void *fun3(void *arg)
{
    sem_wait(&sem3);
	printf("%s:Pthread Come!\n",__FUNCTION__);
	sem_post(&sem1);
    pthread_exit(NULL);
}

int main()
{
	int ret;
	pthread_t tid1, tid2, tid3;

    ret = sem_init(&sem1, 0, 1);
	if(ret != 0)
    {
		perror("pthread_create");
		return -1;
	}

    ret = sem_init(&sem2, 0, 0);
	if(ret != 0)
    {
		perror("pthread_create");
		return -1;
	}

    ret = sem_init(&sem3, 0, 0);
	if(ret != 0)
    {
		perror("pthread_create");
		return -1;
	}
    
	ret = pthread_create(&tid1, NULL, fun1, NULL);
	if(ret != 0)
    {
		perror("pthread_create");
		return -1;
	}
	ret = pthread_create(&tid2, NULL, fun2, NULL);
	if(ret != 0)
    {
		perror("pthread_create");
		return -1;
	}

	ret = pthread_create(&tid3, NULL, fun3, NULL);
	if(ret != 0)
    {
		perror("pthread_create");
		return -1;
	}

	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	pthread_join(tid3, NULL);

    sem_destroy(&sem1);
    sem_destroy(&sem2);
    sem_destroy(&sem3);

	return 0;
}
