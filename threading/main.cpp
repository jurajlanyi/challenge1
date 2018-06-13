#include <iostream>
#include <mutex>
#include <thread>
#include <assert.h>

#include <unistd.h>

using namespace std;

#include <lockguard.h>

mutex g_general_mutex;

// Obtaining the lock via lock_guard
void backgroud_thread_entry(void)
{
    //sleep(4); // Uncomment, if main should get the lock first

    mod_locking::lock_guard<mutex> lock(g_general_mutex);
    cout << "thread2: Lock obtained." << endl;
    sleep(5);
    cout << "thread2: sleep(5) is over. releasing lock" << endl;
}

int main()
{
    cout << "Threading example" << endl;
    thread backgroud_thread_1(&backgroud_thread_entry);

    sleep(2); // sleep, so background thread gets the lock 1st (usually)

    {
        mod_locking::lock_guard<mutex> lock(g_general_mutex);
        cout << "main: Lock obtained." << endl;

        // Check, whether mutex is locked
        assert(false == g_general_mutex.try_lock());

        // Do something useful
        sleep(3);
        cout << "main: sleep(2) is over. releasing lock" << endl;

        //g_general_mutex.lock(); // LANJ: would block forever for "normal" mutex
        //cout << "Lock obtained 2nd time" << endl;
    }

    backgroud_thread_1.join();
    return 0;
}
