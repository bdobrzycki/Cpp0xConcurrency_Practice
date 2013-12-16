// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread02.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <thread>

int main (int argc, char **argv)
{
  // Lambda - (from lambda calculus): define a function on the spot
  // (inline anonymous function [] - read this symbol as a Greek letter lambda)

  // Fork-join parallelism
  std::thread th([]()
  {
      std::cout << "Hello from thread!\n";
  });  //< this symbol is characteristic for Lambda (this is the end of Lambda)

  std::cout << "Hello World!\n";

  th.join(); // Join the worker thread to the main thread. 

  return 0;
}
