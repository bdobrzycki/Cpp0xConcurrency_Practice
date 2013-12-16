// Compiled using GCC compiler (4.8.1) under Linux.
// g++ -std=c++0x thread12.cpp -pthread -o a.out
// Run: ./a.out

#include <iostream>
#include <string>
#include <thread>
#include <future> // promise, future 

// Passing and returning values from threads.

// The string in the following example is passed by move 
// even though the reference is used.

// 3. Thread function has to expect the promise (passed by reference).
void thFun(std::promise<std::string> & prms) 
{
    std::string str("Hello from future!");
   
    // Print the raw pointer to the data.
    // String contains a buffer and there is a raw pointer to this buffer
    // that can be retrieved by calling string data();
    // data() returns a pointer to the character;
    //(cast to void* so that cout can print it as a raw pointer).
    std::cout << "String data: " << (void *)str.data() << std::endl; 
    
    // Set the value.
    prms.set_value(str);
}

int main (int argc, char **argv)
{
    // 1. Create a promise parametrised by the type I would like to pass to the worker thread.
    std::promise<std::string> prms;

    // 2. Pass the promise to the worker thread function by the reference.
    std::thread th(&thFun, std::ref(prms));
    // Since promise is passed by reference the promise is not invalidated.
    // We can call get_future right before we use it (point 4). 

    //---------------------------------------------------------------------------------
    // This is the work that we are doing in parallel with the work that is done by the
    // worker thread function.
    std::cout << "Hello from main!\n";
    //
    // do more stuff here...
    //
    //---------------------------------------------------------------------------------

    // 4. Extract the future from the promise.
    // The future is also parametrised by the type I would like to get from the worker thread.
    std::future<std::string> ftr = prms.get_future();

    // 5. Retrieve the value from the worker thread.
    std::string str = ftr.get(); //< future is the receiving end of the channel
    // If the thread function has finished at this point get will immediately return the string.
    // If the thread function hasn't finished then it will block. 

    std::cout << str << std::endl;

    // Print a raw pointer to the string buffer.
    std::cout << "String data: " << (void *)str.data() << std::endl; 

    // If this was really a move operation than no copy was made
    // and the raw pointer should be exactly the same.  

    th.join(); // Join the worker thread. Otherwise the program would terminate.

    return 0;
}
