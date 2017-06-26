/*************************************************************************
  > File Name: pack.c
  > Author: 陈琳耀 || C01
  > Mail: 1003192057@qq.com 
  > Created Time: 2017年06月01日 星期四 15时19分15秒
 ************************************************************************/
#include "header.h"
#include <string>
#include <dirent.h>
#include <limits.h>

using namespace std;

//打包文件
void tarfile(string filepath, FILE * fp)
{
    //标记文件的目录信息
    //printf("file %s\n", filepath.c_str());

    //写入文件名和尺寸，方便解包
    fprintf(fp, "F\n");//文件类型标识符
    fprintf(fp, "%s\n", filepath.c_str());//文件名
    struct stat filestat;
    stat(filepath.c_str(), &filestat);
    fprintf(fp, "%d\n", (int)filestat.st_size);

    //拷贝文件内容到fp中
    //==读取需要被打包文件的内容
    FILE * f = fopen(filepath.c_str(), "r");
    if (f == NULL)
    {
        printf("fopen %s error\n", filepath.c_str());
        return ;
    }

    //写入打包文件
    char buf[4096] = {0};
    while(1)
    {
        int ret = fread(buf, 1, sizeof(buf), f);
        if (ret <= 0)
        {
            break;
        }

        fwrite(buf, 1, ret, fp);
        memset(buf, 0, sizeof(buf));
    }

    fclose(f);
}

//打包目录
void tardir(string dirpath, FILE * fp)
{
    //标记文件的目录信息
    //printf("dir %s\n", dirpath.c_str());

    //写入目录名，方便解包
    fprintf(fp, "D\n");//文件类型标识符
    fprintf(fp, "%s\n", dirpath.c_str());//目录名

    //遍历目录所有文件，依次打包
    //==打开目录
    DIR * dir = opendir(dirpath.c_str());
    struct dirent* entry = NULL;
    while(1) 
    {
        entry = readdir(dir);//读取dir中的目录项
        if(entry == NULL)//目录项读完了
        {
            return;
        }

        //不读取.和..
        if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0)
        {
            continue;
        }

        //获取新的路径
        //char newpath[1024];
        //sprintf(newpath, "%s/%s", dirpath, entry->d_name);
                
        
        //读到文件
        if (entry->d_type == DT_REG)
        {
            //文件打包
            tarfile(dirpath + "/" + entry->d_name, fp);
        }

        //读到目录
        if (entry->d_type == DT_DIR)
        {
            //tar函数递归
            tardir(dirpath + "/" + entry->d_name, fp);
        }
    }
}

void tar(const char * path, const char * dstfile)
{
    //将整个目录文件内容放入用户指定的dstfile文件中
    //先创建这个文件
    FILE * fp = fopen(dstfile, "w");//移植性好
    if (fp == NULL)
    {
        //perror("fopen");
        printf("open file failed\n");
        return ;
    }

    //打包
    tardir(path, fp);

    fclose(fp);
}

//解包
void untar(const char * tarfile)
{
    FILE * fp = fopen(tarfile, "r");//读取包中的内容
    if (fp == NULL)
    {
        printf("解包文件打开失败\n");
        return;
    }

    //解析内容
    char buf[4096] = {0};
    while(1)
    {
        //按行读取
        if(fgets(buf, sizeof(buf), fp) == NULL)
        {
            break;
        }
        //读到文件类型是普通文件
        if (strcmp(buf, "F\n") == 0)
        {
            //读文件名产生文件
            fgets(buf, sizeof(buf), fp);//文件路径
            buf[strlen(buf)-1] = 0; //去掉 \n
            FILE * file = fopen(buf, "w");

            //再get一下文件大小
            fgets(buf, sizeof(buf), fp);//文件大小
            buf[strlen(buf)-1] = 0; //去掉 \n
            int len = atoi(buf);

            //写入内容
            while(1)
            {
                //判断内容是否超出缓存大小
                int readlen = len > sizeof(buf)?sizeof(buf):len;
                int ret = fread(buf, 1, readlen, fp);
                if (ret <= 0)
                {
                    //文件格式出错
                    printf("file error\n");
                    return;
                }
                fwrite(buf, 1, ret, file);
                len -= ret;
                if (len <= 0)
                {
                    break;
                }
            }

            fclose(file);
        }
        //读到文件类型是目录文件
        else if (strcmp(buf, "D\n") == 0)
        {
            //读取目录名产生目录
            fgets(buf, sizeof(buf), fp);//文件路径
            buf[strlen(buf)-1] = 0;//去掉 \n
            mkdir(buf, 0777);//创建目录
        }
    }
}


// ./tar dir dstfile    打包
// ./tar dstfile 解包
//
//
// ./a.out ../../dir tarfile  跨目录打包
int main(int argc, char * argv[])
{
    //打包
    if(argc == 3)
    {
        //在下面的chdir改变工作目录之前，先保存打包文件生成的绝对路径
        //防止打包文件的路径跟随工作目录一起改变
        char taroutpwd[1024] = {0};
        getcwd(taroutpwd, sizeof(taroutpwd));//打包文件会产生在这个目录下
        string dst = string(taroutpwd) + "/" + argv[2];
        
        
        //==先转成绝对路径
        char cwd[1024] = {0};
        realpath(argv[1], cwd);//../test --> /c01/0601
        printf("cwd = %s\n", cwd);

        //将目标目录的上一级目录 修改为当前工作目录，因此要加"/.."
        char tarinpwd[1024] = {0};
        string src1 = argv[1];
        chdir((src1 + "/..").c_str());//将当前路径改为字符串所显示的路径
        getcwd(tarinpwd, sizeof(tarinpwd));//得到执行打包任务的工作路径／／0601

        //反搜索 ／
        string src = rindex(cwd, '/') + 1;//找到'/'最后出现的位置,+1后得到被打包目录

        //char realpth[1024] = {0}; 
        //realpath((src + "/..").c_str(), realpth);//相对路径转换成绝对路径
        
        printf("src = %s, dst = %s, cwd = %s\n", 
                src.c_str(), dst.c_str(), cwd);

        //return 0;
        tar(src.c_str(), dst.c_str());
    }

    //解包
    else if(argc == 2)
    {
        untar(argv[1]);
    }

    else
    {
        printf("error");
    }

    return 0;
}
