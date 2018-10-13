/**
 * main.cpp
 *
 *  Created on: Oct 9, 2018
 *      Author: Yuxia Sheng
 *      pThread based ring buffer
 *      Compile: g++ main.cpp -lpthread -o foo
 *      https://www.programering.com/a/MzNygzMwATI.html
 **/


#include <iostream>
#include <pthread.h>
#include <unistd.h>
 
#include <queue>
 
using namespace std;
// Ring buffer class that has thread synchronisation

template<typename Data>
class ring_buffer
{
private:
	int size;          /*number of data in the current buffer*/
	int next_in;       /*location of next data in*/
	int next_out;      /*location of next output data in buffer*/
	int capacity;      /*total buffer size*/

    std::vector<Data> data_queue;
    
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t  not_full, not_empty;
    
public:
    void cbuf_init(int cap) {
      size = 0;
      next_in = 0;
      next_out = 0;
      capacity = cap;

      data_queue.resize(cap, 0);
    }
    void cbuf_enqueue(Data data_in)
    {
        pthread_mutex_lock(&mutex);
        while (cbuf_full())
        	pthread_cond_wait(&not_full, &mutex);
        data_queue[next_in++] = data_in;
        size++;
        next_in %= capacity;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&not_empty);

    }
    void cbuf_dequeue(Data &data_out){
        pthread_mutex_lock(&mutex);
        while (cbuf_empty())
        	pthread_cond_wait(&not_empty, &mutex);
        data_out = data_queue[next_out++];
        size--;
        next_out %= capacity;
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&not_full);
    }


    bool cbuf_empty()
    {
        return size == 0;
    }

    bool cbuf_full()
    {
        return size == capacity;
    }

    int cbuf_capacity()
    {
        return capacity;
    }

};

ring_buffer<int> rb;
static bool line1_finish = false;
static bool line2_finish = false;

void* producer_1(void *ptr)
{
    for(int i = 0; i < 5; i++)
    {
        rb.cbuf_enqueue(2*i);
        sleep(1);
    }
    line1_finish = true;
    cout << " line1_finish " << line1_finish << endl;
    return NULL;
}

void* producer_2(void *ptr)
{
    for(int i = 0; i < 5; i++)
    {
        rb.cbuf_enqueue(2*i+1);
        sleep(1);
    }
    line2_finish = true;
    cout << " line2_finish " << line2_finish << endl;
    return NULL;
}

void* consumer(void *ptr)
{
    int tmp;

    for (int i = 0; i < 10; i++)
    {
        
      rb.cbuf_dequeue(tmp);
        cout << "output " << tmp << endl;
        cout << " rb.cbuf_empty() " << rb.cbuf_empty() 
             << " line1_finish " << line1_finish
             << " line2_finish " << line2_finish << endl;

    }

    return NULL;
}

int main(){


    pthread_t    l_1;
    pthread_t    l_2;
    pthread_t    c;

    rb.cbuf_init(1);

    pthread_create(&l_1,NULL,producer_1,0);
    pthread_create(&l_2,NULL,producer_2,0);
    pthread_create(&c,NULL,consumer,0);

    pthread_join(l_1,NULL);
    pthread_join(l_2,NULL);
    pthread_join(c,NULL);
  
  return 0;
}
