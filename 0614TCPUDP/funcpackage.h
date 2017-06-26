#ifndef FUNCPACKAGE_H
#define FUNCPACKAGE_H

#include "../header.h"

//创建服务器socket，直接创建到监听步骤
int myServer(uint16_t port, const char * ip);

//创建客户端socket
int myClientConnect(uint16_t port, const char * ip);

//accept创建新的连接fd
int myAccept(int serverfd, struct sockaddr * addr, socklen_t * addrlen);

//read，规定从管道读取固定的字节数，和读几次无关
int myRead(int socketfd, char * buf, int buflen);

//文件有多少读多少
int readAny(int socketfd, char * buf, int buflen);

//write，规定向管道写入固定的字节数
int myWrite(int socketfd, char * buf, int len);

//传输数据头部的数据大小标志
int myWriteLen(int fd, uint32_t len);

//接收数据头部的大小
int myReadLen(int fd, uint32_t * len);
#endif
