//
// Created by lecai on 13-12-2 上午12:06.
//



#ifndef __singleton_H_
#define __singleton_H_

/*
 * 模板单例
 */
template <class T>
class Singleton {
public:
    /*
     * 获取单例引用
     */
    static T &getMe()
    {
        if(!t) t = new T();
        return *t;
    }

    void release()
    {
        if(t)
            delete t;
        t = 0;
    }

    static T *t;

};

/*初始化 t*/
template <class T>
T *Singleton<T>::t = 0;

#define USE_SINGLETON_OBJECT (__CLASS) \
  static __CLASS &getMe() \
  { \
    static __CLASS cls;\
    return cls;\
  }
#endif //__singleton_H_
