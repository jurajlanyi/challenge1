#include <iostream>
#include <mutex>
#include <thread>
#include <assert.h>

#include <unistd.h>

using namespace std;

#include <lockguard.h>

mutex g_general_mutex;

// Obtaining the lock directly, without guard
/*
void backgroud_thread_entry(void)
{
    g_general_mutex.lock();
    cout << "thread2: Lock obtained by backgroud thread." << endl;
    sleep(5);
    cout << "thread2: sleep(5) is over." << endl;
    g_general_mutex.unlock();
}
*/

void backgroud_thread_entry(void)
{
    mod_locking::lock_guard<mutex> lock(g_general_mutex);
    cout << "thread2: Lock obtained by backgroud thread." << endl;
    sleep(5);
    cout << "thread2: sleep(5) is over." << endl;
}

int main()
{
    cout << "Threading example" << endl;
    thread backgroud_thread_1(&backgroud_thread_entry);

    sleep(2); // sleep, so background thread gets the lock 1st (usually)

    {
        mod_locking::lock_guard<mutex> lock(g_general_mutex);
        cout << "main: Lock obtained." << endl;
        // Do something useful

        assert(false == g_general_mutex.try_lock());

        //g_general_mutex.lock(); // LANJ: would block
        //cout << "Lock obtained 2nd time" << endl;
    }

    backgroud_thread_1.join();
    return 0;
}
