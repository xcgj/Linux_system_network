/*************************************************************************
	> File Name: copyfile.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月01日 星期四 14时22分32秒
 ************************************************************************/

#include ".../header.h"
#define SIZE 4096

int main(int argc, char*argv[])
{
    // 检查参数
    if (argc != 3)
    {
        printf("argument wrong:%s [sorcefile] [destinyfile]\n", argv[0]);
        return -1;
    }

    //拷贝内容
    //==获取文件名
    char * srcfile = argv[1];
    char * dstfile = argv[2];

    //==打开、创建读写文件
    FILE * src = fopen(srcfile, "r");
    if (src == NULL)
    {
        perror("fopen");
        return -2;
    }
    FILE * dst = fopen(dstfile, "w");

    //==拷贝
    char buf[SIZE] = { 0 };
    while(1)
    {
        int count = fread(buf, 1, sizeof(buf), src);
        if (count <= 0)
        {
            break;
        }
        fwrite(buf, 1, count, dst);
        memset(buf, 0, sizeof(buf));
    }

    fclose(src);
    fclose(dst);

    //拷贝文件属性
    //==获取源文件属性
    struct stat srcstat;
    stat(srcfile, &srcstat);//将srcfile的属性拷到srcstat

    //==目标文件时间
    struct utimbuf times;
    times.actime = srcstat.st_atime;
    times.modtime = srcstat.st_mtime;
    int utimeret = utime(dstfile, &times);
    if (utimeret != 0)
    {
        perror("utime");
        return -3;
    }

    //==目标文件权限
    int chmodret = chmod(dstfile, srcstat.st_mode);
    if (chmodret != 0)
    {
        perror("chmod");
        return -4;
    }

    //==目标文件所有者
    int chownret = chown(dstfile, srcstat.st_uid, srcstat.st_gid);
    if (chownret != 0)
    {
        perror("chown");
        return -5;
    }

    return 0;
}
