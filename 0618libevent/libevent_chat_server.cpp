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
 *
 * 聊天服务器:
 * 建立一个数据包结构体，以便建立连接时传输数据
 * 用map保存结构体信息,ip作为key查找，消息从ip传入传出
 ***************************************/
#include<iostream>
using namespace std;

#include <event2/event.h>
#include<event2/listener.h>
#include<event2/bufferevent.h>
#include<string.h>
#include<string>

#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<map>
#include<iterator>

/* 业务整理：
 * 建立 libevent 通信框架--main--listener回调函数--bev回调函数
 * 定义Channel结构体，用来保存客户端各自的通道数据
 * 定义map，方便通过ip快速查找Channel对象
 *
 * 当新的客户端连接过来时，保存他的ip，创建专属Channel对象，放入map，
 * 并将这个新用户和老用户互相发送数据，增加ip地址
 *      以服务器为中介，利用各自的socket管道，先传ip长度，再传ip地址
 *
 * 当客户端传送聊天数据过来时，触发读的回调函数
 * 先读4字节的数据头
 * 读完数据头再读数据内容
 * 如果数据读完整了（这里的buf就是发送方的数据），处理数据：
 *      strtok_r提取接收ip和内容
 *      判断接收方ip是否存在在map中，如果不在，说明下线了，需要通知发送方客户端
 *              整合发送方ip和下线提示为一个字符串
 *              利用发送方的bev事件触发内容传送
 *              先发4字节字符串长度，再发字符串
 *      如果接收方ip在map中
 *              利用接收方的bev事件触发socket传送内容
 *              ＊重新将接收方的ip和发送方的聊天内容整合为字符串
 *              ＊先发4字节字符串长度，再发字符串
 *              ＊＊以上2行优化：在strtok_r前，拷贝一份buf数据，那么这里就传这个buf而无需重新整合
 *
 * 当客户端断开连接时
 *      判断错误码，如果是断开连接，需要清除map中对应的数据
 */
/*数据包结构体*/
class Channel
{
public:
    bufferevent* bev;   /* 每个socket对应一个bufferevent */
    char buf[4096];
    int readSize;
    int packetSize;
    string client_ip;

    Channel()
    {
        readSize = 0;
    }
};

map<string, Channel*> chmap;

//读取数据头
void readHeader(struct bufferevent * bev, Channel * ch)
{
    ch->readSize += bufferevent_read(bev, ch->buf + ch->readSize, 4 - ch->readSize);
    if(ch->readSize == 4)   //数据头读完了
    {
        //获得有效数据的大小
        ch->packetSize = ntohl(*(uint32_t*)(ch->buf));
    }
    else
    {
        //数据头没有读完，返回上一级，使bev重置，等待他重新对集合发出通知再读取
    }
}

//读取数据体
void readData(struct bufferevent * bev, Channel * ch)
{
    ch->readSize += bufferevent_read(bev, ch->buf + ch->readSize,
                                     ch->packetSize + 4 - ch->readSize);
}

void dataTransform(struct bufferevent * bev,
                   char* content, string ip)
{
    char buf[4096];
    sprintf(buf, "%s|%s", ip.c_str(), content);
    uint32_t len = strlen(buf);
    len = htonl(len);
    bufferevent_write(bev, (char*)&len, 4);
    bufferevent_write(bev, buf, strlen(buf));
}

void handleData(Channel * ch)
{
    //处理数据	长度192.168.x.x|文本
    ch->buf[ch->readSize] = 0;
    char * buf = ch->buf + 4;

    char * ptr;
    char * c_ip = strtok_r(buf, "|", &ptr); //另一个客户端（接收数据）的ip
    char * content = strtok_r(NULL, "\0", &ptr);//数据内容

    //判断另一个客户端在线
    Channel * peer = chmap[c_ip];
    if(peer == NULL)
    {
        //发消息给 发数据的客户端，通知他对方已下线
        char info[1024];
        sprintf(info, "系统提示：%s不在线", c_ip);
        dataTransform(ch->bev, info, c_ip);
    }
    else
    {
        //转发消息
        dataTransform(peer->bev, content, ch->client_ip);
    }

    ch->readSize = 0;
    //ch->packetSize = 0;
    //memset(ch->buf, 0, sizeof(ch->buf));
}

// read socketdata
//某个客户端发数据过来了，需要接受里面的数据，转发给另一个客户端
void readcb(struct bufferevent *bev, void *ctx)
{
    //printf("readcb\n");
    Channel * ch = (Channel *)ctx;
    //将通道中的数据读出来保存到数据结构体中
    if(ch->readSize < 4)
    {
        readHeader(bev, ch);
        if(ch->readSize == 4)//检查上一个函数是否把数据头读完了
        {
            //读数据体
            readData(bev, ch);
        }
        else
        {
            //进入下一个判断，把bev重置，等待他重新对集合发出通知再读取
        }
    }
    else
    {
        //读数据体
        readData(bev, ch);
    }

    //判断数据是否全部读完
    //否则将bev重置，等待他重新对集合发出通知再读取
    if(ch->readSize == ch->packetSize + 4)
    {
        handleData(ch);
    }
    else
    {

    }
}

// write socket
void writecb(struct bufferevent *bev, void *ctx)
{
}

//说明有错误发生，需要close socket
void eventcb(struct bufferevent *bev, short what, void *ctx)
{
    Channel * ch = (Channel *) ctx;
    if (what & BEV_EVENT_CONNECTED)
    {
        cout << "connect操作完成" << endl;
    }
    else
    {
        if(what & BEV_EVENT_EOF)
            cout << "客户端："<< ch->client_ip <<" 关闭了socket" << endl;
        bufferevent_free(bev);
        chmap.erase(ch->client_ip);
    }
}

//新用户上线通知工作
void newUserOnline(Channel * ch)
{
    //遍历map，把老用户ip发给新用户，把新用户ip发给老用户
    map<string, Channel*>::iterator it;
    for(it = chmap.begin(); it != chmap.end(); ++it)
    {
        Channel * olduser = it->second;

        //互相发送ip------长度ip
        const char * olduserip = olduser->client_ip.c_str();
        const char * newuserip = ch->client_ip.c_str();
        uint32_t len  = 0;

        //printf("老用户%s\n新用户%s\n", olduserip, newuserip);

        len = strlen(olduserip);//老用户ip长度
        len = htonl(len);
        bufferevent_write(ch->bev, (char*)&len, 4);//ip长度发给新用户
        bufferevent_write(ch->bev, olduserip, strlen(olduserip));//ip发给新用户

        len = strlen(newuserip);//新用户ip长度
        len = htonl(len);
        bufferevent_write(olduser->bev, (char*)&len, 4);//新用户ip长度发给老用户
        bufferevent_write(olduser->bev, newuserip, strlen(newuserip));//新用户ip发给老用户
    }
}

// listen_bind的回调函数
void listen_cb(struct evconnlistener* listener, /*监听器*/
               evutil_socket_t newfd,   /*连接客户端的socket, accept返回的socket*/
               struct sockaddr* addr,  /*客户端地址*/
               int socklen,                     /*客户端地址长度*/
               void* ptr)
{
    // ===========取出客户端的ip地址
    struct sockaddr_in * client_addr = (struct sockaddr_in *)addr;
    // ===========将整型ip地址转成字符串格式的ip地址
    string cli_addr = inet_ntoa(client_addr->sin_addr);
    printf("新连接的客户端ip是%s\n", cli_addr.c_str());

    // 获取base集合
    struct event_base* base = (struct event_base*)ptr;
    //struct event_base* base = evconnlistener_get_base(listener);//方法2

    // 用newfd创建一个bev
    // 把bev放入到base集合中
    struct bufferevent* bev = bufferevent_socket_new(base,
                                                     newfd, BEV_OPT_CLOSE_ON_FREE);

    Channel * ch = new Channel;
    // 设置bev事件的回调函数
    bufferevent_setcb(bev, readcb, NULL, eventcb, ch/*回调函数参数*/);

    // 进入未决状态,让bev去监听指定的消息
    // 如果有消息，会回调readcb, writecb, eventcb
    bufferevent_enable(bev, EV_READ|EV_WRITE);//可能默认是EV_PERSIST的

    //============当新用户连接时，首先通知全体在线成员这个新用户的ip
    //============同时把当前所有在线用户给这个新用户发一遍
    //============为新用户创建一个Channel结构体存数据
    ch->client_ip = cli_addr;
    ch->bev = bev;

    /*
    //=====测试数据
    const char * login = "登录成功";
    uint32_t len = strlen(login);
    len = htonl(len);
    bufferevent_write(bev, (char*)&len, 4);
    bufferevent_write(bev, login, strlen(login));
    */

    newUserOnline(ch);

    chmap[cli_addr] = ch;
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
                listen_cb,    // 回调函数，当发现有客户端来连接时，回调这个函数
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

