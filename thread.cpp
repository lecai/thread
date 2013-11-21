//
// Created by lecai on 13-11-20.
//


#include "thread.h"
#include "mythread.h"

namespace thread{
    //linux 和 mac 版本
    Mutex::Mutex() {
        pthread_mutex_init(&m_hMutex, NULL);
    }

    Mutex::~Mutex() {
        pthread_mutex_destroy(&m_hMutex);
    }

    inline void Mutex::lock() {
        pthread_mutex_lock(&m_hMutex);
    }

    inline void Mutex::unlock() {
        pthread_mutex_unlock(&m_hMutex);
    }

    /*
     * RWLock
     */
    RWLock::RWLock() :rd_count(0),wr_count(0){
        pthread_rwlock_init(&m_hMutex, NULL);
    }

    RWLock::~RWLock() {
        pthread_rwlock_destroy(&m_hMutex);
    }

    inline void RWLock::rdlock() {
        pthread_rwlock_rdlock(&m_hMutex);
        ++rd_count;
    }

    inline void RWLock::wrlock() {
        pthread_rwlock_wrlock(&m_hMutex);
        ++wr_count;
        ++rd_count;
    }

    inline void RWLock::unlock() {
        pthread_rwlock_destroy(&m_hMutex);
        --rd_count;
    }

    /*
     *  Cond
     */
    Cond::Cond() {
          ::pthread_cond_init(&m_hEvent, NULL);
    }

    Cond::~Cond() {
        ::pthread_cond_destroy(&m_hEvent);
    }

    void Cond::broadcast() {
        ::pthread_cond_broadcast(&m_hEvent);
    }

    void Cond::signal() {
        ::pthread_cond_signal(&m_hEvent);
    }

    void Cond::wait(Mutex &m) {
        ::pthread_cond_wait(&m_hEvent, &m.m_hMutex);
    }
/******************************操作包裹类**********************************************************/
    /*
     * Mutex_scope_lock
     */
    Mutex_scope_lock::Mutex_scope_lock(Mutex &m):m_Mutex(m) {
        m_Mutex.lock();
    }

    Mutex_scope_lock::~Mutex_scope_lock(Mutex &m) {
        m_Mutex.unlock();
    }

    /*
     * RWLock_scope_rdlock
     */
    RWLock_scope_rdlock::RWLock_scope_rdlock(RWLock &m) :m_RwLock(m){
        m_RwLock.rdlock();
    }

    RWLock_scope_rdlock::~RWLock_scope_rdlock(RWLock &m) {
        m_RwLock.unlock();
    }

    /*
     * RWLock_scope_wrlock
     */
    RWLock_scope_wrlock::RWLock_scope_wrlock(RWLock &m):m_RwLock(m) {
        m_RwLock.wrlock();
    }

    RWLock_scope_wrlock::~RWLock_scope_wrlock(RWLock &m) {
        m_RwLock.unlock();
    }

///////////////////////////////////////////////////////////////////////
Thread::Thread(const std::string &name, bool joinable) :threadName(name),alive(false),complete(false),joinable(joinable){
        m_hThread = 0;
}

Thread::~Thread() {

}

void Thread::sleep(const long sec) {
    ::sleep(sec * 1000);
}

void Thread::msleep(const long msec) {
    ::sleep(msec);
}

void Thread::usleep(const long usec) {
    ::sleep(usec/1000);
}

/*
 * 检查线程是否joinable
 * @return joinable
 */
const bool Thread::isJoinable() const {
    return joinable;
}

/*
 * 检查线程是否运行
 * @return alive
 */
const bool Thread::isAlive() const {
        return alive;
}

/*
 * 主动结束线程
 * 只是设置标志 当线程run 主回调循环检查这个标志,如果这标志已经设置 则退出线程
 */
void Thread::final() {
    complete = true;
}

const bool Thread::isFinal() const {
    return complete;
}

const std::string& Thread::getThreadName() const {
    return threadName;
}

void* Thread::threadFunc(void *arg) {
    Thread *thread = (Thread*)arg;

    thread->m_Lock.lock();
    thread->alive = true;
    thread->cond.broadcast();
    thread->m_Lock.unlock();


}




}