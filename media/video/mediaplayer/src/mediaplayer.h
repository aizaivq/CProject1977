#include <opengl.h>
class MediaPlayer{
public:
	OpenGL * mOpenGL;
	Decoder * mDecoder;
	MediaPlayer();
	void setRes(char * res);
	void start();
private:
};