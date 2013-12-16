// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread09.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <thread>
#include <algorithm> // for_each()
#include <list>

// Sharing data between threads.
// Safe data sharing between two threads using the move semantics example and unique data.

// Thread function that takes the standard list using move semantics
// and modifies its elements.
void toSin(std::list<double> && list) // Passed by the rvalue reference to use move semantics.
{
    for_each(list.begin(), list.end(), [](double & x) // Lambda function.
    {
       x = sin(x);  // Replaces x with sin(x).
    });

    for_each(list.begin(), list.end(), [](double & x) // Lambda function.
    {
       int count = static_cast<int>(10 * x + 10.5);
       for (int i = 0; i < count; ++i)
       {
           std::cout.put('*');
       }
       std::cout << std::endl;
    });
}

int main (int argc, char **argv)
{
    std::list<double> list; // Create a standard list.
    
    const double pi = 3.141592;
    const double epsilon = 0.0000001;

    // Main thread firstly accesses the data structure.
    // Push back 16 doubles <0, 2pi>.
    for (double x = 0; x < 2 * pi + epsilon; x += pi / 16)
    {
        list.push_back(x);
    }
    
    // Main thread creates the worker thread.
    std::thread th(&toSin, std::move(list)); // Use the move semantics to pass the list to the thread function.
    // When we move the list the original list becomes empty. 
    
    // The list has been moved from the main thread to the worker thread and the main thread has no longer access to it.

    // Main thread calls join() and blocks.
    th.join();

    // We no longer have a shared data - instead we have got a unique data.
    // Unique data is piece of data that has a single owner at all times. 
    // Unique data may change the owner - this is done through move semantics.

    // 1. Program starts in main thread which creates unique data (list) and accesses it.
    // 2. Main thread creates worker thread - instead of passing the argument by reference it moves the unique data.
    // 3. The same unique data now belongs to the worker thread that can safely access it.
    // 4. At that time the main thread looses access to that data. Even if the programmer tried to access the data
    //    it would not work - list is empty.

    return 0;
}
