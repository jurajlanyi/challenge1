#ifndef LOCKGUARD_H
#define LOCKGUARD_H

namespace mod_locking
{
    template <class Mutex_Type> class lock_guard
    {
public:
        lock_guard(Mutex_Type &mutex) : m_mutex(mutex) { m_mutex.lock(); };
        ~lock_guard() { m_mutex.unlock(); };
private:
        Mutex_Type& m_mutex; // LANJ: mutex acquired at construction
    };
}

#endif // LOCKGUARD_H
