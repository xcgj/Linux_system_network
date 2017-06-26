#include "Chat.h"

Chat::Chat(QObject *parent) : QObject(parent)
{
    this->ips = getSysIps();
    myaccount = getSysName();
    broadcast_ip = "255.255.255.255";//默认广播地址
    udp_fd = -1;

    //子线程发信号，让主线程处理产生文件发送的子线程
    connect(this, SIGNAL(sigAckFileTrans(int,int,int)), this,SLOT(onAckFileTrans(int,int,int)));
}

void Chat::init()
{
    //创建socket，默认绑定的ip地址为0
    create_socket("0.0.0.0");

    //发送JSON对象
    sendOnline();

    //创建线程
    pthread_create(&tid, NULL, recv_thread, this);
    pthread_detach(tid);
}

//socket绑定的地址要和广播地址的网段一样，
//修改广播地址时，也要调这个函数修改绑定地址
void Chat::create_socket(QString ip)
{
    //关闭旧的socket管道
    if(this->udp_fd != -1)
        close(this->udp_fd);

    //创建udp socket
    this->udp_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(this->udp_fd < 0)
    {
        qDebug() << "创建udp_fd失败";
        exit(1);
    }

    //初始化地址，绑定本机ip
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = inet_addr(ip.toUtf8().data());//转成char*;

    //socket绑定地址
     int ret = bind(udp_fd, (struct sockaddr*)&addr, sizeof(addr));
     if(ret != 0)
     {
         qDebug() << "error bind";
         exit(1);
     }

     //发送广播
     //===设置可以发送广播
     int arg = 1;
     setsockopt(udp_fd, SOL_SOCKET, SO_BROADCAST, &arg, sizeof(arg));
     //===发送上线广播--报文用JSON
}

//获取本机账户名
QString Chat::getSysName()
{
#ifdef WIN32
            return QProcessEnvironment::systemEnvironment().value("USERNAME");
#else
    char  buf[1024];
    memset(buf, 0, sizeof(buf));
    FILE * fp = popen("whoami", "r");//执行whoani命令后，用fp去读标准输出
    int ret = fread(buf, 1, sizeof(buf), fp);
    buf[ret - 1] = 0;
    fclose(fp);
    return QString(buf);
#endif
    return "xcgj";
}

QStringList Chat::getSysIps()
{
    QStringList ret;//存储提取出来的ip
    QList<QNetworkAddressEntry> entrys;//网络地址的结构体项

    QList<QNetworkInterface> infs =QNetworkInterface::allInterfaces();//获取所有接口信息
    foreach (QNetworkInterface inf, infs)
    {
        entrys.append(inf.addressEntries());
    }
    foreach (QNetworkAddressEntry entry, entrys)
    {
        if(entry.ip().toString().length() != 0)
        {
            ret.append(entry.ip().toString());
        }
    }
    return ret;
}

//给指定地址发送json报文
void Chat::send(const QJsonObject &obj, QString ip)
{
    //先用obj构造document，再用tojson转到buf
    QByteArray buf = QJsonDocument(obj).toJson();

    //将接收者ip绑定到sendto函数中
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(9988);
    addr.sin_addr.s_addr = inet_addr(ip.toUtf8().data());//0xffffffff

    //函数获得JSON数据向指定ip发送数据
    sendto(udp_fd, buf.data()/*字符串传输*/, buf.size(), 0,
           (struct sockaddr*)&addr, sizeof(addr));
}

void * Chat::recv_thread(void * ptr)//静态成员函数
{
    Chat * This = (Chat*) ptr;
    This->run();//静态成员函数不能直接访问非静态成员变量，在run里面调用
    return NULL;
}

void * Chat::recv_file_thread(void * ptr)//接收文件的线程
{
    SendFileInfo * info = (SendFileInfo *) ptr;
    info->chat->recv_one_file(info);
    return NULL;
}

void Chat::recv_one_file(SendFileInfo * info)//接收文件
{
    int newfd = accept(info->server_fd, NULL, NULL);
    if(newfd < 0) return;

    QFile file(info->filepath_dst);
    file.open(QFile::WriteOnly);
    while(info->transSize < info->total)
    {
        char buf[4096];
        int ret = read(newfd, buf, sizeof(buf));
        if(ret > 0)
        {
            file.write(buf, ret);
            info->transSize += ret;

            emit sigProgress(info);
        }
        else if(ret < 0)
        {
            continue;
        }
        else
            break;
    }
    file.close();
    close(newfd);
    emit sigTransFinish(info);
}

void * Chat::send_file_thread(void * ptr)//发送文件的线程
{
    SendFileInfo * info = (SendFileInfo *) ptr;
    info->chat->send_one_file(info);
    return NULL;
}

void Chat::send_one_file(SendFileInfo * info)//发送文件
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(info->port);
    addr.sin_addr.s_addr = inet_addr(info->ip.toUtf8().data());

    ::connect(fd, (struct sockaddr*)&addr, sizeof(addr));

    QFile file(info->filepath_src);
    file.open(QFile::ReadOnly);
    while(info->transSize < info->total)
    {
        QByteArray buf = file.read(4096);
        if(buf.length() == 0) break;

        info->transSize += buf.size();
        write(fd, buf.data(), buf.size());
         emit sigProgress(info);
    }

    close(fd);
    file.close();
    //发信号，通知猪线程删除结构体
    emit sigTransFinish(info);
}

void Chat::run()//处理接受数据
{
    while(1)
    {
        //存储对方的ip地址
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        //数据接收
        char buf[2048];
            //阻塞等待数据，并且获得了发送者的ip地址信息
        int ret = recvfrom(udp_fd, buf, sizeof(buf), 0, (struct sockaddr*)&addr, &len);
        if(ret > 0)
        {
            //处理JSON报文
            buf[ret] = 0;//字符串
            QJsonDocument doc = QJsonDocument::fromJson(QByteArray(buf));
            //获取对方ip
            char *ip = inet_ntoa(addr.sin_addr);
            //解析数据包
            handleMsg(doc.object(), ip);//根节点
        }
        else
        {
            QThread::msleep(100);
        }
//        else if(ret < 0 && errno != EINTR)
//        {
//            qDebug() << "data errno";
//            exit(2);
//        }
    }
}

//运行在独立的线程中，Qt规定线程不能更新主界面，可以发信号
//解析报文内容，传入的ip是发送者的ip
void Chat::handleMsg(const QJsonObject &obj, QString ip)
{
    //对自己的ip屏蔽
    if(ips.indexOf(ip) != -1)
    {
        qDebug() << "自己给自己发送了数据，服务器屏蔽，ip地址：" << ip << "用户名：" << obj.value(NAME).toString();
        return;
    }

    //提取命令
    QString cmd = obj.value(CMD).toString();
    if(cmd == ONLINE)
    {
        //提取发送方的名字和ip
        QString name = obj.value(NAME).toString();
        addUser(ip, name);

        //发JSON报文回复这个ip
        QJsonObject resp;
        resp.insert(CMD, QString(ONLINEACK));
        resp.insert(NAME, this->myaccount);

        // 给对方发送报文
        send(resp, ip);
    }

    if(cmd == ONLINEACK)
    {
        //提取名字,ip就是参数ip
        QString name = obj.value(NAME).toString();
        addUser(ip, name);
    }

    if(cmd == CHAT)
    {
        bool broadcast = obj.value(BROADCAST).toBool();
        QString content = obj.value(CONTENT).toString();
        QString name = obj.value(NAME).toString();

        //通知界面更新
        emit sigNewContent( name,  content,  broadcast, ip);
    }

    /*
    QJsonObject obj;
    obj.insert(CMD, QString(SENDFILE));
    obj.insert(NAME, info.fileName());
    obj.insert(FILESIZE, filesize);
    obj.insert(ID, sfInfo->id);
    */
    if(cmd == SENDFILE)//此时是文件接收方
    {
        QString filename = obj.value(NAME).toString();//文件名
        int filesize = obj.value(FILESIZE).toInt();//文件大小
        int peerid = obj.value(ID).toInt();//文件结构体在对方map中的存储位置

        //让主线程去创建文件信息结构体
        emit this->sigTransFileRequest(filename, filesize,peerid,ip);
    }

    if(cmd == SENDFILEACK)
    {
        QString ok = obj.value(RESULT).toString();
        if(ok == OK)
        {
            //创建发送线程
            int id = obj.value(ID).toInt();
            int peerid = obj.value(PEERID).toInt();
            int port = obj.value(PORT).toInt();

            emit sigAckFileTrans(id, peerid, port);
        }
        else if(ok == CANCEL)
        {
            //让主线程去删除SendFileInfo结构体
        }
    }
}

//创建线程发送文件
void Chat::onAckFileTrans(int id, int peerid, int port)
{
    SendFileInfo * info = transinfo[id];
    info->peerid = peerid;
    info->port = port;

    pthread_t t;
    pthread_create(&t, NULL, send_file_thread, info);
    pthread_detach(t);
}

void Chat::addUser(QString ip, QString name)
{
    User* user = new User;
    user->name = name;
    user->ip = ip;

    //ip和名字放入map
    if(others[ip])
    {
        delete others[ip];//删除重复
    }
    others[ip] = user;

    emit sigNewUser(name, ip);
}

//发送聊天内容
void Chat::sendMsg(QString content, QString ip)
{
/*
{
    cmd:"chat"
    broadcast:true
    content:"msg"
    name:"name"
}
*/
    QJsonObject obj;
    obj.insert(CMD, QString(CHAT));
    //obj.insert(BROADCAST, broadcast);
    obj.insert(BROADCAST, ip.indexOf("255") != -1);
    obj.insert(CONTENT, content);
    obj.insert(NAME, myaccount);

    send(obj, ip);
}

void Chat::sendOnline()
{
    //清除旧列表信息，对新用户上线没有影响
    //遍历列表信息，逐个删除
    foreach (QString ip, others.keys())
    {
        delete others[ip];
    }
    others.clear();

    //发送报文
    QJsonObject obj;
    /*上线的JAON报文：
        {
            cmd: online,
            name: account-name
        }
    */
    obj.insert(CMD, QString(ONLINE));
    obj.insert(NAME, myaccount);
    //以广播形式发送报文
    send(obj, broadcast_ip);
}

//构建文件结构体保存到map表，并发送 文件发送请求报文---此时是文件发送者
void Chat::sendFile(QString path, QString ip)
{
    //获取文件大小
    QFile file(path);
    file.open(QFile::ReadOnly);
    int filesize = file.size();
    file.close();

    //保存要发送的文件的信息
    SendFileInfo* sfInfo = new SendFileInfo(this);
    sfInfo->filepath_src = path;
    sfInfo->total = filesize;
    sfInfo->ip = ip;

    //发送SENDFILE报文
    //获得文件名
    QFileInfo info(path);
    //info.path();
    //info.fileName();
    QJsonObject obj;
    obj.insert(CMD, QString(SENDFILE));
    obj.insert(NAME, info.fileName());
    obj.insert(FILESIZE, filesize);
    obj.insert(ID, sfInfo->id);

    send(obj, ip);

    //保存文件结构到map表
    transinfo[sfInfo->id] = sfInfo;
}

//响应文件发送请求报文后采取的行动---此时是文件接收者
//在主线程运行
void Chat::ackFileTransRequest(QString filename, int filesize,
                               int peerid, QString ip, bool ok, QString dstfilename)
{
    if(ok)
    {
        //初始化结构体属性
        SendFileInfo * info = new SendFileInfo(this);
        info->filepath_src = filename;
        info->filepath_dst = dstfilename;
        info->peerid = peerid;//接收方的id在结构体建立时就初始化了
        info->total = filesize;
        info->ip = ip;//发送文件者的ip

        //放入map表
        transinfo[info->id] = info;

        //创建socket，准备接收文件
        info->server_fd = socket(AF_INET, SOCK_STREAM, 0);
        //不绑定ip地址和端口，让系统自动绑定
        listen(info->server_fd, 20);
        //系统自动绑定后去获取端口
        struct sockaddr_in addr;
        socklen_t len = sizeof(addr);
        getsockname(info->server_fd, (struct sockaddr*)&addr, &len);
        qDebug() << "port is:" << ntohs(addr.sin_port);

        //创建线程，等待文件接收和接收处理文件
        pthread_t t;
        pthread_create(&t, NULL, recv_file_thread, info);
        pthread_detach(t);

        //给发送者回复报文
        QJsonObject obj;
        obj.insert(CMD, QString(SENDFILEACK));
        obj.insert(RESULT, QString(OK));
        obj.insert(ID, info->peerid);//该文件信息在发送方map表的位置
        obj.insert(PEERID, info->id);
        obj.insert(PORT, (int)ntohs(addr.sin_port));
        //发送
        send(obj, ip);
    }
    else
    {
        //给发送者回复报文
        QJsonObject obj;
        obj.insert(CMD, QString(SENDFILEACK));
        obj.insert(RESULT, QString(CANCEL));
        obj.insert(ID, peerid);//该文件信息在发送方map表的位置
        //发送
        send(obj, ip);
    }
}
