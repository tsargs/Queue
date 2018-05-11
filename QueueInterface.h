//
//  QueueInterface.h
//  Queue
//
//  Created by Shyam Prathish Sargunam on 5/8/18.
//  Copyright © 2018 Shyam Prathish Sargunam. All rights reserved.
//

#ifndef QUEUE_INTERFACE
#define QUEUE_INTERFACE

#include <cstdint>

class QueueInterface
{
public:
    /*! Returns and removes the integer at the front of the queue.
     Should throw an std::out_of_range exception if the queue is
     currently empty
     */
    virtual int pop() = 0;
    /*! Inserts an integer at the back of the queue
     */
    virtual void push(int value) = 0;
    /*! Returns the current size of the queue
     */
    virtual uint64_t size() const noexcept = 0;
    /* prints the contents of the queue
     */
    virtual void print() = 0;
};

#endif /* QUEUE_INTERFACE */
