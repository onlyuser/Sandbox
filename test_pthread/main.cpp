#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *f(void *ptr)
{
    //pthread_mutex_lock(&mutex);
    char *s = (char *) ptr;
    for(int i = 0; i<3; i++)
    {
        printf("%s: %d\n", s, i);
        sleep(1);
    }
    //pthread_mutex_unlock(&mutex);
    return NULL;
}

int main(int argc, char** argv)
{
    const char *s1 = "Thread 1";
    const char *s2 = "Thread 2";
    pthread_t t1, t2;
    int r1 = pthread_create(&t1, NULL, f, (void*)s1);
    int r2 = pthread_create(&t2, NULL, f, (void*)s2);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Thread 1 returns: %d\n", r1);
    printf("Thread 2 returns: %d\n", r2);
    return 0;
}
