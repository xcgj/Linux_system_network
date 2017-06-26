#!/bin/bash

# 自定义变量，等号左右无空格
var=abc

# 使用变量
echo $var
echo ${var}edf

# 访问环境变量
echo $HOME

# 参数变量
# $0 就像命令行参数的./a.out，是可执行程序本身
echo "命令行参数会赋值给下面的变量"
echo $1
echo $2
# 这样写得到的是的参数$1+0
echo $10
# 这样写是参数10
echo ${10}
echo 参数个数是：$#

# 字符串和空格的使用
var="hello word"
var='hello word'
var="hello'word"
var='hello"word'
var=hello\ word

# 赋值``获得其他命令的输出结果
var=`ls`
echo $var

# 执行子进程bash，参数的问题
echo "exec 子脚本"
# 使用子bash进程执行了./sub.sh文件的echo $var
./sub.sh    # 不会打印结果
# 用环境变量传参，让子bash进程获得参数
# 当创建一个子进程时，子进程会继承父进程的的环境变量
export var=abc    # export 定义的是环境变量，让当前进程多了一个环境变量
echo 修改了父进程的环境变量
./sub.sh

# 计算
echo 运算
a=3
b=2
c=`expr $a \* $b` # 把结果赋值给c
echo c=$c

# $优先级
echo "hello \$var"
echo "hello $var"
