// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread10.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <string>
#include <thread>
#include <future> // promise, future  

// Passing and returning values from threads.

// 4. Thread function has to expect the promise.
// Since we use move semantics the promise should be passed be the rvalue reference.
void thFun(std::promise<std::string> && prms) 
{
    std::string str("Hello from future!");
    // Set the value.
    prms.set_value(str);
}

int main (int argc, char **argv)
{
    // 1. Create a promise parametrised by the type I would like to pass to the worker thread.
    std::promise<std::string> prms;

    // 2. Extract the future from this promise.
    // The future is also parametrised by the type I would like to get from the worker thread.
    std::future<std::string> ftr = prms.get_future();

    // 3. Pass the promise to the worker thread function using move semantics.
    std::thread th(&thFun, std::move(prms));

    //---------------------------------------------------------------------------------
    // This is the work that we are doing in parallel with the work that is done by the
    // worker thread function.
    std::cout << "Hello from main!\n";
    //
    // do more stuff here...
    //
    //---------------------------------------------------------------------------------

    // 5. Retrieve the value from the worker thread.
    std::string str = ftr.get(); //< future is the receiving end of the channel
    // If the thread function has finished at this point get will immediately return the string.
    // If the thread function hasn't finished then it will block. 

    std::cout << str << std::endl;

    th.join(); // Join the worker thread. Otherwise the program would terminate.
    // We could join the worker thread before getting the future.
    // In this case get() would return immediately.

    return 0;
}
