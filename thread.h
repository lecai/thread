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

#define MY_HANDLE pthread_t
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
    ~Mutex_scope_lock();

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
    ~RWLock_scope_rdlock();

private:
    RWLock &m_RwLock; //读写锁引用

};

class RWLock_scope_wrlock{
public:
    /*
     * 构造函数
     * 对读写锁进行 wrlock 操作
     * @param m 读写锁引用
     */
    RWLock_scope_wrlock(RWLock &m);

    /*
     * 析构函数
     * 对读写锁进行 unlock 操作
     */
    ~RWLock_scope_wrlock();
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

    /*
     * 等待特定的条件变量满足
     * @param m 需要等待的互斥体
     */
    void wait(Mutex &m);

private:
    MY_COND_HANDLE m_hEvent; //系统条件变量
};

class Thread{
public:
    /*
     * 构造函数 创建一个对象
     * @param name 线程名称
     * @param joinable 标明此线程退出时 是否保存状态 true 保存 false 不保存
     */
    Thread(const std::string &name = std::string("lThread"), bool joinable = true);

    /*
     * 析构函数 用于销毁一个对象 获取释放的空间
     */
    virtual ~Thread();

    /*
     * 让线程睡眠指定的时间 秒
     * @param sec 指定时间 秒
     */
    static void sleep(const long sec);

    /*
     * 让线程睡眠指定的时间 毫秒
     * @param msec 指定时间 毫秒
     */
    static void msleep(const long msec);

    /*
     * 让线程睡眠指定的时间 微秒
     * @param usec 指定时间 微秒
     */
    static void usleep(const long usec);

    /*
     * 是否保存状态
     * @return 是否保存
     */
    const bool isJoinable() const ;

    /*
     * 检查线程是否在运行状态
     * @return 是否在运行
     */
    const bool isAlive() const ;

    /*

     */
    static void *threadFunc(void *arg);

    bool start();
    void join();

    /*
     * 主动结束线程
     * 设置标志 , 当线程的run 主回调循环的时候遇到这个标志被设置的时候 退出循环
     */
    void final();

    /*
     * 判断线程是否继续执行下去
     * @return 标志
     */
    const bool isFinal() const ;

    /*
     * 纯虚函数 线程主回调函数
     * 如果是无限循环 则在每个循环中 都要检查isFinal 这样保证线程安全退出
     */
    virtual void run() = 0;

    /*
     * 返回线程名称
     */
    const std::string &getThreadName() const ;
public:
    std::string threadName; //线程名称
    Mutex m_Lock;//互斥锁
    volatile bool alive;//线程是否在进行
    volatile bool complete;
    MY_HANDLE m_hThread;//线程编号
    bool joinable;//线程标志
    Cond m_Cond;
};

class ThreadGroup{
public:
    struct stCallback{
        virtual void *exec(Thread *e) = 0;
        virtual ~stCallback();
    };

    typedef std::vector<Thread*> Container ; //容器类型

    ThreadGroup();
    ~ThreadGroup();
    void add(Thread *thread);
    Thread *getByIndex(Container::size_type index);
    Thread *operator[] (Container::size_type index);
    void joinAll();
    void execAll(stCallback &cb);

    const Container::size_type size()
    {
         RWLock_scope_rdlock scope_rdlock(m_hRWLock);
        return m_Cts.size();
    }

private:
    Container m_Cts;
    RWLock m_hRWLock;
};
}
#endif //__thread_H_
