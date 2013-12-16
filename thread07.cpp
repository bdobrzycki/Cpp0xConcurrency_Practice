// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread07.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <thread>
#include <algorithm> // for_each()
#include <cassert>

// Sharing data between threads.
// Data race between threads example.

// Single linked list.
struct List
{
    struct Node
    {
        int _x;
        Node * _next;
        
        explicit Node(int x)
          :_x(x), _next(nullptr) // C++0x new feature.
        {}
    };
    
    // List.
    Node * _head;
    
    explicit List()
      :_head(nullptr)
    {}
    
    void insert(int x)
    {
        auto node = new Node(x);
        node->_next = _head;
        _head = node;
    } 

    int count() const
    {
      int n = 0;
      auto curr = _head;
      while (curr != nullptr)
      {
        ++n;
        curr = curr->_next;
      }
      return n;
    }
};

// Thread function that takes the List by the reference
// and inserts a hundred integers into it. 
void thFun(List & list) 
{
    for (int i = 0; i < 100; ++i)
    {
        list.insert(i);
    }
}

int main (int argc, char **argv)
{
    List list; // 1. Create a list - list is a local variable here.

    // Fork-join parallelism
    std::vector<std::thread> workers; // Vector of worker threads.

    // Start threads.  
    for (int i = 0; i < 10; ++i)
    {   
        workers.push_back(std::thread(&thFun, std::ref(list))); // 2. List passed by the reference.
    }

    // Barrier i.e. joining all the threads to the main thread. 
    std::for_each(workers.begin(), workers.end(), [](std::thread& th) // Lambda is taking a parameter (the element stored by the vector).
    { 
        th.join();
    });

    // List is a local variable here.
    // Since join() is done within the scope of the list variable,
    // all the threads will finish before we exit the scope so the reference to the list will be valid all this time.
    // However all the threads share the reference to the same list. 

    // Make sure all the elements are on the list i.e. count elements.
    // Every thread adds 100 elements to the list; there are 10 threads therefore there should be 1000 elements
    // at all times.
    int total = list.count();
    std::cout << "Total elements: " << total << std::endl;

    // NOTE that the number of elements on the list is changing almost on every consecutive execution!
    // This is typical of data races between threads.
    // Threads were racing and created the corruption of the shared data structure. 

    return 0;
}
