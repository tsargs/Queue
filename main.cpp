//
//  main.cpp
//  Queue
//
//  Created by Shyam Prathish Sargunam on 5/8/18.
//  Copyright © 2018 Shyam Prathish Sargunam. All rights reserved.
//

#include <iostream>
#include <vector>
#include <thread>
#include "Queue.h"
#include "ThreadSafeQueue.h"

// counters for concurrent push and pop operations
static int push_count = 0;
static int pop_count = 0;

// maximum number of push, pop operations for each test case
static int MAX_PUSH_COUNT;
static int MAX_POP_COUNT;

enum class PopOption
{
    POP,            // If queue is empty, out_of_range exception is thrown
    WAIT_AND_POP    // Threads will wait till queue has some data to perform pop()
};

enum class TestCase
{
    TESTCASE_01,
    TESTCASE_02,
    TESTCASE_03,
    TESTCASE_04,
    TESTCASE_05,
    TESTCASE_06,
    TESTCASE_07
};

std::string local_pop(Queue& q)
{
    std::string exception_msg;
    
    try
    {
        return "Popping " + std::to_string(q.pop());
    }
    catch(const std::out_of_range& e)
    {
        exception_msg = "Out of range exception: " + std::string(e.what());
    }
    catch (...)
    {
        exception_msg = "Some Exception";
    }
    
    return exception_msg;
}

void test_queue()
{
    Queue q;
    std::cout << "Pushing\n";
    q.push(1);
    q.push(2);
    q.push(3);
    q.push(4);
    q.push(5);
    
    q.print();
    
    std::cout << local_pop(q) << std::endl;
    std::cout << local_pop(q) << std::endl;
    std::cout << local_pop(q) << std::endl;
    
    q.print();
    
    std::cout << local_pop(q) << std::endl;
    std::cout << local_pop(q) << std::endl;
    std::cout << local_pop(q) << std::endl;
    std::cout << local_pop(q) << std::endl;
    
    q.push(7);
    q.push(8);
    
    q.print();
    
    std::cout << local_pop(q) << std::endl;
    std::cout << local_pop(q) << std::endl;
    std::cout << local_pop(q) << std::endl;
    std::cout << local_pop(q) << std::endl;
}

void producer(ThreadSafeQueue* q)
{
    while(push_count < MAX_PUSH_COUNT)
    {
        q->push(++push_count);
    }
}

void consumer(ThreadSafeQueue* q, PopOption option)
{
    try
    {
        while(pop_count < MAX_POP_COUNT)
        {
            if(option == PopOption::POP)
                std::cout << q->pop()<< " popped" << std::endl;
            else if(option == PopOption::WAIT_AND_POP)
                std::cout << q->wait_and_pop()<< " popped" << std::endl;
            ++pop_count;
        }
    }
    catch(const std::out_of_range& e)
    {
        std::cout<< "Out of range exception: " << e.what() << std::endl;
    }
    catch (...)
    {
        std::cout<< "Some Exception" << std::endl;
    }
}

//tests single thread of producer and single thread of consumer
void test_case_01()
{
    ThreadSafeQueue q;
    
    MAX_PUSH_COUNT = 10;
    MAX_POP_COUNT = 8;
    
    std::thread p(producer, &q);
    std::thread c(consumer, &q, PopOption::POP);
    
    if(p.joinable())
        p.join();
    
    if(c.joinable())
        c.join();
    
    q.print();
}


//tests three threads of producer and three threads of consumer
void test_case_02()
{
    ThreadSafeQueue q;
    
    MAX_PUSH_COUNT = 100;
    MAX_POP_COUNT = 100;
    
    int thread_count = 3;
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    for(int i = 0; i < thread_count; i++)
    {
        producers.push_back(std::thread(producer, &q));
        consumers.push_back(std::thread(consumer, &q, PopOption::POP));
    }

    for(int i = 0; i < thread_count; i++)
    {
        if(producers[i].joinable())
            producers[i].join();
        
        if(consumers[i].joinable())
            consumers[i].join();
    }
    
    q.print();
    std::cout << "Size: " << q.size() << std::endl;
}

//tests fewer number of producer threads compared to consumer threads
//pop() method immediately returns if queue is empty
void test_case_03()
{
    ThreadSafeQueue q;
    
    MAX_PUSH_COUNT = 50;
    MAX_POP_COUNT = 80;
    
    int producer_thread_count = 2;
    int consumer_thread_count = 6;
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    for(int i = 0; i < producer_thread_count; i++)
        producers.push_back(std::thread(producer, &q));
    
    for(int i = 0; i < consumer_thread_count; i++)
        consumers.push_back(std::thread(consumer, &q, PopOption::POP));
    
    for(int i = 0; i < producer_thread_count; i++)
    {
        if(producers[i].joinable())
            producers[i].join();
    }
    
    for(int i = 0; i < consumer_thread_count; i++)
    {
        if(consumers[i].joinable())
            consumers[i].join();
    }

    q.print();
    std::cout << "Size: " << q.size() << std::endl;
}

//tests fewer number of producer threads compared to consumer threads
//wait_and_pop() method waits for the queue to have new data that can be popped
void test_case_04()
{
    ThreadSafeQueue q;
    
    MAX_PUSH_COUNT = 50;
    MAX_POP_COUNT = 80;
    
    int producer_thread_count = 2;
    int consumer_thread_count = 6;
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    for(int i = 0; i < producer_thread_count; i++)
        producers.push_back(std::thread(producer, &q));
    
    for(int i = 0; i < consumer_thread_count; i++)
        consumers.push_back(std::thread(consumer, &q, PopOption::WAIT_AND_POP));
    
    for(int i = 0; i < producer_thread_count; i++)
    {
        if(producers[i].joinable())
            producers[i].join();
    }
    
    for(int i = 0; i < consumer_thread_count; i++)
    {
        if(consumers[i].joinable())
            consumers[i].join();
    }
    
    q.print();
    std::cout << "Size: " << q.size() << std::endl;
}

//tests one producer threads vs twenty consumer threads
//pop() method immediately returns if queue is empty
void test_case_05()
{
    ThreadSafeQueue q;
    
    MAX_PUSH_COUNT = 20;
    MAX_POP_COUNT = 180;
    
    int producer_thread_count = 1;
    int consumer_thread_count = 20;
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    for(int i = 0; i < consumer_thread_count; i++)
        consumers.push_back(std::thread(consumer, &q, PopOption::POP));
    
    for(int i = 0; i < producer_thread_count; i++)
        producers.push_back(std::thread(producer, &q));

    for(int i = 0; i < producer_thread_count; i++)
    {
        if(producers[i].joinable())
            producers[i].join();
    }
    
    for(int i = 0; i < consumer_thread_count; i++)
    {
        if(consumers[i].joinable())
            consumers[i].join();
    }
    
    q.print();
    std::cout << "Size: " << q.size() << std::endl;
}

//tests one producer threads vs twenty consumer threads
//wait_and_pop() method waits for the queue to have new data that can be popped
void test_case_06()
{
    ThreadSafeQueue q;
    
    MAX_PUSH_COUNT = 20;
    MAX_POP_COUNT = 180;
    
    int producer_thread_count = 1;
    int consumer_thread_count = 20;
    
    std::vector<std::thread> producers;
    std::vector<std::thread> consumers;
    
    for(int i = 0; i < consumer_thread_count; i++)
        consumers.push_back(std::thread(consumer, &q, PopOption::WAIT_AND_POP));
    
    for(int i = 0; i < producer_thread_count; i++)
        producers.push_back(std::thread(producer, &q));
    
    for(int i = 0; i < producer_thread_count; i++)
    {
        if(producers[i].joinable())
            producers[i].join();
    }
    
    for(int i = 0; i < consumer_thread_count; i++)
    {
        if(consumers[i].joinable())
            consumers[i].join();
    }
    
    q.print();
    std::cout << "Size: " << q.size() << std::endl;
}

//tests single producer thread for sequential insertion of data
void test_case_07()
{
    ThreadSafeQueue q;
    
    MAX_PUSH_COUNT = 100;
    
    int producer_thread_count = 20;
    
    std::vector<std::thread> producers;
    
    for(int i = 0; i < producer_thread_count; i++)
        producers.push_back(std::thread(producer, &q));
    
    for(int i = 0; i < producer_thread_count; i++)
    {
        if(producers[i].joinable())
            producers[i].join();
    }
    
    q.print();
    std::cout << "Size: " << q.size() << std::endl;
}

void test_thread_safe_queue(TestCase test_case)
{
    switch (test_case)
    {
        case TestCase::TESTCASE_01:
            test_case_01();
            break;
            
        case TestCase::TESTCASE_02:
            test_case_02();
            break;
            
        case TestCase::TESTCASE_03:
            test_case_03();
            break;
            
        case TestCase::TESTCASE_04:
            test_case_04();
            break;
            
        case TestCase::TESTCASE_05:
            test_case_05();
            break;
            
        case TestCase::TESTCASE_06:
            test_case_06();
            break;
            
        case TestCase::TESTCASE_07:
            test_case_07();
            break;
            
        default:
            break;
    }
}

int main(int argc, const char * argv[])
{
    //test_queue();
    
    test_thread_safe_queue(TestCase::TESTCASE_01);

    std::cin.get();
    
    return 0;
}

