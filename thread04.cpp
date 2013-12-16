// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread04.cpp -pthread -o a.out
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
        // A clousure is a lambda function that captures external values.
        workers.push_back(std::thread([i]() // Lambda captures variable i by value
                                            // i.e. copy and store i as a private data member
                                            // inside a function object (lambda de facto is).
        {
            // Capturing makes variable i visible inside the body of the lambda function.
            std::cout << "Hello from thread " << i << "!\n"; // Note that these are three different calls
                                                             // therefore various threads can interleaving each other between calls - run the program to see the effect.
                                                             // Every program execution will be non-deterministic and this is undesired.
        }));
    }

    std::cout << "Hi from main!\n";

    // Barrier i.e. joining all the threads to the main thread. 
    std::for_each(workers.begin(), workers.end(), [](std::thread& th) // Lambda taking a parameter (the element stored by the vector).
    { 
        th.join();
    });

    // Note that this is not deterministic!
    // Every run of the program results with different behaviour.

    return 0;
}
