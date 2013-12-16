// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread14.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <string>
#include <thread>
#include <future> // promise, future 

// Tasks and exceptions.

// Call a function asynchronously (analogically and much simpler to thread 13 example).

// The function looks just like a regular function (return values, throw exceptions).
std::string fun() 
{
    std::string str("Hello from future!");

    throw(std::exception()); //< Throw a standard exception.

    // When working with a task there is no need to catch this exception here.

    return str;
}

int main (int argc, char **argv)
{
    // Start a task and use the future to retrieve the returning value. 
    auto ftr = std::async(&fun);

    // This is exactly the same mechanism - it is the syntactic sugar over
    // thread13.cpp example. 

    std::cout << "Hello from main!\n";

    try
    {
        std::string str = ftr.get(); //< Future is the receiving end of the channel.   
        std::cout << str << std::endl;
    }
    catch(std::exception & e) // Catch the exception that was passed by the channel between the task and calling thread.
    {
       std::cout << e.what() << std::endl;
    }

    // Note that there is no thread join() here because there is no thread.
    
    return 0;
}
