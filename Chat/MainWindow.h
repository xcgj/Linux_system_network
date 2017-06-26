#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QLabel>
#include <QComboBox>
#include <QVariant>
#include <QFileDialog>
#include <QMessageBox>

//窗口插件
#include <QListWidget>
#include <QTextBrowser>
#include <QTextEdit>
#include <QPushButton>

//窗口布局
#include <QSplitter>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include "Chat.h"

class MainWindow : public QWidget
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

    //插件
    QListWidget      * userList;
    QTextBrowser   * msgshow;
    QTextEdit           * msgInput;
    QPushButton    * sendmsg;
    QPushButton    * sendFile;
    QPushButton    * setup;

    void initUserList();

    //业务执行对象
    Chat chat;

    QString getSelectIp();

signals:

public slots:
    //主界面自调函数，点发送按钮发送消息，更新聊天框、输入框
    void onsend();
    void onSetup();
    void onSendFile();

    //处理外部信号函数
        //收到新用户上线的信号处理函数，更新好友列表
    void onNewUser(QString name, QString ip);
        //处理用户消息信号，更新聊天框
    void onNewContent(QString name, QString content, bool broadcast, QString ip);
        //处理文件接收请求
    void onTransFileRequest(QString filename, int filesize, int peerid, QString ip);
        //文件接收处理进度
    void onProgress(SendFileInfo * info);
        //文件接收传输完成，删除info结构体
    void onTransFinish(SendFileInfo * info);
};

#endif // MAINWINDOW_H
