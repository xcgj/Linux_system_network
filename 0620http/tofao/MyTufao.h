#ifndef MYTUFAO_H
#define MYTUFAO_H

#include <QObject>
#include <QUrl>

#include <tufao-1/Tufao/HttpServer>
#include <tufao-1/Tufao/HttpServerRequest>
#include <tufao-1/Tufao/HttpServerResponse>
using namespace Tufao;

class MyTufao : public QObject
{
    Q_OBJECT
public:
    explicit MyTufao(QObject *parent = 0);

    //服务器
    HttpServer server;

    void handlePostData(
            HttpServerRequest& request,
            HttpServerResponse& response);
signals:

public slots:
    void onRequestReady(Tufao::HttpServerRequest&,
                        Tufao::HttpServerResponse&);
};

#endif // MYTUFAO_H
