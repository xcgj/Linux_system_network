/*************************************************************************
	> File Name: UidGid.c
	> Author: 陈琳耀 || C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月02日 星期五 18时23分50秒
 ************************************************************************/

#include<stdio.h>
#include<unistd.h>

int main()
{
    uid_t uid = getuid();   //真实账户
    uid_t euid = geteuid(); //有效账户
    gid_t gid = getgid();   //真实组
    gid_t egid = getegid(); //有效组

    printf("uid = %d, euid = %d\n", uid, euid);
    printf("gid = %d, egid = %d\n", gid, egid);

    return 0;
}
