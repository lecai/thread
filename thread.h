//
// Created by lecai on 13-11-20.
//



#ifndef __thread_H_
#define __thread_H_

#include "platform.h"
#include <list>
#include <vector>
#include <string>
#include <sys/_types/_pthread_cond_t.h>
#include <sys/_types/_pthread_mutex_t.h>
#include <sys/_types/_pthread_rwlock_t.h>
#include <sys/_types/_pthread_t.h>

#define MY_HANDLE pthread_t;  //
#define MY_API
#define MY_MUTEX_HANDLE pthread_mutex_t
#define MY_COND_HANDLE  pthread_cond_t
#define MY_RW_LOCK_HANDLE pthread_rwlock_t

/*
声明类集合
 */
namespace thread{
    class Mutex; //锁对象
    class Mutex_scope_lock; //锁操作类
    class Cond ; //线程系统条件变量
    class RWLock ; //读写锁
    class RWLock_scope_rdlock; //读锁操作类
    class Thread;
};
namespace thread{

class Mutex{
public:
    /*
     * @brief 构造函数 构造一个互斥体对象
     */
    Mutex();

    /*
     * @brief 析构函数
     */
    ~Mutex();

    /*
     * 加锁一个互斥体
     */
    inline void lock();

    /*
     * 解锁一个互斥体
     */
    inline void unlock();

    MY_MUTEX_HANDLE m_hMutex; //互斥体对象
};

    /*
     * 方便复杂函数中锁的使用
     */
class Mutex_scope_lock{
public:
    /*
     * 构造函数
     * 对锁进行lock操作
     * @param m 锁的引用
     */
    Mutex_scope_lock(Mutex &m);

    /*
     * 析构函数
     * 对锁进行unlock操作
     * @param m 锁引用
     */
    ~Mutex_scope_lock(Mutex &m);

    Mutex &m_Mutex; //锁对象
};
    /*
     * 封装系统的读写锁,去除繁杂的手工初始化 和销毁的操作
     */
class RWLock{
public:
    /*
     * 构造函数
     * 创建一个读写锁
     */
    RWLock();

    /*
     * 析构函数
     * 销毁一个读写锁
     */
    ~RWLock();

    /*
     * 对读写锁 读操作 进行加锁操作
     *
     */
    inline void rdlock();

    /*
     * 对读写锁 写操作 进行加锁操作
     */
    inline void wrlock();

    /*
     * 对读写锁 进行解锁操作
     */
    inline void unlock();

private:
    MY_RW_LOCK_HANDLE m_hMutex; //系统读写锁
    unsigned int rd_count;
    unsigned int wr_count;
};

class RWLock_scope_rdlock{
public:
    /*
     * 构造函数
     * 对读写锁进行 rdlock 操作
     * @param m 读写锁引用
     */
    RWLock_scope_rdlock(RWLock &m);

    /*
     * 析构函数
     * 对读写锁进行 unlock 操作
     */
    ~RWLock_scope_rdlock(RWLock &m);

private:
    RWLock &m_RwLock; //读写锁引用

};

/*
 * 封装系统条件变量 省去手工初始化 和 销毁的繁琐工作
 */
class Cond{
public:
    /*
     * 构造函数 用于创建一个系统条件变量
     */
    Cond();

    /*
     * 析构函数 用于销毁一个系统条件变量
     */
    ~Cond();

    /*
     * 对所有等待这个条件变量的线程 广播信号,使这些线程能继续向下执行下去
     */
    void broadcast();

    /*
     * 对所有等待这个条件变量的线程 发送信号,使这些线程继续向下执行下去
     */
    void signal();

private:
    MY_COND_HANDLE m_hEvent; //系统条件变量

};
class thread {

};

}
#endif //__thread_H_
