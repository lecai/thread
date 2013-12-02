//
// Created by lecai on 13-11-22.
// 模仿 netty 实现基于bsd socket的 客户端库 序列化支持protobuf 可以传 文件
//
// 需实现 2进制缓存器 线程安全的消息队列  解码器 (自定义序列化对象)
//



#ifndef __mac_net_H_
#define __mac_net_H_

#include <vector>
#include <string>
#include <list>

namespace net{
    /********************二进制缓存器
     * Buffer 获取信息后 重置当前位置
     */
    class Record{
    public:
        Record(void *cmd, unsigned int len)
        {
            contents = new unsigned char[len];
            memcpy(contents, cmd, len);
            contentSize = len;
            offset = 0;
        }
        Record()
        {
            offset = contentSize = 0;
            contents = NULL;
        }
        ~Record()
        {
            if(contents) delete contents;
            contents = NULL;
        }

        bool empty()
        {
            return offset == contentSize;
        }
        /* 接收
         * @param buffer
         * @param len
         * @return len
         */
        unsigned int recv(void *buffer,unsigned int len)
        {
            if(empty()) return 0;
            len = leftSize() < len ? leftSize() : len;
            memcpy(buffer, contents+offset, len);
            offset+=len;
            return len;
        }
        /*发送完全
         * @param T con
         * @return bool true 发送完全 false 发送失败
         */
        template <class CONNECTION>
        bool sendOver(CONNECTION *con)
        {
            unsigned int leftSize = leftSize();
            if(leftSize == 0) return true;
            int sendLen = con->send(contents + offset,leftSize);
            offset += leftSize;
            if(sendLen < leftSize) return false;
            return true;
        }

        /*
         * 读取后剩下长度
         */
        unsigned int leftSize()
        {
            return contentSize - offset;
        }
    private:
        unsigned int offset;
        unsigned int contentSize;
        unsigned char *contents;
    };

    /*
     * 线程安全的 消息队列
     */
    /*
     * @brief 单向链表
     */
    template <typename CLASS>
    class Node{
    public:
        CLASS node;
        Node<CLASS> *next;
        Node()
        {
            next = NULL;
        }
    };





}
class mac_net {

};


#endif //__mac_net_H_
