/*
 * main.cpp
 *
 *  Created on: Oct 7, 2018
 *      Author: Yuxia Sheng
 *      Code is from https://blog.csdn.net/caoshangpa/article/details/80393644
 *      C++ version print odd and even numbers in two threads
 *      Compile: g++ -std=c++11 main_cpp.cpp -lpthread -o foo
 */

#include <thread>
#include <iostream>
#include <mutex>
#include <condition_variable>

std::mutex data_mutex;
std::condition_variable data_var;
bool flag = true;

void printA()
{
    for (int i = 0; i < 10; i++)
    {
        //std::this_thread::sleep_for(std::chrono::seconds(1));
        std::unique_lock<std::mutex> lck(data_mutex) ;
        data_var.wait(lck,[]{return flag;});
        std::cout<<"thread: "<< std::this_thread::get_id() << "   printf: " << "A" <<std::endl;
        flag = false;
        data_var.notify_one();
    }
}

void printB()
{
    for (int i = 0; i < 10; i++)
    {
        std::unique_lock<std::mutex> lck(data_mutex) ;
        data_var.wait(lck,[]{return !flag;});
        std::cout<<"thread: "<< std::this_thread::get_id() << "   printf: " << "B" <<std::endl;
        flag = true;
        data_var.notify_one();
    }
}

int main()
{
    std::thread tA(printA);
    std::thread tB(printB);
    tA.join();
    tB.join();
    return 0;
}
