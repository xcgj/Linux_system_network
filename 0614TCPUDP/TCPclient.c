/*************************************************************************
	> File Name: TCPclient.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月14日 星期三 15时07分53秒
 ************************************************************************/

#include"../header.h"
//客户端
//格式：./a.out src ip port
int main(int argc, char * argv[])
{
    signal(SIGPIPE, SIG_IGN);

    if(argc != 4)
    {
        printf("参数错误\n");
        return 0;
    }

    const char * dst = argv[1];
    const char * ip = argv[2];
    const char * port = argv[3];

    //创建sockt对象
    int clifd = socket(AF_INET, SOCK_STREAM, 0);
    printf("%d\n", clifd);

    //初始化服务器地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(atoi(port));

    //发送连接请求
    int ret = connect(clifd, (struct sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        perror("connect");
        return 0;
    }

    //处理数据
    int filefd = open(dst, O_RDONLY);
    char buf[1024];
    while(1)
    {
        //从文件读取内容到buf，最终读取的字节数是bufread
        int bufread = read(filefd, buf, sizeof(buf));
        if(bufread == 0) break;//文件读完了

        //读取的内容写入socket管道，最终写入的结果是writebytes，每次写入的结果是writeret
        int writebytes = 0;
        while(1)
        {
            //如果服务器的fd的缓冲区太小，会导致写入不全
            int writeret = write(clifd, buf+writebytes, bufread-writebytes);
            if(writeret > 0) 
            {
                writebytes += writeret;
            }
            if(writeret < 0)
            {
                perror("write");//未知严重错误
                break;
            }
            if(bufread == writebytes)//本次读取的buf全部写入socket了
            {
                break;
            }
        }
    }

    close(clifd);
    return 0;
}
