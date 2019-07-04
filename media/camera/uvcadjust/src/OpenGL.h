#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>
enum TYPE_DRAW
{
	DRAW_YUV420P,
	DRAW_YUYV
};

class OpenGL
{
private:
	
public:
	GLuint mIdY,mIdU,mIdV;
	GLuint mTextureUniformY,mTextureUniformU,mTextureUniformV;
	unsigned char *mBuff;
	int mDraw;
	int mWinX;
	int mWinY;
	int mWinWidth;
	int mWinHeight;
	int mDataWidth;
	int mDataHeight;
	const char *mDataPath;
	const char *mTitle;
	void CreateYUV420PWindow(const char *title,const char * path,int x,int y,int winWidth,int winHeight,int dataWidth,int dataHeight,TYPE_DRAW type);

};