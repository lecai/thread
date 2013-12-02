//
// Created by lecai on 13-12-2 下午12:18.
//


#include "epollpool.h"
#include "thread.h"
class ListenerThread:public thread::Thread
{
    
    
private:
    int handle;
    int epfd; //socket 文件描述符
    EpollThreadConnectionPoll *poll;
};