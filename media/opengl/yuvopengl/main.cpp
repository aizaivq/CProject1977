#include <GL/glut.h>
#include <stdio.h>
#include <OpenGL.h>
#include <stdlib.h>
#include <unistd.h>

extern OpenGL mOpenGL;

int main(int argc,char **argv)
{
  // openGL.CreateYUV420PWindow("main","yuv420p.yuv",100,100,1280,720,1280,720,DRAW_YUV420P);
   mOpenGL.CreateYUV420PWindow("main","yuyv.yuv",100,100,1280,720,640,480,DRAW_YUYV);

    while(true)
    {
        sleep(100000);
    } 
}
