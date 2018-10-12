/**
 * main.cpp
 *
 *  Created on: Oct 9, 2018
 *      Author: Yuxia Sheng
 *      Code is pthread version of https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
 *      pthread version thread-safe queue
 *      Compile: g++ main.cpp -lpthread -o foo
 **/


#include <iostream>
#include <pthread.h>
 
#include <queue>
 
using namespace std;
// Queue class that has thread synchronisation

template<typename Data>
class concurrent_queue
{
private:
    std::queue<Data> the_queue;
    
    pthread_mutex_t the_mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_cond_t the_condition_variable;
    
public:
    concurrent_queue() {
      pthread_mutex_init(&the_mutex, NULL);
      pthread_cond_init(&the_condition_variable, NULL);
    }
    void push(Data const& data)
    {
        pthread_mutex_lock(&the_mutex);
        the_queue.push(data);
        pthread_mutex_unlock(&the_mutex);        
        pthread_cond_signal(&the_condition_variable); 
    }

    bool empty() const
    {
        bool res = false;
        pthread_mutex_lock(&the_mutex);
        res = the_queue.empty();
        pthread_mutex_unlock(&the_mutex);
        return res;
    }

    bool try_pop(Data& popped_value)
    {
        pthread_mutex_lock(&the_mutex);
        if(the_queue.empty())
        {
            pthread_mutex_unlock(&the_mutex);
            return false;
        }
        
        popped_value=the_queue.front();
        the_queue.pop();
        pthread_mutex_unlock(&the_mutex);
        return true;
    }

    void wait_and_pop(Data& popped_value)
    {
        pthread_mutex_lock(&the_mutex);
        while(the_queue.empty())
        {
            pthread_cond_wait(&the_condition_variable, &the_mutex);
        }
        
        popped_value=the_queue.front();
        the_queue.pop();
        pthread_mutex_unlock(&the_mutex);
    }

};

int main(){

  concurrent_queue<int> cq;
  cq.push(1);
  cq.push(2);
  int res = 0;
  cq.try_pop(res);
  cout << " pop res = " << res << endl;
  cq.try_pop(res);
  cout << " pop res = " << res << endl;
  
  return 0;
}
