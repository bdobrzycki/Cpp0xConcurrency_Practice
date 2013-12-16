// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread06.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <thread>
#include <algorithm> // for_each()
#include <cassert>

// Shearing data between threads.
// NOTE: This program compiles and runs but it is not correct.

// Thread function.
void thFun(int & i) // 1. We want to pass the argument to the thread function by reference.
{
    std::cout << "Hi from worker " << i << "!\n";
}

// Function creates threads and pushes them on the stack and starts them.
void test(std::vector<std::thread> & workers)
{
    // Start threads.  
    for (int i = 0; i < 8; ++i) // local variable i
    {   
        auto th = std::thread(&thFun, std::ref(i)); // 2. Pass i (i is sheared memory) to the thread function by reference using std::ref().

        workers.push_back(std::move(th)); // Use move semantics.

        assert(!th.joinable());
    }
}

int main (int argc, char **argv)
{
    // Fork-join parallelism
    std::vector<std::thread> workers; // Vector of worker threads.
    
    test(workers);
    
    std::cout << "Hi from main!\n";

    // Barrier i.e. joining all the threads to the main thread. 
    std::for_each(workers.begin(), workers.end(), [](std::thread& th) // Lambda is taking a parameter (the element stored by the vector).
    { 
        assert(th.joinable());
        th.join();
    });

    // Compile this program and run it.
    // You shall see "Hi from worker 2" twice or something along these lines.
    // You shall see weird printouts "Hi from worker -1074500036" or similar that
    // do not correspond to any of these counters.
    //
    // What happened here?
    // -------------------
    // After the call to test() finished this local variable i disappeared - it was on the stack.
    // The thred function took reference to this variable on the stack and the function test() returned
    // so that stack was then reused by the other function call - overwritten by new values.
    // The fact that number 2 was printed out twice is because both of these thread functions took the reference to the same variable
    // and picked whatever value was there at the moment. The variable i (sheared memory) goes out of scope (i.e. passing reference of local variable).

    // This is just one of many dangers of passing references to threads. 
    // The lifetime of the objects you refer to. 

    return 0;
}
