//
//  Queue.cpp
//  Queue
//
//  Created by Shyam Prathish Sargunam on 5/8/18.
//  Copyright © 2018 Shyam Prathish Sargunam. All rights reserved.
//

#include "Queue.h"


/*! Returns and removes the integer at the front of the queue.
 Should throw an std::out_of_range exception if the queue is
 currently empty
 */
int Queue::pop()
{
    if(head != nullptr)
    {
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
void Queue::push(int value)
{
    std::unique_ptr<node> new_node = std::make_unique<node>();
    new_node->data = value;
    //new_node->next is nullptr by default
    
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
    
    ++len;
}


/*! Returns the current size of the queue
 */
uint64_t Queue::size() const noexcept
{
    return len;
}


void Queue::print()
{
    std::cout << "Printing" << std::endl;
    node* p = head.get();
    while(p != nullptr)
    {
        std::cout<< p->data << std::endl;
        p = p->next.get();
    }
}
