/*************************************************************************
	> File Name: fcpy.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月03日 星期六 23时05分04秒
 ************************************************************************/

#include "../header.h"

/*
 * 检查命令行参数是否合法
 * 获取源文件大小
 * 创建目标文件
 * 均分任务
 * 创建子进程分发任务，子进程完成任务退出
 * 父进程wait函数清理子进程
 * */

void multicp(const char * srcf, const char * dstf, int pos, int ave)
{
    //1 打开源文件和目标文件
    //2 定位文件游标位置
    //3 拷贝文件
    //1
    FILE * src = fopen(srcf, "r");
    FILE * dst = fopen(dstf, "w");
    //2
    fseek(src, pos, SEEK_SET);
    fseek(dst, pos, SEEK_SET);
    //3
    char buf[4096] = {0};
    while(1)
    {
        int cplen = sizeof(buf) < ave ? sizeof(buf) : ave;
        int ret = fread(buf, 1, cplen, src);
        fwrite(buf, 1, ret, dst);
        ave -= ret;
        if (ave <= 0)
        {
            break;
        }
    }

    fclose(src);
    fclose(dst);
}

//格式：./a.out -job n srcf dstf
int main(int argc, char* argv[])
{
    //判断命令行
    if (argc != 5)
    {
        printf("参数不够\n");
        return 1;
    }

    if (strcmp(argv[1], "-job") != 0)
    {
        printf("无效命令\n");
        return 2;
    }

    int n = atoi(argv[2]);
    if (n < 1)
    {
        printf("进程数错误\n");
        return 3;
    }

    //文件长度--stat
    char* srcf = argv[3];
    char* dstf = argv[4];

    //int stat(const char *path, struct stat *buf);
    // On success, zero is returned.  On error, -1 is returned, and  errno  is  set appropriately.
    struct stat buf;
    int ret = stat(srcf, &buf);//检查文件合法性
    if (ret == -1)
    {
        printf("文件不存在\n");
        return 4;
    }
    int flen = buf.st_size;

    //目标文件
    //文件存在--O_CREAT
    //文件长度--truncate
    //int truncate(const char *path, off_t length);
    //The  truncate() function cause the regular file named by path or referenced by fd to be truncated  to  a  size  of  precisely length bytes.
    int fd = open(dstf, O_WRONLY|O_CREAT, 0777);//保证文件存在
    close(fd);
    //文件长度一样大
    truncate(dstf, flen);

    //分段写文件
    int average = flen / n;
    int i = 0;
    for (; i < n - 1; ++i)
    {
        //产生 n - 1 个子进程拷贝文件，父进程拷贝最后的部分
        pid_t pid = fork();
        if (pid == 0)
        {
            //调用拷贝函数
            int pos = average * i;//文件指起始位置
            multicp(srcf, dstf, pos, average);
            return 0;//拷贝完成结束子进程
        }
    }

    //父进程完成最后部分拷贝
    int pos = average * i;
    int leftdata = flen - pos;
    multicp(srcf, dstf, pos, leftdata);

    //父进程清除子进程PCB
    for (i = 0; i < n - 1; ++i)
    {
        wait(NULL);
    }

    return 0;
}
