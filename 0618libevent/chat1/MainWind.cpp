#include "MainWind.h"
#include <QSplitter>    //让窗口中间的分隔线可以移动
#include <QHBoxLayout>
#include <QString>
#include <QVBoxLayout>
#include <QDebug>
#include <QtEndian>

#define uint32_t unsigned int
/* 实现方法：
 * 创建窗口
 * 布局控件
 * 创建客户端socket对象
 * 连接指定服务器和端口号
 *
 * 设置可以接收服务器发过来的消息
 * 通过socket接收发过来的数据，将其显示在窗口上
 *      先处理数据头
 *      再处理数据内容
 * 通过socket发送数据
 *      获取对方ip
 *      将ip和地址绑定
 *      先写入这个字符串的大小
 *      在写入这个字符串
 */

MainWind::MainWind(QWidget *parent) :
    QWidget(parent)
{
    /*窗口设置*/
    leftWindow = new QListWidget;
    rightWindow = new QWidget;
    leftWindow->setWindowTitle("用户列表");
    rightWindow->setWindowTitle("聊天窗口");

    QHBoxLayout * layout = new QHBoxLayout(this);
    QVBoxLayout * vBox = new QVBoxLayout(rightWindow);

    QSplitter * splitter = new QSplitter;
    splitter->addWidget(leftWindow);
    splitter->addWidget(rightWindow);

    layout->addWidget(splitter);
    vBox->addWidget(output = new QTextBrowser, 2);
    vBox->addWidget(input = new QTextEdit, 1);
    vBox->addWidget(send = new QPushButton("send"));
    vBox->setMargin(0);

    /*连接服务器*/
    socket = new QTcpSocket;
    //登陆连接服务器
#if 0
    //socket->bind(QHostAddress("192.168.2.117"));
    socket->connectToHost(QHostAddress("192.168.87.128"), 9988);
#else
    //socket->bind(QHostAddress("192.168.2.117"));
    socket->connectToHost(QHostAddress("192.168.2.117"), 9988);
#endif
    //服务器发消息过来
    connect(socket, SIGNAL(readyRead()), this, SLOT(onReadyRead()));

    //点按钮发消息
    connect(send, SIGNAL(clicked()), this, SLOT(onSend()));
}

//读取数据
void MainWind::readData()
{
    buf += socket->read(packetSize + 4 - buf.size());
}

void MainWind::onReadyRead()
{
    while(socket->bytesAvailable() > 0)
    {
        if(buf.size() < 4)
        {
            buf += socket->read(4 - buf.size());
            if(buf.size() == 4)
            {
                u_int32_t p = *((u_int32_t*)buf.data());
                packetSize = qFromBigEndian(p);
                qDebug() << "\n数据包大小：" << packetSize;

                readData();
            }
        }
        else
        {
            readData();
        }

        if(buf.size() == packetSize + 4)
        {
            buf = buf.mid(4);//偏移4字节
            //buf[packetSize] = 0;
            qDebug() << buf;

            //buf可能出现的情况
            // ip
            // ip|chat
            // ip|chat|chat
            //用index查找|
            int index = buf.indexOf("|");
            QString ip;
            //上线消息
            if(index == -1)//没找到
            {
                ip = buf;
                this->leftWindow->addItem(ip);
            }
            //聊天消息
            else
            {
                QString content;
                ip = buf.left(index);
                content = buf.mid(index+1);

                //显示输出
                this->output->append("[" + ip + "]" + "  :\n" +  content + "\n");
            }

        }
        else
        {
            //...
        }
        buf.clear();//readSize = 0，这个放置的位置好像不太合适
    }
}

//发数据
void MainWind::onSend()
{
    //从输入框获取数据
    if(input->toPlainText().size() == 0)
    {//没有数据
        return;
    }
    //选用户发送
    if(leftWindow->currentItem() == NULL)
    {//没选择发送对象
        return;
    }

    QListWidgetItem * item = leftWindow->currentItem();
    QString ip = item->text();

    QByteArray buffer = ip.toUtf8() + "|" +  input->toPlainText().toUtf8();
    //qDebug() << "buffer = " << buffer;
    u_int32_t size = buffer.size();
    //qDebug()  << "size = " << size;
    size = qToBigEndian(size);

    int ret = socket->write((const char*)&size, 4);
    //qDebug() << ret;
    ret = socket->write(buffer);
    //qDebug() << ret;

    output->append("[Me]  :\n" + input->toPlainText() + "\n");
    input->clear();
}
