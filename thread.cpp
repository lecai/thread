//
// Created by lecai on 13-11-20.
//


#include "thread.h"

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
}