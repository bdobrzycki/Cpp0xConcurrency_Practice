// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x dwaWatki.cpp -pthread -o a.out -Wl,--no-as-needed
// Run: ./a.out

#include <iostream>
#include <thread>
#include <vector>
#include <algorithm>
#include <chrono> // time utilities

#define USE_MULTITHREADING

namespace MathTools
{
  // Define constant values.
  const double PI = 3.14159265358979323846264338;
}

const unsigned int maxIter = 10000000;

void threadFun(const std::vector<double> & data, std::vector<double> & results)
{
  for_each (data.begin(), data.end(), [&data, &results](double xVal)
  {
      results.push_back(sin(xVal));
      
      // Print progress.	  
      if (results.size() % (maxIter / 10) == 0)
      {
#ifdef USE_MULTITHREADING
	std::cout << "Worker thread [";
#else
	std::cout << "Main thread   [";
#endif
	for (int i = 0; i < (results.size() / static_cast<double>(data.size()) * 10); ++i)      
	{
	  std::cout.put('*');
	}
	for (int i = 0; i < (10 - results.size() / static_cast<double>(data.size()) * 10); ++i)      
	{
	  std::cout.put(' ');
	}
	std::cout.put(']');
	std::cout << "\n";
      }	      
  });
}

int main (int argc, char **argv)
{
	
#ifdef USE_MULTITHREADING
  const unsigned long numOfHardwareThreads = std::thread::hardware_concurrency();
  std::cout << "Hardware threads: " << numOfHardwareThreads << std::endl;
#endif
	
  unsigned int i;

  std::vector<double> data;
  std::vector<double> results1;
  std::vector<double> results2;

  // Generate data.
  for (i = 0; i < maxIter; ++i)
  {
    data.push_back(i * MathTools::PI);
  }
  
  // Get the current time.
  auto startTime = std::chrono::system_clock::now();

  //////////////////////////////////////////
  // Code for profiling.
  
#ifdef USE_MULTITHREADING
  // Start a single worker thread.
  std::thread worker(&threadFun, std::cref(data), std::ref(results1));
#else  
  threadFun(data, results1);
#endif
  
  // Continue work in the main thread.
  for_each (data.begin(), data.end(), [&data, &results2](double xVal)
  {
      results2.push_back(sin(xVal));

      // Print progress.	  
      if (results2.size() % (maxIter / 10) == 0)
      {
        std::cout << "Main thread   [";
        for (int i = 0; i < (results2.size() / static_cast<double>(data.size()) * 10); ++i)      
        {
            std::cout.put('x');
        }
        for (int i = 0; i < (10 - results2.size() / static_cast<double>(data.size()) * 10); ++i)      
        {
            std::cout.put(' ');
        }
        std::cout.put(']');
        std::cout << "\n";
      }
  });
  
#ifdef USE_MULTITHREADING
  worker.join();
#endif
  //////////////////////////////////////////
 
  // Get the current time.
  auto endTime = std::chrono::system_clock::now();

  auto duration = endTime - startTime;

  // Change the duration into time in miliseconds.
  auto durationMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration);

  // Print the number of counts.
  std::cout << "Duration (ms): " << durationMs.count() << std::endl;

  return 0;
}
