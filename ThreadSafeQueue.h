//
//  Queue.h
//  Queue
//
//  Created by Shyam Prathish Sargunam on 5/8/18.
//  Copyright © 2018 Shyam Prathish Sargunam. All rights reserved.
//

#ifndef THREAD_SAFE_QUEUE
#define THREAD_SAFE_QUEUE

#include <iostream>
#include <memory>
#include <mutex>
#include <condition_variable>
#include "QueueInterface.h"

class ThreadSafeQueue: public QueueInterface
{
private:
    struct node
    {
        int data;
        std::unique_ptr<node> next;
    };
    
    std::unique_ptr<node> head;
    node* tail;     // not a unique or shared pointer so that 
    
    uint64_t len;
    
    mutable std::mutex m;
    std::condition_variable cv;
    
public:
    /*! Returns and removes the integer at the front of the queue.
     Should throw an std::out_of_range exception if the queue is
     currently empty
     */
    int pop();
    /*! Returns and removes the integer at the front of the queue.
     */
    int wait_and_pop();
    /*! Inserts an integer at the back of the queue
     */
    void push(int value);
    /*! Returns the current size of the queue
     */
    uint64_t size() const noexcept;
    /* prints the contents of the queue
     */
    void print();
    
    ThreadSafeQueue():len(0)
    {
    }
    
    ~ThreadSafeQueue()
    {
    }
};

#endif /* THREAD_SAFE_QUEUE */
