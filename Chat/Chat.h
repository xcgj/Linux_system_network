#ifndef CHAT_H
#define CHAT_H

#include <QObject>
#include <QString>
#include <QDebug>
#include <QMap>
#include <errno.h>

//#include <QUdpSocket>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonValue>
#include <QJsonArray>

#include "Chatdef.h"
#include <pthread.h>

#include <QNetworkAddressEntry>
#include <QNetworkInterface>
#include <QApplication>//环境变量
#include <QProcessEnvironment>

#include <stdlib.h>
#include <unistd.h>
#include <QThread>
#include <QFileInfo>
#include <QFile>

class Chat;

static int SendFileInfoId = 0;
typedef struct
{
    QString name;
    QString ip;
}User;

typedef struct SendFileInfo
{
    QString filepath_src;
    QString filepath_dst;
    int id;
    int peerid; //这个文件在对方的map表中，保存在哪个位置
    QString ip;//文件发送的ip
    int total;
    int transSize;
    bool started; //对方同意接收文件，给发送方开始传输的信号
    int port;

    Chat * chat;
    int server_fd; //服务器的fd

    SendFileInfo(Chat * ch)
    {
        server_fd = -1;
        port = -1;
        peerid = -1;
        started = false;
        transSize = 0;
        chat = ch;
        id = SendFileInfoId++;//说操作了全局变量，明了这个类的构造必须在单个线程中进行。让主线程操作
    }

    ~SendFileInfo()
    {
        if(server_fd != -1)
            close(server_fd);
    }
}SendFileInfo;

class Chat : public QObject
{
    Q_OBJECT
public:
    explicit Chat(QObject *parent = 0);

    //账户信息
    QString myaccount;       //存自己的ip 用户名信息
    QStringList ips;                //自己可以有很多ip地址
    QMap<QString, User*> others;//存好友的ip 用户名信息
    //QMap<int, SendFileInfo*> sends;//保存文件发送的结构体的键值对表
    //QMap<int, SendFileInfo*>  recvs;//保存文件接收的结构体的键值对表
    QMap<int, SendFileInfo*> transinfo;
    QString broadcast_ip;   //默认广播ip地址

    int udp_fd;

    //线程
    pthread_t tid;
    static void * recv_thread(void * ptr);//static防止了this指针
    static void * recv_file_thread(void * ptr);//接收文件的线程
    static void * send_file_thread(void * ptr);//发送文件的线程
    void recv_one_file(SendFileInfo * info);//接受文件
    void send_one_file(SendFileInfo * info);//发送文件
    void run();
        //数据处理
    void handleMsg(const QJsonObject &obj, QString ip);
        //当JSON解析出来的数据是 老用户收到上线通知 或者 新用户收到老用户的ONLINEACK回复 时
        //发送信号让主界面更新好友列表
    void addUser(QString ip, QString name);

    //程序初始化
    void init();
    QStringList getSysIps();
    QString getSysName();
    void create_socket(QString ip);//socket绑定的地址要和广播地址的网段一样，修改广播地址也要修改绑定地址

    //发送数据
        //给指定地址发送json报文，通用函数，可重入函数
    void send(const QJsonObject &obj, QString ip);
        //发送聊天内容
    void sendMsg(QString content, QString ip);
        //构建文件结构体保存到map表，并发送 文件发送请求报文---此时是文件发送者
    void sendFile(QString path, QString ip);
        //响应文件发送请求报文后采取的行动---此时是文件接收者
    void ackFileTransRequest(QString filename, int filesize, int peerid, QString ip, bool ok, QString dstfilename = "");

    //上线通知或重新发一次上线通知，这里要清空旧的好友列表，获取新列表
    void sendOnline();

signals:
    //后台消息与槽处理
        //创建线程发送文件
    void sigAckFileTrans(int id, int peerid, int port);

    //通知主界面
        //来新用户了的信号，新用户的名字和ip
    void sigNewUser(QString name, QString ip);
        //消息解析完毕，通知主界面显示内容
    void sigNewContent(QString name, QString content, bool broadcast, QString ip);
        //文件接收请求
    void sigTransFileRequest(QString filename, int filesize, int peerid, QString ip);
        //文件接收进度
    void sigProgress(SendFileInfo * info);
        //文件接收完成，删除info结构体
    void sigTransFinish(SendFileInfo * info);

public slots:
    void onAckFileTrans(int id, int peerid, int port);
};

#endif // CHAT_H
