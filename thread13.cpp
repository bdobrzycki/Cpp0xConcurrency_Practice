// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread13.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <string>
#include <thread>
#include <future> // promise, future 

// Exceptions.

// If an exception is thrown inside the thread and is not caught
// the program will terminate. It is always a good idea to catch an exception
// inside the thread and handle it there OR pass this exception back
// to the main thread. 

void thFun(std::promise<std::string> && prms) 
{
    try
    {
        std::string str("Hello from future!");
                
        throw(std::exception()); //< Throw a standard exception. 

        prms.set_value(str);
    }
    catch(...) //< Catch all exceptions and pass them back
               //  to the main thread.
    {
        // Set the exception in the promise.
        prms.set_exception(std::current_exception()); //< current_exception() will retrieve
                                                      //  any exception and make a copy of it
    }
}

int main (int argc, char **argv)
{
    std::promise<std::string> prms;

    // Get the future before starting the worker thread.
    // After the move operation the future is invalidated.
    std::future<std::string> ftr = prms.get_future();

    std::thread th(&thFun, std::move(prms));

    std::cout << "Hello from main!\n";

    // Exception will be re-thrown when we call get() from future.
    try
    {
        std::string str = ftr.get(); //< Future is the receiving end of the channel.   
        std::cout << str << std::endl;
    }
    catch(std::exception & e)  //< Handle specific exception.
    {
        std::cout << e.what() << std::endl;
    }
   
    th.join();

    return 0;
}
