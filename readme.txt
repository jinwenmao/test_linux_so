Linux �������
2008-11-10 15:09:00 li_zhongnan �Ķ��� 15275
��Ȩ����������Ϊ����ԭ�����£���ѭ CC 4.0 BY-SA ��ȨЭ�飬ת���븽��ԭ�ĳ������Ӻͱ�������
�������ӣ�https://blog.csdn.net/li_zhongnan/article/details/3267006
Linux �������

 

���ߣ�����   <li.zhongnan@hotmail.com>
 

1������

 

��̬���ǳ�����Ƴ��õļ��������ö�̬�������Ч�ļ��ٳ����С����ʡ�ռ䣬���Ч�ʣ����ӳ���Ŀ���չ�ԣ�����ģ�黯������Windows��Linux����ϵͳ�ж��ж�̬��ĸ��Windows�����Ϊ��̬���ӿ⣨Dynamic Link Library��DLL�������ļ���չ��Ϊ.dll��Linux����Ϊ����⼼����Shared Library������Ӧ�Ĺ�����ļ���չ��Ϊ.so��

 

����˼�壬��̬���ڳ������е�ʱ�򱻶�̬���ӡ������ھ���ʹ�ö�̬���ʱ��ȴ�����ֲ�ͬ�ķ�ʽ����ʽ���Ӻ���ʽ���ӡ���ʽ�����ڱ���/���ӽ׶���ɣ��ɱ���ϵͳ���ݶ�̬���ͷ�ļ��Ϳ��ļ����б�������ӣ��Ӷ�ȷ�������õĺ���ԭ�κ͵�ַ����ʽ������������API����ʵ�ּ��غ�ж�ع���⣬��ȡ�����ú�����ַ����ȡ������Ϣ�ȹ��ܡ�

 

2����ʽ���Ӿ���

��1����̬���ļ����룺dl_func.c

#include <stdio.h>
extern char name[];
int add(int a, int b)
{
        printf("calling add/n");
        printf("Hello, %s!/n", name);
        return a + b;
}

���ļ��е�add()����������������֮�ͣ����Ҵ�ӡ�ⲿ������ֵ�����ⲿ�����ɵ��ù��������������塣

 

��2���ͻ����������룺dl_demo1.c

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

������������ù������е�add()������������֮�󲢴�ӡ��ͬʱ�����������У�������name��ֵ���Ա���add()�����д�ӡ��

 

��3������������

���빲��⣺

gcc -o libdl_func.so -fPIC -rdynamic -shared dl_func.c
ѡ��-fPICָʾ����������������λ�ö����Ĵ��룬һ����Ҫ�Գ����ļ������亯������������������ļ��Ĵ��붼Ӧ���Դ�ѡ����б��룬ѡ��-rdynamicָʾ������������/���ӵ�Ϊ���������ļ�������Ҫʹ���ⲿ�����������Ҫ-sharedѡ�������������׳�������Ϣ��undefined reference to `name'����ʾ�����ҵ�name������

������������

gcc -o dl_demo1 -L./ -ldl_func dl_demo1.c
ѡ��-L./ ָʾ�������ڵ�ǰĿ¼��Ѱ�ҹ�����ļ���-ldl_funcָʾ��Ҫ�Ĺ�����ļ���Ϊlibdl_func.so��

���У�

./dl_demo1

�����

calling add
Hello, NHN XDBMS!
10 + 20 = 30

 

3����ʽ����API����

��ʽ������Ҫ�漰��4��API������ dlopen , dlerror , dlsym �� dlclose ������Щ����ԭ�ζ��������dlfcn.hͷ�ļ��С�

 

��1��void *dlopen(const char *file, int mode);

�ú�����������ָ��ģʽ��ָ���Ĺ���⣬����Ӱ�䵽�ڴ��У����ҷ��ؾ����
��һ��������ָ�����������ƣ�����������λ�ò���ָ���Ĺ���⡣
����������LD_LIBRARY_PATH�г����÷ֺż��������Ŀ¼��
���ļ�/etc/ld.so.cache���ҵ��Ŀ���б���ldconfigά����
��Ŀ¼usr/lib��
��Ŀ¼/lib��
����ǰĿ¼��
�ڶ���������ָ����δ򿪹���⡣
��RTLD_NOW����������е����к������ص��ڴ�
��RTLD_LAZY�����ƺ�����еĺ����ļ��ز�����ֱ������dlsym()ʱ������ĳ����

 

��2��void *dlsym(void *restrict handle, const char *restrict name);

�ú�������һ��ָ����name��ȷ����������ڵ��ָ�롣����dlsymʱ������dlopen()���صĹ�����phandle�Լ�����/����������Ϊ����������Ҫ���غ���/��������ڵ�ַ��

 

��3��char *dlerror(void);

dlerror ���� NULL ����һ��ָ�������������� ASCII �ַ�����ָ��

 

��4��int dlclose(void *handle);

�رվ������ȡ������Ŀ���ļ���ӳ��  

 

4����ʽ���Ӿ���

��1����̬���ļ����룺dl_func.c

����ʽ���ӵĴ�����ͬ��

 

��2���ͻ����������룺dl_demo.c

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

���������������name��ֵ���Ա���add()�����д�ӡ����������dlopen�������ع����libdl_func.so������dlclose�رվ��������dlerror��ȡ������Ϣ������dlsym��λ������е�add������Ȼ����øú���ִ�мӷ����㡣

 

��3������������

���빲��⣺

��ǰ���������뷽����ͬ��

������������

gcc -o dl_demo -fPIC -ldl dl_demo.c
���ڱ���name��Ҫ��������е�add()����ʹ�ã���˱���ʹ��ѡ��-fPIC��ѡ��-ldlָʾ��������Ҫ������libdl.so���ļ���

���У�

./dl_demo

�����

����ʽ���������������ͬ��

 

5������

��1�����������������ģ����˹������Ը�����ʹ���⣬�����Ҳ����ʹ�õ��ó����еı��������ڹ�����д�ӡ���������е�name����������name���ⲿ�����������������ӹ����ʱ��Ҫʹ��-sharedѡ�

��2�����˿��Թ������⣬�����Թ�������������dl_func.c�ж��������:

int num = 100;

��ô�����������������������ã�

int *d;
d = (int *)dlsym(dlh, "num");
printf("num = %d/n", *d);
    

