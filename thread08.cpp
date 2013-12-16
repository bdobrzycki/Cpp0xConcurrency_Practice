// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread08.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <thread>
#include <algorithm> // for_each()
#include <list>

// Sharing data between threads.
// Safe data sharing between two threads example.

// Thread function that takes the standard list by the reference
// and modifies its elements.
void toSin(std::list<double> & list) // Passed by the reference.
{// II synchronisation point (the start of thread function)
    for_each(list.begin(), list.end(), [](double & x) // Lambda function.
    {
       x = sin(x);  // Replaces x with sin(x).
    });
}// III synchronisation point (the end of thread function)

int main (int argc, char **argv)
{
    std::list<double> list; // Create a standard list.
    
    const double pi = 3.141592;
    const double epsilon = 0.0000001;

    // 1. Main thread firstly accesses the data structure.
    // Push back 16 doubles <0, 2pi>.
    for (double x = 0; x < 2 * pi + epsilon; x += pi / 16)
    {
        list.push_back(x);
    }
    
    // 2. Main thread creates the worker thread.
    // I synchronisation point (thread creation)
    std::thread th(&toSin, std::ref(list));
    
    // 3. From this point on the main thread does not access shared data structure (self imposed ban).

    // 4. Main thread calls join() and blocks.
    // IV synchronisation point (the exit from thread join())
    th.join();

    // 5. When thread join() returns, the main thread is now free to access the data structure again
    // (in the meanwhile the worker thread can freely access the data structure and terminates triggering
    // the return from the thread join()).

    // 6. Main thread takes turn and safely accesses the list (after the worker thread joined).
    for_each(list.begin(), list.end(), [](double & x) // Lambda function.
    {
       int count = static_cast<int>(10 * x + 10.5);
       for (int i = 0; i < count; ++i)
           std::cout.put('*');
       std::cout << std::endl;
    });

    // There is no real concurrency here but this simple program demonstrates deep principles involved
    // in shared memory concurrency.

    // This pattern of taking turns forms the basics of sheared memory concurrency.

    // Taking turns means that threads have to communicate somehow.

    // These points of communication are called synchronisation points.

    // There are four synchronisation points in this program:
    //   I.   thread creation
    //   II   the start of thread function
    //   III. the end of thread function
    //   IV.  the exit from thread join()

    // "happens before" relationships:
    // I always happens before II
    // III always happens before IV

    // This is how this intuition of taking turns can be described in more rigorous language
    // of synchronisation points and "happens before" relationships.

    return 0;
}
