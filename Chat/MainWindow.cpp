#include "MainWindow.h"
MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent)
{
    //接收chat对象产生的信号
    connect(&chat, SIGNAL(sigNewUser(QString,QString)),
            this, SLOT(onNewUser(QString,QString)));
    connect(&chat, SIGNAL(sigNewContent(QString , QString , bool, QString)),
            this, SLOT(onNewContent(QString , QString , bool, QString)));
    connect(&chat, SIGNAL(sigTransFileRequest(QString,int,int,QString)),
            this, SLOT(onTransFileRequest(QString,int,int,QString)));
    connect(&chat, SIGNAL(sigProgress(SendFileInfo*)),
            this, SLOT(onProgress(SendFileInfo*)));
    connect(&chat, SIGNAL(sigTransFinish(SendFileInfo*)),
            this, SLOT(onTransFinish(SendFileInfo*)));

    //信号连接以后在调用初始化函数，保证chat发出的信号不丢失
    chat.init();

    //适配窗口
    QVBoxLayout * mainlayout = new QVBoxLayout(this);

    //可以动分割线
    QSplitter * splitter = new QSplitter(this);
    mainlayout->addWidget(splitter);

    //左边好友列表，右边插件集合
    QWidget * container;//容器--插件集合
    splitter->addWidget(userList = new QListWidget);
    //userList->addItem("好友列表");
    splitter->addWidget(container = new QWidget);
    //整理容器
    QVBoxLayout * vBox = new QVBoxLayout(container);
    QHBoxLayout * hBox = new QHBoxLayout;//按钮集合
    hBox->addWidget(setup = new QPushButton("设置"));
    hBox->addWidget(sendFile = new QPushButton("发文件"));
    hBox->addWidget(sendmsg = new QPushButton("发送"));
    vBox->addWidget(msgshow = new QTextBrowser, 4);
    vBox->addWidget(msgInput = new QTextEdit, 1);
    vBox->addLayout(hBox);

    //边距
    vBox->setMargin(0);
    hBox->setMargin(0);

    //发送消息
    connect(sendmsg, SIGNAL(clicked()), this, SLOT(onsend()));
    connect(setup, SIGNAL(clicked()), this, SLOT(onSetup()));
    connect(sendFile, SIGNAL(clicked()), this, SLOT(onSendFile()));

    initUserList();
}

void MainWindow::onNewUser(QString name, QString ip)
{
    //主线程收到信号，信号参数含有新用户的ip和用户名，更好友列表
    //先遍历列表，看看是否已经存在过，有就删掉
    int count = this->userList->count();//统计列表好友数量
    for(int i=0; i<count; ++i)
    {
        QListWidgetItem * item = userList->item(i);//单个好友数据结构体
        if(item->text().indexOf(ip) != -1)//查找指定ip在结构体中的位置(下标)
        {
            delete userList->takeItem(i);//删除item
            break;
        }
    }
    //添加好友
    this->userList->addItem(name+"@"+ip);
}

void MainWindow::onsend()
{
    //获取对方ip地址
    QString ip = getSelectIp();

    //输入框获取内容
    QString content = msgInput->toPlainText();
    if(content.length() == 0)
    {
        return;
    }

    //转交任务给后台类去发聊天内容
    chat.sendMsg(content, ip);

    //更新界面
    msgInput->clear();
    msgshow->append("我对" + ip + "说:\n" + content + "\n");
}

void MainWindow::onNewContent(QString name, QString content, bool broadcast, QString ip)
{
    if(broadcast)
        msgshow->append(name + "对大家说:\n" + content);
    else
        msgshow->append(name + "对我说:\n" + content);
}

void MainWindow::onSetup()
{
    //弹出选择对话框
    QDialog dlg;
    QComboBox * combo;
    QHBoxLayout * lay = new QHBoxLayout(&dlg);
    lay->addWidget(new QLabel("网卡"));
    lay->addWidget(combo = new QComboBox());
    //QStringList ret;
    {
        QList<QNetworkAddressEntry> entrys;

        QList<QNetworkInterface> infs = QNetworkInterface::allInterfaces();
        foreach(QNetworkInterface inf, infs)
        {
            entrys.append(inf.addressEntries());
        }
        foreach (QNetworkAddressEntry entry, entrys)
        {
            //获取地址
            if(entry.broadcast().toString().isEmpty())
                continue;
            combo->addItem(entry.ip().toString());//获取ip地址
            combo->setItemData(combo->count()-1, entry.broadcast().toString());//广播地址绑定ip地址
        }
    }

    //下拉选项增加广播列表
    //combo->addItems(ret);

    dlg.exec();

    //得到当前用户的选择，并且赋值给chat对象
    chat.broadcast_ip = combo->itemData(combo->currentIndex()).toString();

    //重新创建socket，传入用户想要绑定的ip地址
    chat.create_socket(combo->currentText());

    //重新发一次上线通知，这里要清空旧的好友列表，获取新列表
    chat.sendOnline();

    //清空主界面的列表
    initUserList();
}

void MainWindow::initUserList()
{
    this->userList->clear();
    this->userList->addItem("所有用户@" + chat.broadcast_ip);
}

void MainWindow::onSendFile()
{
    //获取对方ip地址
    QString ip = getSelectIp();

    QString filename = QFileDialog::getOpenFileName(NULL, "请选择发送的文件");
    if(filename.length() == 0)
    {
        return;
    }
    qDebug() << filename;
    chat.sendFile(filename, ip);
}

QString MainWindow::getSelectIp()
{
    //从好友列表选择对话的好友，同时获得ip地址
    QString text = userList->currentItem()->text();
    qDebug() << text;//name@ip

    //拆分字符串，获得ip地址
    QStringList stringlist = text.split('@');
    if(stringlist.length() != 2)
    {
        return "";
    }
    QString ip = stringlist.at(1);
    return ip;
}

void MainWindow::onTransFileRequest(QString filename, int filesize,
                                     int peerid, QString ip)
{
    //弹对话框询问获得选项
    QString msg = QString("%1 给你发送文件 %2，是否接收？").arg(chat.others[ip]->name, filename);
    if(QMessageBox::question(this, "收到文件请求", msg) == QMessageBox::Yes)
    {
        QString dstfilename = QFileDialog::getSaveFileName(this);
        if(dstfilename.length() == 0)
        {
            chat.ackFileTransRequest(filename, filesize, peerid, ip, false);
            return;
        }
        chat.ackFileTransRequest(filename, filesize, peerid, ip, true, dstfilename);
    }
    else
    {
        chat.ackFileTransRequest(filename, filesize, peerid, ip, false);
    }
}

//文件接收处理进度
void MainWindow::onProgress(SendFileInfo * info)
{
    qDebug() << "trans progress" << info->transSize << " | " << info->total;
}

//文件接收传输完成，删除info结构体
void MainWindow::onTransFinish(SendFileInfo * info)
{
    if(info->server_fd == -1)
        qDebug() << "send trans complete" << info->filepath_src;
    else
        qDebug() << "receve trans complete" << info->filepath_dst;
    chat.transinfo.remove(info->id);
    delete info;
}
