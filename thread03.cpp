// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread03.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <thread>
#include <algorithm> // for_each()

int main (int argc, char **argv)
{
    // Fork-join parallelism
    std::vector<std::thread> workers; // Vector of worker threads.
    
    // Start ten threads.  
    for (int i = 0; i < 10; ++i)
    {
        workers.push_back(std::thread([]() // Lambda used here.
        {
            std::cout << "Hello from thread!\n";
        }));
    }

    std::cout << "Hi from main!\n";

    // Barrier i.e. joining all the threads to the main thread. 
    std::for_each(workers.begin(), workers.end(), [](std::thread& th) // Lambda is taking a parameter (the element stored by the vector).
    { 
        th.join();
    });

    // Note that this is not deterministic!
    // Every run of the program results with a different behaviour.

    return 0;
}
