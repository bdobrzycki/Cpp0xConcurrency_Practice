// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread01.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <thread>

// Thread function.
void threadFun()
{
  std::cout << "Hello from thread!\n";
}

int main (int argc, char **argv)
{
  // This is worker thread that will execute function.
  // Fork-join parallelism - this is the fork - execution splits to 
  // main and worker thread.

  std::thread th(&threadFun); //< Create thread and pass the thread function.

  std::cout << "Hello World!\n";

  th.join(); // Join worker thread to main thread. 

  return 0;
}
