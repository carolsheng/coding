/*
 * main1.cpp
 *
 *  Created on: Oct 13, 2018
 *      Author: Yuxia Sheng
 *      Ref: https://blog.csdn.net/haoel/article/details/1948051
 *      virtual function study
 *      Compile: g++ main1.cpp -o foo
 */


#include<stdio.h>
#include<stdlib.h>
#include <iostream>

using namespace std;

class A
{
public:
    virtual void print()
    {cout<<"A"<<endl;}
};
 
class B:public A
{
public:
    void print()
    {cout<<"B"<<endl;}
};

void populate(A* a)
{
  a->print();
}

int main()
{
   A a;
   B b;
  
   b.print();
   
   populate(&b);
 
   return 0;
}
