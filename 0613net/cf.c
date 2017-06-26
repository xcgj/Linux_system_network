/*************************************************************************
	> File Name: cf.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月13日 星期二 22时14分16秒
 ************************************************************************/

#include"../header.h"

int main(int argc, char * argv[])
{
    if(argc != 2) 
    {
        printf("参数错误\n");
        return -1;
    }
    char * filename = argv[1];

    //创建客户端对象
    int cfd = socket(AF_INET, SOCK_STREAM, 0);

    //初始化服务器的地址
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(20004);

    //连接服务器
    int ret = connect(cfd, (struct sockaddr*)&addr, sizeof(addr));
    if (ret < 0)
    {
        perror("connect");
        return 0;
    }

    printf("连接成功\n");

    printf("开始传输文件名...\n");
    //传数据
    //  传文件名
    char buf[1024] = {0};
    while(1)
    {
    int filenamelen = strlen(filename);
    int writeret = write(cfd, filename, filenamelen);
    
    //sleep(1);

    if(writeret == filenamelen)
        read(cfd, buf, sizeof(buf));

    if (strcmp(buf, "OK") == 0)
        break;
    }
    printf("%s\n", buf);
    //write(cfd, buf, sizeof(buf));
    //write(cfd, "xcgj", 4);

    printf("开始传输内容...\n");
    //  传文件内容
    int ffd = open(filename, O_RDONLY);
    struct stat filestat;
    fstat(ffd, &filestat);
    int filelen = filestat.st_size;
    printf("文件大小 = %d...\n", filelen);
    while(1)
    {
        memset(buf, 0, sizeof(buf));
        int writebytes = write(ffd, buf, sizeof(buf));
        filelen -= writebytes;
        if(filelen <= 0)
            break;
    }   
    
    printf("文件大小 = %d...传输结束...\n", filelen);

    close(cfd);
    return 0;
}
