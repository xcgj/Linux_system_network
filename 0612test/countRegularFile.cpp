/*************************************************************************
	> File Name: countGeneralFile.c
	> Author: C01
	> Mail: 1003192057@qq.com 
	> Created Time: 2017年06月12日 星期一 10时57分50秒
 ************************************************************************/

#include <iostream>
#include <string>
#include <string.h>
#include <dirent.h>
using namespace std;

//递归遍历文件目录
//判断是否是普通文件

int g_fileno = 0;

void count(string dir)
{
    DIR * dstdir = opendir(dir.c_str());
    struct dirent * entry = NULL;

    while(1)
    {
        //当前目录遍历完了
        entry = readdir(dstdir);
        if (entry == NULL)
        {
            break;
        }
        //跳过. ..
        if (strcmp(entry->d_name, ".")==0 || strcmp(entry->d_name, "..")==0)
        {
            continue;
        }
        //遍历到目录，进去继续遍历
        if (entry->d_type == DT_DIR)
        {
            count(dir + "/" + entry->d_name);
        }
        //遍历到文件
        if (entry->d_type == DT_REG)
        {
            g_fileno++;
        }
        //cout << entry->d_name << " ";
    }

    //cout << endl;

    closedir(dstdir);
}

//格式：./a.out dir
int main(int argv, char * argc[])
{
    string dir = argc[1];
    
    count(dir);
    
    cout << g_fileno << endl;

    return 0;
}
