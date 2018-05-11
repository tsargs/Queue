# Queue
A simple queue implementation and a thread safe queue implementation.

Thread safe queue implementation:

a)	Since it might be preferable sometimes to have a thread wait for data to arrive on an empty queue, a wait_and_pop() method was included in the ThreadSafeQueue class. It uses condition_variable.wait() so that, if the queue is empty at the time of this call, then the thread waits for notification from push() function about insertion of new data through condition_variable.notify_all() facility.

Testing:

a)	I came up very basic test cases based on simple producer – consumer design. The test cases 02 through 07 test multiple threads of producer and/or consumer.

b)	The producer and consumer threads perform push and pop operations on the queue based on push_count, pop_count, MAX_PUSH_COUNT and MAX_POP_COUNT values. The producers and consumers however do not tightly follow the conditions based on these four variables to produce synchronized, expected number of calls to push() and pop(). To do that successfully, the variables push_count and pop_count need to be protected by separate mutex variables. However, that would make the underlying push and pop operations synchronized at a higher level and I thought it would not be a challenging test case to see if the queue works as expected. So, in some cases, because push_count and pop_count are not protected, the number of push and pop operations do not exactly match the MAX_PUSH_COUNT and MAX_POP_COUNT values. 

c)	Because of the above design for push and pop operations, multiple threads will not produce same, consistent results all the time. So, approximate, expected outcomes were tested just by analyzing the console logs.

d)	We can see that wait_and_pop() is called from consumer threads, they wait till data is inserted into the queue whereas when pop() function is called, the immediately return throwing std::out_of_range exception

