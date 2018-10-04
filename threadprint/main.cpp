/*
 * main.cpp
 *
 *  Created on: Oct 3, 2018
 *      Author: Yuxia Sheng
 *      Ref: https://blog.csdn.net/Sanjiye/article/details/78766542?utm_source=copy
 *      print odd and even numbers in two threads
 */


#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<pthread.h>

#define COUNT 20
static  int   i = 1;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  even  = PTHREAD_COND_INITIALIZER;
pthread_cond_t  uneven = PTHREAD_COND_INITIALIZER;

/*
 * can also be initialized (dynamic way) as pthread_mutex_init ( &mutex, NULL);
 */

// print odd number
void* funPrint1(void *arg)
{
    while(i <= COUNT)
    {
        pthread_mutex_lock(&mutex);
        if(i % 2 != 0)
        {
            printf("thread 1 : %d\n",i);
            ++i;
            pthread_cond_signal(&uneven);
        }
        else
            pthread_cond_wait(&even, &mutex);
        pthread_mutex_unlock(&mutex);
    }
}
// print even number
void* funPrint2(void *arg)
{
    while(i <= COUNT)
    {
        pthread_mutex_lock(&mutex);
        if(i % 2 == 0)
        {
            printf("thread 2 : %d\n",i);
            ++i;
            pthread_cond_signal(&even);
        }
        else
            pthread_cond_wait(&uneven, &mutex);
        pthread_mutex_unlock(&mutex);
    }
}

int main()
{
    pthread_t tid1, tid2;
    pthread_create(&tid1, NULL, funPrint1, NULL);
    pthread_create(&tid2, NULL, funPrint2, NULL);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    return 0;
}
