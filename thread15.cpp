// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread15.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <string>
#include <thread>
#include <future> // promise, future 
#include <chrono> // time duration

// Tasks

// Call a function asynchronously (analogically and much simpler to thread 13 example).

// Function does not return any value.
void fun() 
{
   std::cout << "Starting task\n"; 
   std::this_thread::sleep_for(std::chrono::seconds(8));
   std::cout << "Ending task\n"; 
}

int main (int argc, char **argv)
{
    // Start a task. There is no future get() because fun() does not return any value.
    auto ftr = std::async(&fun);
    std::cout << "Exiting from main!\n";

    // There is no thread join() because there is no thread. There is an async task. 

    // Thread join() and future get() are these two places where the execution of the thread
    // is forced. Now we have void fun() so when create a future for this task there is no need 
    // to call get() from the future - there is no value return by the thread.

    // There is no join() and no get() - there is nothing forcing the thread to execute.
    // The destructor of the future is the final thing that can force the execution of the task.

    // 1. Destructor of the future is called here.
    // 2. The task is started by this.
    // Main can't exit before the task has finished executing.
    // TODO: Does not work for me. Main exits unless I added the line below:
    ftr.get(); // Only after adding this line the program works as intended. 
}
