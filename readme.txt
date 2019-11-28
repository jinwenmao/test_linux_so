Linux 共享库编程
2008-11-10 15:09:00 li_zhongnan 阅读数 15275
版权声明：本文为博主原创文章，遵循 CC 4.0 BY-SA 版权协议，转载请附上原文出处链接和本声明。
本文链接：https://blog.csdn.net/li_zhongnan/article/details/3267006
Linux 共享库编程

 

作者：终南   <li.zhongnan@hotmail.com>
 

1、介绍

 

动态库是程序设计常用的技术，采用动态库可以有效的减少程序大小，节省空间，提高效率，增加程序的可扩展性，便于模块化管理。在Windows和Linux操作系统中都有动态库的概念。Windows将其称为动态链接库（Dynamic Link Library，DLL），其文件扩展名为.dll，Linux称其为共享库技术（Shared Library），相应的共享库文件扩展名为.so。

 

故名思义，动态库在程序运行的时候被动态链接。但是在具体使用动态库的时候却有两种不同的方式：隐式链接和显式链接。隐式链接在编译/链接阶段完成，由编译系统根据动态库的头文件和库文件进行编译和链接，从而确定待调用的函数原形和地址。显式链接则是利用API函数实现加载和卸载共享库，获取带调用函数地址，获取错误信息等功能。

 

2、隐式链接举例

（1）动态库文件代码：dl_func.c

#include <stdio.h>
extern char name[];
int add(int a, int b)
{
        printf("calling add/n");
        printf("Hello, %s!/n", name);
        return a + b;
}

该文件中的add()函数计算两个整数之和，并且打印外部变量的值，该外部变量由调用共享库的事例程序定义。

 

（2）客户端事例代码：dl_demo1.c

#include <stdio.h>
#include <dlfcn.h>

int add(int a, int b);

char name[100];
int main(int argc, char *argv[]) {
        int a = 10, b = 20;
        int c = 0;

        strcpy(name, "NHN XDBMS");
        c = add(a, b);
        printf("%d + %d = %d/n", a, b, c);

        return 0;
}

该事例程序调用共享库的中的add()函数计算两数之后并打印，同时在事例程序中，给变量name赋值，以便在add()函数中打印。

 

（3）编译与运行

编译共享库：

gcc -o libdl_func.so -fPIC -rdynamic -shared dl_func.c
选项-fPIC指示编译器将代码编译成位置独立的代码，一般需要以程序文件共享其函数或变量给其他程序文件的代码都应该以此选项进行编译，选项-rdynamic指示编译器所编译/链接的为共享库程序文件。由于要使用外部变量，因此需要-shared选项，否则编译器会抛出错误信息：undefined reference to `name'，表示不能找到name变量。

编译事例程序：

gcc -o dl_demo1 -L./ -ldl_func dl_demo1.c
选项-L./ 指示编译器在当前目录下寻找共享库文件，-ldl_func指示需要的共享库文件名为libdl_func.so。

运行：

./dl_demo1

输出：

calling add
Hello, NHN XDBMS!
10 + 20 = 30

 

3、显式链接API函数

显式链接主要涉及到4个API函数（ dlopen , dlerror , dlsym 和 dlclose ），这些函数原形定义包含在dlfcn.h头文件中。

 

（1）void *dlopen(const char *file, int mode);

该函数用来按照指定模式打开指定的共享库，将其影射到内存中，并且返回句柄。
第一个参数：指定共享库的名称，将会在下面位置查找指定的共享库。
－环境变量LD_LIBRARY_PATH列出的用分号间隔的所有目录。
－文件/etc/ld.so.cache中找到的库的列表，用ldconfig维护。
－目录usr/lib。
－目录/lib。
－当前目录。
第二个参数：指定如何打开共享库。
－RTLD_NOW：将共享库中的所有函数加载到内存
－RTLD_LAZY：会推后共享库中的函数的加载操作，直到调用dlsym()时方加载某函数

 

（2）void *dlsym(void *restrict handle, const char *restrict name);

该函数返回一个指向由name所确定的请求入口点的指针。调用dlsym时，利用dlopen()返回的共享库的phandle以及函数/变量名称作为参数，返回要加载函数/变量的入口地址。

 

（3）char *dlerror(void);

dlerror 返回 NULL 或者一个指向描述最近错误的 ASCII 字符串的指针

 

（4）int dlclose(void *handle);

关闭句柄并且取消共享目标文件的映射  

 

4、显式链接举例

（1）动态库文件代码：dl_func.c

与隐式链接的代码相同。

 

（2）客户端事例代码：dl_demo.c

#include <stdio.h>
#include <dlfcn.h>

char name[100];
int main(int argc, char *argv[]) {
        int a = 10, b = 20;
        int c = 0;
        void *dlh = NULL;
        int (*add)();

        strcpy(name, "NHN XDBMS");
        if((dlh = dlopen("libdl_func.so", RTLD_LAZY)) == NULL) {
                fprintf (stderr, "***DL ERROR: %s./n", dlerror ());
                return 1;
        }
        if((add = (int (*)())dlsym(dlh, "add")) == NULL) {
                fprintf (stderr, "***DL ERROR: %s./n", dlerror ());
                return 1;
        }

        c = add(a, b);
        printf("%d + %d = %d/n", a, b, c);
        dlclose(dlh);
        return 0;
}

该事例程序给变量name赋值，以便在add()函数中打印。程序利用dlopen函数加载共享库libdl_func.so，利用dlclose关闭句柄，利用dlerror获取错误信息，利用dlsym定位共享库中的add函数，然后调用该函数执行加法计算。

 

（3）编译与运行

编译共享库：

与前述共享库编译方法相同。

编译事例程序：

gcc -o dl_demo -fPIC -ldl dl_demo.c
由于变量name需要被共享库中的add()函数使用，因此必须使用选项-fPIC。选项-ldl指示编译器需要用来到libdl.so库文件。

运行：

./dl_demo

输出：

与隐式链接事例的输出相同。

 

5、其他

（1）如事例中所给出的，除了共享库可以给别人使用外，共享库也可以使用调用程序中的变量，如在共享库中打印事例程序中的name。不过由于name在外部定义和声明因此在链接共享库时需要使用-shared选项。

（2）除了可以共享函数外，还可以共享变量，如果在dl_func.c中定义个变量:

int num = 100;

那么可以在事例程序中这样调用：

int *d;
d = (int *)dlsym(dlh, "num");
printf("num = %d/n", *d);
    

