/*************************************************************************
	> File Name: stat.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年05月30日 星期二 22时49分26秒
 ************************************************************************/

#include "../header.h"

int main()
{
    //结构体
    struct stat buf;

    int ret = stat("a.out", &buf);
    if (ret < 0)
    {
        //文件不存在
        perror((char*)&stat);//stat()函数可以用来判断文件是否存在
        return 0;
    }

    printf("ino is %d\n", (int)buf.st_ino);
    printf("mode is 0x%x\n", (int)buf.st_mode);
    printf("size is %d\n", (int)buf.st_size);
    
    if (S_ISREG(buf.st_mode))//判断是否是普通文件
    {
        printf("普通文件\n");
    }

    //判断拥有这个文件的账户，是否可以读这个文件
    if (S_IRUSR & buf.st_mode)
    {
        printf("用户可读");
    }
    return 0;
}
