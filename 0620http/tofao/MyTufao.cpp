#include "MyTufao.h"
#include <QDebug>

#include <QJsonArray>//解析JSON
#include <QJsonDocument>
#include<QJsonObject>
#include<QJsonValue>

/* 业务：用tufao创建服务器，并处理和响应客户端*/

//curl http://127.0.0.1:9988 -d "post data"

//pro:
//Qt += network
//LIBS += -ltufao1

MyTufao::MyTufao(QObject *parent) :
    QObject(parent)
{
    //服务器监听
    server.listen(  QHostAddress::Any,/*任意地址可连接*/
                            9988/*内部自动转换大小端*/);
    //如果连接了，通过信号告诉其他程序，让其他程序处理
    connect(&server,
            SIGNAL(requestReady(Tufao::HttpServerRequest&,
                                         Tufao::HttpServerResponse&)),
            this,
            SLOT(onRequestReady(Tufao::HttpServerRequest&,
                                      Tufao::HttpServerResponse&)));
}

//处理数据
void MyTufao::handlePostData(
        HttpServerRequest& request,
        HttpServerResponse& response)
{
    response.writeHead(HttpResponseStatus::OK);//请求结果：200
    response.write("xcgj");

    //qDebug() << request.readBody();
    QByteArray body = request.readBody();
    qDebug << body;

    //解析JSON
    QJsonDocument doc = QJsonDocument::fromJson(body);
    //得到根对象
    QJsonObject root = doc.object();
    //获取键值对
//    QJsonValue cmd = root.value("cmd");
//    QJsonValue uname = root.value("uname");
//    QJsonValue passwd = root.value("passwd");
    QString cmd = root.value("cmd").toString();
    QString uname = root.value("uname").toString();
    QString passwd = root.value("passwd").toString();
    if(cmd == "register")
    {
        //比较数据
        //响应数据用JSON格式写回，客户端需要做相应的解析工作
        QJsonObject obj;
        obj.insert("result", "ok");
        obj.insert("cmd", "register");

        //JSON转成字符串格式
        QJsonDocument doc(obj);
        //打包
        QByteArray responsejson = doc.toJson(doc);

        response.end(responsejson);
        return;
    }
    else if(cmd == "login")
    {
        //比较数据
        //响应数据用JSON格式写回
        QJsonObject obj;
        obj.insert("result", "ok");
        obj.insert("cmd", "login");

        //JSON转成字符串格式
        QJsonDocument doc(obj);
        //打包
        QByteArray responsejson = doc.toJson(doc);

        response.end(responsejson);
        return;
    }

    response.end("cmd not found");
}

//处理客户端发来的请求
void MyTufao::onRequestReady(
        HttpServerRequest& request,
        HttpServerResponse& response)
{
#if 0   //  GET
    //request   里面存储了客户端的请求
    qDebug() << "url is" << request.url().path();
    //qDebug() << request.httpVersion();  //1.1
    qDebug() << request.method();         //GET POST

    //response  向客户端写数据
    response.writeHead(HttpResponseStatus::OK);//请求结果：200
    response.write("xcgj");
    response.end("end");
#else
    if(request.method() == "POST")
    {
        connect(
                    &request,
                    &HttpServerRequest::end, //end信号表示正文也接收完了
                    [&](){
            handlePostData(request, response);
        });
    }
#endif
}
