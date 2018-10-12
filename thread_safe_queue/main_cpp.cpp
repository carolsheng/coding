/**
 * main.cpp
 *
 *  Created on: Oct 9, 2018
 *      Author: Yuxia Sheng
 *      Code is from https://www.justsoftwaresolutions.co.uk/threading/implementing-a-thread-safe-queue-using-condition-variables.html
 *      C++ version thread-safe queue
 *      Compile: g++ -std=c++11 main_cpp.cpp -lpthread -o foo
 **/


#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
 
#include <queue>
 
using namespace std;
// Queue class that has thread synchronisation

template<typename Data>
class concurrent_queue
{
private:
    std::queue<Data> the_queue;
    std::mutex the_mutex;
    std::condition_variable the_condition_variable;
public:
    void push(Data const& data)
    {
        std::unique_lock<std::mutex> lock(the_mutex);
        the_queue.push(data);
        lock.unlock();
        the_condition_variable.notify_one();
    }

    bool empty() const
    {
        std::unique_lock<std::mutex> lock(the_mutex);
        return the_queue.empty();
    }

    bool try_pop(Data& popped_value)
    {
        std::unique_lock<std::mutex> lock(the_mutex);
        if(the_queue.empty())
        {
            return false;
        }
        
        popped_value=the_queue.front();
        the_queue.pop();
        return true;
    }

    void wait_and_pop(Data& popped_value)
    {
        std::unique_lock<std::mutex> lock(the_mutex);
        while(the_queue.empty())
        {
            the_condition_variable.wait(lock);
        }
        
        popped_value=the_queue.front();
        the_queue.pop();
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
