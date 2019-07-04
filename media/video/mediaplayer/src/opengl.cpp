#include <stdio.h>  
#include <stdlib.h>  
#include <malloc.h>  
#include <string.h>  
#include <mediaplayer.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>

extern MediaPlayer  *player;


GLuint p;                  
GLuint id_y, id_u, id_v; 
GLuint textureUniformY, textureUniformU,textureUniformV;  
#define ATTRIB_VERTEX 3  
#define ATTRIB_TEXTURE 4  



OpenGL::OpenGL()
{
	pthread_mutex_init(&mLock, NULL);
	pthread_cond_init(&mCondition, NULL);
	mClock = new Clock();
	mClock->mCondition = &mCondition;
	mClock->mLock = &mLock;

}

char *textFileRead(char * filename)  
{  
	char *s = (char *)malloc(1024);  
	memset(s, 0, 1024);  
	FILE *infile = fopen(filename, "rb");  
	int len = fread(s, 1, 1024, infile);  
	fclose(infile);  
	s[len] = 0;  
	return s;  
}  
void display(void){  
	printf("opengl display\n");
	pthread_mutex_lock(&player->mOpenGL->mLock);

	AVFrame *frame = av_frame_alloc();
	AVPacket packet;
	av_init_packet(&packet);
	bool gotFrame = player->mDecoder->decodeFrame(&packet,frame);
	if(packet.stream_index == 0 && gotFrame)
	{
		printf("display wait\n");
					pthread_cond_signal(&player->mOpenGL->mCondition);

		pthread_cond_wait(&player->mOpenGL->mCondition, &player->mOpenGL->mLock);
		int pixel_w = frame->width;
		int pixel_h = frame->height;
		glClearColor(0.0,255,0.0,0.0);  
		glClear(GL_COLOR_BUFFER_BIT);  
		glActiveTexture(GL_TEXTURE0);  
		glBindTexture(GL_TEXTURE_2D, id_y);  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w, pixel_h, 0, GL_RED, GL_UNSIGNED_BYTE, frame->data[0]);   
		glUniform1i(textureUniformY, 0);      

		glActiveTexture(GL_TEXTURE1);  
		glBindTexture(GL_TEXTURE_2D, id_u);  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w/2, pixel_h/2, 0, GL_RED, GL_UNSIGNED_BYTE, frame->data[1]);         
		glUniform1i(textureUniformU, 1);  
		glActiveTexture(GL_TEXTURE2);  
		glBindTexture(GL_TEXTURE_2D, id_v);  
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, pixel_w/2, pixel_h/2, 0, GL_RED, GL_UNSIGNED_BYTE, frame->data[2]);      
		glUniform1i(textureUniformV, 2);     
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  
		glutSwapBuffers();  
	}
	else if(packet.stream_index == 1)
	{

	}

	pthread_mutex_unlock(&player->mOpenGL->mLock);


}

void initShaders()  
{  
	GLint vertCompiled, fragCompiled, linked;  

	GLint v, f;  
	const char *vs,*fs;  
        //Shader: step1  
	v = glCreateShader(GL_VERTEX_SHADER);  
	f = glCreateShader(GL_FRAGMENT_SHADER);  
        //Get source code  
	vs = textFileRead("Shader.vsh");  
	fs = textFileRead("Shader.fsh");  
        //Shader: step2  
	glShaderSource(v, 1, &vs,NULL);  
	glShaderSource(f, 1, &fs,NULL);  
        //Shader: step3  
	glCompileShader(v);  
        //Debug  
	glGetShaderiv(v, GL_COMPILE_STATUS, &vertCompiled);  
	glCompileShader(f);  
	glGetShaderiv(f, GL_COMPILE_STATUS, &fragCompiled);  

        //Program: Step1  
	p = glCreateProgram();   
        //Program: Step2  
	glAttachShader(p,v);  
	glAttachShader(p,f);   

	glBindAttribLocation(p, ATTRIB_VERTEX, "vertexIn");  
	glBindAttribLocation(p, ATTRIB_TEXTURE, "textureIn");  
        //Program: Step3  
	glLinkProgram(p);  
        //Debug  
	glGetProgramiv(p, GL_LINK_STATUS, &linked);    
        //Program: Step4  
	glUseProgram(p);  


        //Get Uniform Variables Location  
	textureUniformY = glGetUniformLocation(p, "tex_y");  
	textureUniformU = glGetUniformLocation(p, "tex_u");  
	textureUniformV = glGetUniformLocation(p, "tex_v");   

    #if TEXTURE_ROTATE  
	static const GLfloat vertexVertices[] = {  
		-1.0f, -0.5f,  
		0.5f, -1.0f,  
		-0.5f,  1.0f,  
		1.0f,  0.5f,  
	};      
    #else  
	static const GLfloat vertexVertices[] = {  
		-1.0f, -1.0f,  
		1.0f, -1.0f,  
		-1.0f,  1.0f,  
		1.0f,  1.0f,  
	};      
    #endif  

    #if TEXTURE_HALF  
	static const GLfloat textureVertices[] = {  
		0.0f,  1.0f,  
		0.5f,  1.0f,  
		0.0f,  0.0f,  
		0.5f,  0.0f,  
	};   
    #else  
	static const GLfloat textureVertices[] = {  
		0.0f,  1.0f,  
		1.0f,  1.0f,  
		0.0f,  0.0f,  
		1.0f,  0.0f,  
	};   
    #endif  
        //Set Arrays  
	glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertexVertices);  
        //Enable it  
	glEnableVertexAttribArray(ATTRIB_VERTEX);      
	glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, textureVertices);  
	glEnableVertexAttribArray(ATTRIB_TEXTURE);  


        //Init Texture  
	glGenTextures(1, &id_y);   
	glBindTexture(GL_TEXTURE_2D, id_y);      
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  

	glGenTextures(1, &id_u);  
	glBindTexture(GL_TEXTURE_2D, id_u);     
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  

	glGenTextures(1, &id_v);   
	glBindTexture(GL_TEXTURE_2D, id_v);      
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);  
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);  

}  

void drawLoop(int value){  
	display();  
	glutTimerFunc(player->mOpenGL->mFrequence, drawLoop, 0);  
}  


void OpenGL::get(Decoder *decoder,AVPacket *packet){
	decoder->mQueue->get(packet);

}




void OpenGL::createWindow()
{
	int ac = 1;
	char *av[] = {"main"};
	glutInit(&ac, av);    
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);  
	glutInitWindowPosition(100, 100);  
	glutInitWindowSize(1280, 720);  
	glutCreateWindow("");  
	glutTimerFunc(mFrequence, drawLoop, 0);   
	initShaders();  
	glutMainLoop();  
}



void Clock::start()
{
	while(1)
	{
		printf("clock while\n");
					printf("clock pthread_mutex_unlock2: mFrequence: %d\n",mFrequence);

		if(mFrequence > 0)
		{

			struct timeval tv;
		     gettimeofday(&tv,NULL);
		     long timeNow = tv.tv_sec*1000 + tv.tv_usec/1000;
		     long timeUse = timeNow - mLastTime;
    		//	printf("use time: %ld\n",timeUse);


    			if(timeUse >= 40)
    			{
    				    			mLastTime = timeNow;

			pthread_cond_signal(&player->mOpenGL->mCondition);
			pthread_cond_wait(&player->mOpenGL->mCondition, &player->mOpenGL->mLock);
    			}


			
			printf("clock pthread_mutex_unlock\n");

			pthread_mutex_unlock(&player->mOpenGL->mLock);
			printf("clock pthread_mutex_unlock1\n");
			usleep(1000);

//			usleep(mFrequence);
			printf("clock pthread_mutex_unlock2: mFrequence: %d\n",mFrequence);

		}
	}

}

void Clock::setFrequence(int f)
{
	mFrequence = 1000000/f;
}


void OpenGL::setFrequence(int f)
{
	printf("setFrequence f: %d\n",f);
	mFrequence = 5;
	mClock->setFrequence(f);
}
