# 编译
target = b.out
srcs = $(shell ls *.c)
objs = $(srcs:.c=.o)

# 声明.d文件，让程序去找依赖文件
deps:= $(srcs:.c=.d)

all: $(target)
$(target): $(objs)
	$(CC) -o $@ $^

# 子makefile：make -f .d，继续找.d文件，找到后make一下获得依赖关系
-include $(deps)

# 将gcc -MM .c的输出(a.o: a.c a.h)重定向到.d文件，重定向创造了.d文件
%.d: %.c
	gcc -MM $< > $@

%.o:%.c
	gcc -c $< -o $@

# 清理
clean: 
	-rm $(target) *.o *.out *.d -rf

# 测试
test:
	@echo source file is: $(srcs)
	@echo target file is: $(objs)
	@echo depend file is: $(deps)
