/*************************************************************************
	> File Name: mycp.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月05日 星期一 16时58分42秒
 ************************************************************************/

#include "../header.h"

//子进程拷贝函数
void procpy(int index, const char* sf, const char* df, int flen, int pro)
{
    //打开文件
    int sfd = open(sf, O_RDONLY);
    int dfd = open(df, O_WRONLY);

    //定位文件游标
    int part = flen;
    if (pro != 1)
        part = flen / (pro - 1);//让最后一个进程考剩余的部分
    int pos = index*part;
    lseek(sfd, pos, SEEK_SET);
    lseek(dfd, pos, SEEK_SET);

    //读写文件
    char buf[4096] = {0};
    while(1)
    {
        int convert = 0;
        if (part < sizeof(buf))
        {
            convert = part;
        } 
        else
        {
            convert = sizeof(buf);
        }

        int ret = read(sfd, buf, convert);
        write(dfd, buf, ret);

        part -= ret;
        if (part <= 0)
        {
            break;
        }
    }
}

// ./a.out sf df 4
int main(int argc, char * argv[])
{
    //解析参数
    const char* sf = argv[1];
    const char* df = argv[2];
    int pro = atoi(argv[3]);

    //创建目标文件
    unlink(df);//删除旧文件
    int fd = open(df, O_CREAT, 0777);//创建文件
    close(fd);

    //计算源文件的文件大小
    struct stat inf;
    stat(sf, &inf);
    int flen = inf.st_size;

    //产生子进程，拷贝文件
    int i = 0;
    for (; i < pro; ++i)
    {
        pid_t pid = fork();
        if (pid == 0)
        {
            procpy(i, sf, df, flen, pro);
            return 0;
        }
    }

    //父进程负责收尸
    for (i = 0; i < pro; ++i)
    {
        wait(NULL);
    }

    return 0;
}
