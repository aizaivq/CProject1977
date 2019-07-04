#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
#include <decoder.h>
#include <pthread.h>

#define ATTRIB_VERTEX 3  
#define ATTRIB_TEXTURE 4  
class Clock{
public:
	void start();
	void setFrequence(int f);
	pthread_cond_t     * mCondition;
	pthread_mutex_t    * mLock;
private:
	long mLastTime;
	int mFrequence = -1;

};

class OpenGL{
public:
	OpenGL();
	Clock *mClock;
	void createWindow();
	void get(Decoder *decoder,AVPacket *packet);
	void setFrequence(int f);
	int mFrequence = -1;
	pthread_cond_t      mCondition;
	pthread_mutex_t     mLock;
private:
	GLuint p;                  
	GLuint id_y, id_u, id_v; 
	GLuint textureUniformY, textureUniformU,textureUniformV;  




};

