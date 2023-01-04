#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>

using namespace std;

// Global variables
string input;
string reversed;
string capitalized;
string shifted;

// Mutex and condition variable to synchronize threads
mutex m;
condition_variable cv;

// Flag to indicate that input thread has finished
bool input_done = false;

// Function for input thread
void input_thread_func()
{
    // Take input from user
    cout << "Enter a string: ";
    getline(cin, input);

    // Notify other threads that input is done
    {
        lock_guard<mutex> lock(m);
        input_done = true;
    }
    cv.notify_all();
}

// Function for reverse thread
void reverse_thread_func()
{
    // Wait until input is done
    unique_lock<mutex> lock(m);
    cv.wait(lock, [] {return input_done; });

    // Reverse the input string
    reversed = input;
    reverse(reversed.begin(), reversed.end());

    // Print reversed string
    cout << "Reversed string: " << reversed << endl;
}

// Function for capitalization thread
void capitalization_thread_func()
{
    // Wait until input is done
    unique_lock<mutex> lock(m);
    cv.wait(lock, [] {return input_done; });

    // Capitalize the input string
    capitalized = input;
    for (auto& c : capitalized)
    {
        c = toupper(c);
    }

    // Print capitalized string
    cout << "Capitalized string: " << capitalized << endl;
}

// Function for shift thread
void shift_thread_func()
{
    // Wait until input is done
    unique_lock<mutex> lock(m);
    cv.wait(lock, [] {return input_done; });

    // Shift the input string
    shifted = input;
    for (auto& c : shifted)
    {
        c = (c - 'a' + 2) % 26 + 'a';
    }

    // Print shifted string
    cout << "Shifted string: " << shifted << endl;
}

int main()
{
    // Create threads
    thread input_thread(input_thread_func);
    thread reverse_thread(reverse_thread_func);
    thread capitalization_thread(capitalization_thread_func);
    thread shift_thread(shift_thread_func);

    // Wait for input thread to finish
    input_thread.join();

    // Wait for other threads to finish
    reverse_thread.join();
    capitalization_thread.join();
    shift_thread.join();

    return 0;
}
