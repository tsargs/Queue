//
//  Queue.cpp
//  Queue
//
//  Created by Shyam Prathish Sargunam on 5/8/18.
//  Copyright © 2018 Shyam Prathish Sargunam. All rights reserved.
//

#include "ThreadSafeQueue.h"

/*! Returns and removes the integer at the front of the queue.
 */
int ThreadSafeQueue::wait_and_pop()
{
    std::unique_lock<std::mutex> u_lock(m);
    cv.wait(u_lock, [this](){return head != nullptr;});
    
    std::unique_ptr<node> temp = std::move(head);
    head = std::move(temp->next);
    --len;
    
    if(head == nullptr)
        tail = nullptr;
    
    u_lock.unlock();
    
    return temp->data;
}

/*! Returns and removes the integer at the front of the queue.
 Should throw an std::out_of_range exception if the queue is
 currently empty
 */
int ThreadSafeQueue::pop()
{
    if(head != nullptr)
    {
        std::lock_guard<std::mutex> lg(m);
        std::unique_ptr<node> temp = std::move(head);
        head = std::move(temp->next);
        --len;
        
        if(head == nullptr)
            tail = nullptr;
        
        return temp->data;
    }   // temp is deallocated here because it goes out of scope
    else
        throw std::out_of_range("Queue is empty");
    
    return false;
}


/*! Inserts an integer at the back of the queue
 */
void ThreadSafeQueue::push(int value)
{
    std::unique_ptr<node> new_node = std::make_unique<node>();
    new_node->data = value;         //new_node->next is unique_ptr. So its initialized to nullptr
    
    std::lock_guard<std::mutex> lg(m);
    
    if(tail != nullptr)
    {
        tail->next = std::move(new_node);
        tail = tail->next.get();
    }
    else
    {
        //both head and tail are null pointers
        tail = new_node.get();
        head = std::move(new_node);
    }
    
    cv.notify_all();
    
    ++len;
}


/*! Returns the current size of the queue
 */
uint64_t ThreadSafeQueue::size() const noexcept
{
    std::lock_guard<std::mutex> lg(m);
    return len;
}


void ThreadSafeQueue::print()
{
    std::lock_guard<std::mutex> lg(m);
    std::cout << "Printing" << std::endl;
    node* p = head.get();
    while(p != nullptr)
    {
        std::cout<< p->data << std::endl;
        p = p->next.get();
    }
}
