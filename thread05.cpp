// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread05.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <thread>
#include <algorithm> // for_each()
#include <cassert>

// Thread function.
void thFun(int i)
{
    std::cout << "Hi from worker " << i << "!\n";
}

int main (int argc, char **argv)
{
    // Fork-join parallelism
    std::vector<std::thread> workers; // Vector of worker threads.
    
    // Start threads.  
    for (int i = 0; i < 8; ++i)
    {   
        auto th = std::thread(&thFun, i); // Pass i to the thread function by value.

        workers.push_back(std::move(th)); // Use move semantics.

        assert(!th.joinable());
    }

    std::cout << "Hi from main!\n";

    // Barrier i.e. joining all the threads to the main thread. 
    std::for_each(workers.begin(), workers.end(), [](std::thread& th) // Lambda taking a parameter (the element stored by the vector).
    { 
        assert(th.joinable());
        th.join();
    });

    return 0;
}
