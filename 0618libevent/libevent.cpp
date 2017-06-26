/*************************************************************************
    > File Name: libevent.cpp
    > Author: C01
    > Mail: 1003192057@qq.com
    > Created Time: 2017年06月18日 星期日 14时44分24秒
 ************************************************************************/

/****************************************
 * 创建event_base事件集合的对象
 * 初始化服务器地址
 * 创建一个监听器，集合绑定地址和监听器
 * 监听器未决状态
 * 开始监视集合中的socket动静
 * 有动静，调用监听器中的回调函数处理
 ***************************************/
#include<iostream>
using namespace std;

#include <event2/event.h>
#include<event2/listener.h>
#include<event2/bufferevent.h>
#include<string.h>

// read socket
void readcb(struct bufferevent *bev, void *ctx)
{
    char buf[1024];
    memset(buf, 0, sizeof(buf));
    // 从bufferevent中抽取数据
    bufferevent_read(bev, buf, sizeof(buf));

    cout << buf << endl;
}

// write socket
void writecb(struct bufferevent *bev, void *ctx)
{

}

//说明有错误发生，需要close socket
void eventcb(struct bufferevent *bev, short what, void *ctx)
{
    if (what & BEV_EVENT_CONNECTED)
    {
        cout << "connect操作完成" << endl;
    }
    else
    {
    /* 其他错误
     * #define BEV_EVENT_READING	0x01	< error encountered while reading
     * #define BEV_EVENT_WRITING  0x02	< error encountered while writing
     * #define BEV_EVENT_EOF           0x10	< eof file reached
     * #define BEV_EVENT_ERROR		 0x20	< unrecoverable error encountered
     * #define BEV_EVENT_TIMEOUT	 0x40	< user-specified timeout reached
     * #define BEV_EVENT_CONNECTED	0x80	< connect operation finished. 表示connect操作完成
     */
        cout << "对面的关闭了socket" << endl;
        bufferevent_free(bev);
    }
}

// listen_bind的回调函数
void listen_cb(struct evconnlistener* listener/*监听器*/,
               evutil_socket_t newfd/*连接客户端的socket, accept返回的socket*/,
               struct sockaddr* addr/*客户端地址*/,
               int socklen/*客户端地址长度*/,
               void* ptr)
{
    // 获取base集合
    struct event_base* base = (struct event_base*)ptr;
    //struct event_base* base = evconnlistener_get_base(listener);//方法2

    // 用newfd创建一个bev
    // 把bev放入到base集合中
    struct bufferevent* bev = bufferevent_socket_new(base, newfd, BEV_OPT_CLOSE_ON_FREE);

    // 设置bev事件的回调函数
    bufferevent_setcb(bev, readcb, writecb, eventcb, base/*回调函数参数*/);

    // 进入未决状态,让bev去监听指定的消息
    // 如果有消息，会回调readcb, writecb, eventcb
    bufferevent_enable(bev, EV_READ|EV_WRITE|EV_PERSIST);//可能默认是EV_PERSIST的
}

int main()
{
    // 类似创建一个epoll或者select对象，默认下linux下应该epoll
    // event_base是一个事件的集合
    // 在libevent中，事件指一件即将要发生事情，evbase就是用来监控这些事件的
    // 发生（激活）的条件的
    struct event_base * base = event_base_new();

    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = INADDR_ANY;

    // 创建一个监听器--服务器socket，并加入到base集合中，该监听器是初始化状态，当有客户端连接时，回调listen_cb函数
    // base绑定addr地址并监听
    //evconnlistener_cb cb = listen_cb;   // 回调函数
    struct evconnlistener* listener = evconnlistener_new_bind(
                base,           // 绑定和监听的事件集合的对象
                listen_cb,               // 回调函数，当发现有客户端来连接时，回调这个函数
                base,           // 回调函数的最后一个参数
                LEV_OPT_REUSEABLE | LEV_OPT_CLOSE_ON_FREE,  // reuseable:TIME_WAIE失效，异常退出端口可立即复用      closeonfree：listener被释放时清理sockets
                250,            // 最大同时监听的socket的数量
                (struct sockaddr*)&addr,    //  地址
                sizeof(addr));
    // 创建完毕后，base集合中会有listener

    // 进入未决状态,使listener被启动, 只有pending状态的event，才可能被激活
    evconnlistener_enable(listener);

    // base监听它的所有未决状态的事件,进入死循环,类似epoll_wait,如果event条件ready，那么这个event就变成激活状态，去调回调函数，listener只有一个回调函数，普通socket生成的bev有三个回调函数
    event_base_dispatch(base);

    // 从上一个函数出来后，说明程序快要结束了，这里用来释放内存空间
    event_base_free(base);

    return 0;
}

/*
111行：未决的作用是什么？
当监听器或事件器发现socket有数据时，如果没有进入未决状态，不会主动向event_base报告说来数据了，必须先进入未决状态才行
63行： newfd是怎么产生的，谁传过来的参数？
event_base_dispatch()函数调用accept()函数产生了newfd，并在调用回调函数listener_cb时将其传入
74行： 有了newfd为什么还要产生bev，bev是服务器端的socket吗？和newfd的区别？
监听了bev，newfd也被监听，那结果是只要newfd有消息，bev也会有，bev不是多余的吗
newfd属于事件集合，bev属于事件处理集合，共生关系
*/


