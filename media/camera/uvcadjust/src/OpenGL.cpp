#include <stdio.h>  

#include <sys/time.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <malloc.h>  
#include <string.h>  
#include <pthread.h>
#include <OpenGL.h>
#include <unistd.h>

#include <string>
using namespace std;

OpenGL mOpenGL;

const GLchar* SHADERV = "attribute vec4 vertexIn;\n"
"attribute vec2 textureIn;\n" 
"varying vec2 textureOut;\n"  
"void main(void)\n"  
"{\n"
"gl_Position = vertexIn;\n"   
"textureOut = textureIn;\n" 
"}";
const GLchar* SHADERF = "varying vec2 textureOut;\n"  
"uniform sampler2D tex_y;\n"
"uniform sampler2D tex_u;\n"
"uniform sampler2D tex_v;\n"
"void main(void)\n"
"{\n"
"vec3 yuv;\n"
"vec3 rgb;\n"     
"yuv.x = texture2D(tex_y, textureOut).r;\n"  
"yuv.y = texture2D(tex_u, textureOut).r - 0.5;\n"  
"yuv.z = texture2D(tex_v, textureOut).r - 0.5;\n" 
"rgb = mat3( 1,1,1,0,-0.39465,2.03211,1.13983,-0.58060,0)*yuv;\n"     
"gl_FragColor = vec4(rgb, 1);\n"
"}";


void DrawCallback()
{

}

void Draw(int value)
{
	glutTimerFunc(30, Draw, 0);  

	if(mOpenGL.mBuff)
	{
		int size = mOpenGL.mDataWidth * mOpenGL.mDataHeight * 3 / 2;
		int uSizeHeight = mOpenGL.mDataHeight / 2;
		int vSizeHeight = mOpenGL.mDataHeight / 2;

		unsigned char *plane[3];


		switch(mOpenGL.mDraw)
		{
			case DRAW_YUV420P:
			{
				size = mOpenGL.mDataWidth * mOpenGL.mDataHeight * 3 / 2;
				uSizeHeight = mOpenGL.mDataHeight / 2;
				vSizeHeight = mOpenGL.mDataHeight / 2;

				plane[0] = mOpenGL.mBuff;  
				plane[1] = plane[0] + mOpenGL.mDataWidth * mOpenGL.mDataHeight;  
				plane[2] = plane[1] + mOpenGL.mDataWidth * mOpenGL.mDataHeight/4;  

				break;
			}
			case DRAW_YUYV:
			{
				size = mOpenGL.mDataWidth * mOpenGL.mDataHeight * 2;
				uSizeHeight = mOpenGL.mDataHeight;
				vSizeHeight = mOpenGL.mDataHeight;

				unsigned char YUV422P[size];
				memset(YUV422P,0,size);
				for(int i = 0;i < size ;i++)
				{
					if(i % 2 == 0)
					{	
						/*			
						if(i > uSizeHeight * vSizeHeight * 3 / 2)
						{
							if(mOpenGL.mBuff[i]   * 2 >= 255)
							{
								mOpenGL.mBuff[i] = 255;
							}
							else
							{
								mOpenGL.mBuff[i] = mOpenGL.mBuff[i]   * 2;
							}
						}
						*/
						YUV422P[i / 2] = mOpenGL.mBuff[i];
					}
					else if(i % 4 == 1)
					{
						YUV422P[mOpenGL.mDataWidth * mOpenGL.mDataHeight + i / 4] = mOpenGL.mBuff[i];  

					}
					else if(i % 4 == 3)
					{
						YUV422P[mOpenGL.mDataWidth * mOpenGL.mDataHeight * 3 / 2 + i / 4] = mOpenGL.mBuff[i];  
					}
				}
				plane[0] = YUV422P;  
				plane[1] = plane[0] + mOpenGL.mDataWidth * mOpenGL.mDataHeight ;  
				plane[2] = plane[1] + mOpenGL.mDataWidth * mOpenGL.mDataHeight / 2;  
			/*
				FILE * file = fopen("data.yuv","w");
				if(file)
				{
					fwrite(plane[0],sizeof(char),mOpenGL.mDataWidth * mOpenGL.mDataHeight,file);
					fwrite(plane[1],sizeof(char),mOpenGL.mDataWidth * mOpenGL.mDataHeight / 2,file);
					fwrite(plane[2],sizeof(char),mOpenGL.mDataWidth * mOpenGL.mDataHeight / 2,file);
				}
				fclose(file);*/
				break;
			}
		}
		sleep(1);

		glClearColor(0.0,255,0.0,0.0);  
		glClear(GL_COLOR_BUFFER_BIT);  
		glActiveTexture(GL_TEXTURE0);  
		glBindTexture(GL_TEXTURE_2D, mOpenGL.mIdY);  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mOpenGL.mDataWidth, mOpenGL.mDataHeight, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[0]);   
		glUniform1i(mOpenGL.mTextureUniformY, 0);      
		glActiveTexture(GL_TEXTURE1); 
		glBindTexture(GL_TEXTURE_2D, mOpenGL.mIdU);  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mOpenGL.mDataWidth / 2, uSizeHeight , 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[1]);         
		glUniform1i(mOpenGL.mTextureUniformU, 1);  
		glActiveTexture(GL_TEXTURE2);  
		glBindTexture(GL_TEXTURE_2D, mOpenGL.mIdV);  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, mOpenGL.mDataWidth / 2, vSizeHeight , 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, plane[2]);     
		glUniform1i(mOpenGL.mTextureUniformV, 1);    

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  
		glutSwapBuffers();  
		mOpenGL.mBuff = NULL;

	}
	else
	{
		printf("buff NULL\n");
	}
}

void *ThreadCallOpenGL(void *arg)
{
	int x = mOpenGL.mWinX;
	int y = mOpenGL.mWinY;
	int winWidth = mOpenGL.mWinWidth;
	int winHeight = mOpenGL.mWinHeight;
	int dataWidth = mOpenGL.mDataWidth;
	int dataHeight = mOpenGL.mDataHeight;
	const char * title = mOpenGL.mTitle;
	const char * dataPath = mOpenGL.mDataPath;
	printf("path mTitle x y winWidth winHeight dataWidth dataHeight: %s %s %d %d %d %d %d %d\n",dataPath,title,x,y,winWidth,winHeight,dataWidth,dataHeight);
	int ac = 1;
	char av0[] = "main";
	char *av[] = {av0};
	glutInit(&ac,av);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  
	printf("x: %d\n",x);
	printf("y: %d\n",y);
	glutInitWindowPosition(x, y);  
	glutInitWindowSize(winWidth, winHeight);  
	glutCreateWindow(title);

	GLint vertCompiled, fragCompiled, linked;  
	GLint v, f;  
	const char *vs,*fs;  
	v = glCreateShader(GL_VERTEX_SHADER);  
	f = glCreateShader(GL_FRAGMENT_SHADER);  
	glShaderSource(v, 1, &SHADERV,NULL);  
	glShaderSource(f, 1, &SHADERF,NULL);  
	glCompileShader(v);  
	glGetShaderiv(v, GL_COMPILE_STATUS, &vertCompiled);  
	glCompileShader(f);  
	glGetShaderiv(f, GL_COMPILE_STATUS, &fragCompiled);  
	GLuint p = glCreateProgram();   
	glAttachShader(p,v);  
	glAttachShader(p,f);   
	glBindAttribLocation(p, 3, "vertexIn");  
	glBindAttribLocation(p, 4, "textureIn");  
	glLinkProgram(p);  
	glGetProgramiv(p, GL_LINK_STATUS, &linked);    
	glUseProgram(p);  
	mOpenGL.mTextureUniformY = glGetUniformLocation(p, "tex_y");  
	mOpenGL.mTextureUniformU = glGetUniformLocation(p, "tex_u");  
	mOpenGL.mTextureUniformV = glGetUniformLocation(p, "tex_v");   
	static const GLfloat vertexVertices[] = {  
		-1.0f, -1.0f,  
		1.0f, -1.0f,  
		-1.0f,  1.0f,  
		1.0f,  1.0f,  
	};      
	static const GLfloat textureVertices[] = {  
		0.0f,  1.0f,  
		1.0f,  1.0f,  
		0.0f,  0.0f,  
		1.0f,  0.0f,  
	};   
	glVertexAttribPointer(3, 2, GL_FLOAT, 0, 0, vertexVertices);  
	glEnableVertexAttribArray(3);      
	glVertexAttribPointer(4, 2, GL_FLOAT, 0, 0, textureVertices);  
	glEnableVertexAttribArray(4);  
	
	glGenTextures(1, &mOpenGL.mIdY);   
	glBindTexture(GL_TEXTURE_2D, mOpenGL.mIdY);      
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glGenTextures(1, &mOpenGL.mIdU);  
	glBindTexture(GL_TEXTURE_2D, mOpenGL.mIdU);     
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	glGenTextures(1, &mOpenGL.mIdV);   
	glBindTexture(GL_TEXTURE_2D, mOpenGL.mIdV);      
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  
	
	glutDisplayFunc(DrawCallback);
	glutTimerFunc(1, Draw, 0);   


	glutMainLoop();
}

void OpenGL::CreateYUV420PWindow(const char *title,const char * path,int x,int y,int winWidth,int winHeight,int dataWidth,int dataHeight,TYPE_DRAW type)
{
	mDraw = type;
	mWinX = x;
	mWinY = y;
	mWinWidth = winWidth;
	mWinHeight = winHeight;
	mDataWidth = dataWidth;
	mDataHeight = dataHeight;
	mTitle = title;
	mDataPath = path;
	pthread_t thread;
	int ret = pthread_create(&thread, NULL, &ThreadCallOpenGL, NULL);
	printf("pthread_create ret: %d\n",ret);
	pthread_detach(thread);  
	pthread_join(thread, NULL);  
}
