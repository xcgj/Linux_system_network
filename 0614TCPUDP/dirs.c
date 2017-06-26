/*************************************************************************
  > File Name: dirs.c
  > Author: C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月15日 星期四 13时02分41秒
 ************************************************************************/

#include"../network.h"

//目录
void recvDir(int fd)
{
    //先接受目录名字大小
    uint32_t len;
    myReadLen(fd, &len);

    //再接收目录名字
    char * dirpath = alloca(len+1);//栈上申请空间
    myRead(fd, dirpath, len);
    dirpath[len] = 0;

    //创建目录
    mkdir(dirpath, 0777);

}

//普通文件
int recvFile(int fd)
{
    //先接收文件名字大小
    uint32_t len;
    myReadLen(fd, &len);

    //再接收文件名字
    char * filepath = alloca(len+1);
    myRead(fd, filepath, len);
    filepath[len] = 0;

    //再接收文件内容大小
    myReadLen(fd, &len);

    //创建文件
    int filefd = open(filepath, O_WRONLY|O_CREAT|O_TRUNC, 0777);
    char buf[4096];
    //读取文件内容
    while(1)
    {
        int readlen = len;
        if(readlen > 4096)
        {
            readlen = 4096;
        }
        //一次性读取固定大小的内容
        int ret = myRead(fd, buf, readlen);
        if(ret > 0)
        {
            //一次性写入固定大小的内容
            myWrite(filefd, buf, ret);
            len -= ret;
        }
        else
        {
            break;//出错或对方关闭
        }
    }

    close(filefd);

    if(len == 0)//全部写入了
        return 0;
    return -1;
}

// ./a.out srcd dstd ip port
int main()
{
    //服务器socket对象
    int server = myServer(9988, "127.0.0.1");

    //连接
    while(1)//可能多个客户端请求连接
    {
        int fd = myAccept(server, NULL, NULL);
        if(fd < 0)
            return -1;

        //接收整个目录
        //  接收目录名，用来创建目录
        //  客户端遍历src，服务器创建
        //  客户端传送文件或者目录的标记，方便服务器创建

        while(1)
        {
            char flag;
            myRead(fd, &flag, 1);//读取标记

            if(flag == 'd')
            {
                recvDir(fd);
            }
            else if(flag == 'r')
            {
                int ret = recvFile(fd);
                if(ret < 0)
                    break;
            }
            else if(flag == 'x')
            {
                break;
            }
        }
        close(fd);
    }
    close(server);
    return 0;
}
