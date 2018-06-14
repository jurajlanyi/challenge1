#include <iostream>
#include <mutex>
#include <thread>
#include <assert.h>
#include <condition_variable>

#include <unistd.h>
#include <signal.h>
#include <chrono>
#include <cstdlib>

using namespace std;

// not using mod_locking, as std::condition_variable needs unique::lock
//#include <../threading/lockguard.h>

enum e_task_id {TASK_MAIN, TASK_1, TASK_2, TASK_3, TASK_ANY };  // Thread/ task IDs enum
typedef int task_id;

// START block - protected by g_general_mutex
mutex g_general_mutex;                                      // Variables below should be modified only by mutex owner
condition_variable g_general_condvar;                       // Threads wait on this condvar
task_id work_for_thread = TASK_MAIN;                        // Indicates, which thread should do next job
int threads_ready = 0;                                      // Count of worker threads ready
// END block - protected by g_general_mutex

// LANJ - TODO: consider alternate implementation with dedicated condvar instance for each thread
// This could have significant performance impact, as less threads would be waken up

sig_atomic_t g_b_terminate = 0;                             // Is set to 1 by SIGINT signal handler. All worker threads should finish ASAP

///////////////////////////////////

void sig_int_handler(int p)
{
    g_b_terminate = 1;
}

void worker_thread_main(task_id my_task_id)
{
//    mod_locking::lock_guard<mutex> lock(g_general_mutex);
// LANJ: TODO - mod_locking - not compatible with std:: condition_variable !!!

    unique_lock<mutex> lock(g_general_mutex);

    cout << "worker" << my_task_id << ":Starting, waiting." << endl;
    threads_ready++;
    g_general_condvar.notify_all();

    while (1)
    {   // LANJ: CHANGE: if work_for_thread == my_task_id the thread will not wait on condvar
        while ((!g_b_terminate) && (work_for_thread != my_task_id))
            g_general_condvar.wait(lock);
        if (g_b_terminate)
            break;

        cout << "worker" << my_task_id << ": Signal received, doing work ..." << endl;

        // Doing my work ...
        this_thread::sleep_for(chrono::milliseconds(100*(rand() % 5))); // LANJ: smaller sleep than requested

        work_for_thread++;                                  // This thread should work next
        if (work_for_thread == TASK_ANY)
            work_for_thread = TASK_1;

        cout << "worker" << my_task_id << ": done with work, signal next thread #" << work_for_thread << endl;
        g_general_condvar.notify_all();                     // Unblock all waiting threads
    }

    threads_ready--;
    g_general_condvar.notify_all();                         // Allow other worker threads to unblock and terminate
    cout << "worker" << my_task_id << ": Terminating" << endl;
}

int main()
{
    cout << "Starting threading example 2 with 3 worker threads" << endl;

    signal(SIGINT, sig_int_handler);                        // Signal handler for CTRL-C

    thread background_thread_1(&worker_thread_main, TASK_1);
    thread background_thread_2(&worker_thread_main, TASK_2);
    thread background_thread_3(&worker_thread_main, TASK_3);

    // Wait for all worker threads to be in ready state
    // Otherwise it will miss notify()
    {
        unique_lock<mutex> lock(g_general_mutex);
        while (threads_ready < 3) {
            g_general_condvar.wait(lock);
        }
        cout << "main: all workers ready, starting. Thread 1 is next" << endl;
        work_for_thread = TASK_1;
        g_general_condvar.notify_all();
    }

    int cycle_cnt = 0;
    while(1) {
        if (g_b_terminate) {
            cout << "main: SIGINT termination - start" << endl;
            break;
        }
        sleep(1);
        cycle_cnt++;
        cout << "main: cycle_cnt is " << cycle_cnt << endl;
        if (cycle_cnt >= 60) {                              // After 60 iterations stop worker threads
            g_b_terminate = 1;
            g_general_condvar.notify_all();                 // LANJ: can signal without mutex lock()
            break;
        }
    }

//  Code below is not really needed
#if 0
    // Wait until all worker threads finish
    {
        unique_lock<mutex> lock(g_general_mutex);
        while(1) {
            if (threads_ready == 0)
                break;
            g_general_condvar.wait(lock);
        }
    }
#endif

    cout << "main: joining threads, terminating" << endl;

    background_thread_1.join();
    background_thread_2.join();
    background_thread_3.join();

    return 0;
}
