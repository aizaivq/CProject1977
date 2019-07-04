#include <stdio.h>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}
#include <mediaplayer.h>
#include <pthread.h>

#include <GL/gl.h>
#include <GL/glext.h>
#include <GL/glut.h>

MediaPlayer  *player;

int main(int argc,char **argv)
{
	if(argc > 1)
	{
		printf("argv: %s\n",argv[1]);
	}
	else
	{
		printf("exit\n");
		return 0;
	}
	avcodec_register_all();
	av_register_all();

	player = new MediaPlayer();
	player->setRes(argv[1]);
	player->start();


	return 1;
}
