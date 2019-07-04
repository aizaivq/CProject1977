2.库文件生成

1）源码

[cpp] view plain copy
print?

    //clTest.c  
      
    int add(int a, int b) {  
        return a + b;  
    }  

2)生成库文件

cd到clTest.c所在目录，输入命令

[html] view plain copy
print?

    gcc -shared -o libclTest.so clTest.c  

会在当前目录生成文件libclTest.so

3.主程序

1）源码

[cpp] view plain copy
print?

    //cclTest.c  
    #include <stdio.h>  
    #include <dlfcn.h>  
      
    int main(void) {  
        int(*myadd)(int a, int b);  
        void *handle;  
        handle = dlopen("./libclTest.so", RTLD_LAZY);  
        myadd = dlsym(handle, "add");  
      
        int result = myadd(1, 2);  
      
        dlclose(handle);  
      
        printf("%d\n", result);  
    }  


2）编译成可执行文件

cd到cclTest.c所在目录，输入命令

[html] view plain copy
print?

    gcc -o cclTest cclTest.c -ldl  

生成可执行文件cclTest
4.执行

cd到可执行文件cclTest所在目录

输入命令

[html] view plain copy
print?

    ./cclTest  




1. 静态库的生成

makefile命令的简介可参考：跟我一起写 Makefile。使用ar命令生成.a文件，可参考：Linux下动态库(.so)和静态库(.a)

# 1、准备工作，编译方式、目标文件名、依赖库路径的定义。
CC = g++
CFLAGS  := -Wall -O3 -std=c++0x 

# opencv 头文件和lib路径 
OPENCV_INC_ROOT = /usr/local/include/opencv 
OPENCV_LIB_ROOT = /usr/local/lib

OBJS = GenDll.o #.o文件与.cpp文件同名
LIB = libgendll.a # 目标文件名 

OPENCV_INC= -I $(OPENCV_INC_ROOT)

INCLUDE_PATH = $(OPENCV_INC)

LIB_PATH = -L $(OPENCV_LIB_ROOT)

# 依赖的lib名称
OPENCV_LIB = -lopencv_objdetect -lopencv_core -lopencv_highgui -lopencv_imgproc

all : $(LIB)

# 2. 生成.o文件 
%.o : %.cpp
    $(CC) $(CFLAGS) -c $< -o $@ $(INCLUDE_PATH) $(LIB_PATH) $(OPENCV_LIB) 

# 3. 生成静态库文件
$(LIB) : $(OBJS)
    rm -f $@
    ar cr $@ $(OBJS)
    rm -f $(OBJS)

tags :
     ctags -R *

# 4. 删除中间过程生成的文件 
clean:
    rm -f $(OBJS) $(TARGET) $(LIB)

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25
    26
    27
    28
    29
    30
    31
    32
    33
    34
    35
    36
    37
    38

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25
    26
    27
    28
    29
    30
    31
    32
    33
    34
    35
    36
    37
    38

2. 动态库的生成

第1、4步准备和收尾工作与静态库的保持一致，第2步和第3步所使用的命令稍有不同。

# 1、准备工作，编译方式、目标文件名、依赖库路径的定义。
CC = g++
CFLAGS  := -Wall -O3 -std=c++0x 

# opencv 头文件和lib路径 
OPENCV_INC_ROOT = /usr/local/include/opencv 
OPENCV_LIB_ROOT = /usr/local/lib

OBJS = GenDll.o #.o文件与.cpp文件同名
LIB = libgendll.so # 目标文件名 

OPENCV_INC= -I $(OPENCV_INC_ROOT)

INCLUDE_PATH = $(OPENCV_INC)

LIB_PATH = -L $(OPENCV_LIB_ROOT)

# 依赖的lib名称
OPENCV_LIB = -lopencv_objdetect -lopencv_core -lopencv_highgui -lopencv_imgproc

all : $(LIB)

# 2. 生成.o文件 
%.o : %.cpp
    $(CC) $(CFLAGS) -fpic -c $< -o $@ $(INCLUDE_PATH) $(LIB_PATH) $(OPENCV_LIB) 

# 3. 生成动态库文件
$(LIB) : $(OBJS)
    rm -f $@
    g++ -shared -o $@ $(OBJS)
    rm -f $(OBJS)

tags :
     ctags -R *

# 4. 删除中间过程生成的文件 
clean:
    rm -f $(OBJS) $(TARGET) $(LIB)

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25
    26
    27
    28
    29
    30
    31
    32
    33
    34
    35
    36
    37
    38

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25
    26
    27
    28
    29
    30
    31
    32
    33
    34
    35
    36
    37
    38

-fpic 和 -shared 命令可参考：Linux下动态库(.so)和静态库(.a)【注】这篇文章说可以使用ld命令生成.so文件，但我在测试时发会报错。
3. 动态库和静态库的调用

， 这两个的使用方法几乎没有区别。动态库的引用有显式和隐式两种，这里只说隐式调用。我使用main.cpp来测试生成的库文件， makefile如下：

CC = g++
CFLAGS  := -Wall -O3 -std=c++0x 

OPENCV_INC_ROOT = /usr/local/include/opencv 
OPENCV_LIB_ROOT = /usr/local/lib
MY_ROOT = ../

OPENCV_INC= -I $(OPENCV_INC_ROOT)
MY_INC = -I $(MY_ROOT)

EXT_INC = $(OPENCV_INC) $(MY_INC)

OPENCV_LIB_PATH = -L $(OPENCV_LIB_ROOT)
MY_LIB_PATH = -L $(MY_ROOT)

EXT_LIB = $(OPENCV_LIB_PATH) $(MY_LIB_PATH) 

OPENCV_LIB_NAME = -lopencv_objdetect -lopencv_highgui -lopencv_imgproc -lopencv_core 
MY_LIB_NAME = -lgendll

all:test

test:main.cpp
    $(CC) $(CFLAGS) main.cpp $(EXT_INC) $(EXT_LIB) $(MY_LIB_NAME) $(OPENCV_LIB_NAME) -o test

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25

    1
    2
    3
    4
    5
    6
    7
    8
    9
    10
    11
    12
    13
    14
    15
    16
    17
    18
    19
    20
    21
    22
    23
    24
    25

4. 注意事项：

1、在测试过程中，经常会报错：找不到.so文件。一种简单的解决方法如下：
在Linux终端输入如下命令：

export LD_LIBRARY_PATH=/home/shaoxiaohu/lib:LD_LIBRARY_PATH:
