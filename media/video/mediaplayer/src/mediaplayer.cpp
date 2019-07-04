#include <pthread.h>
#include <mediaplayer.h>
void * threadLoopDecoder(void* p)
{
	Decoder *decoder = (Decoder *)p;
	decoder->start();
}

void * threadLoopClock(void *p)
{
	OpenGL *mOpenGL = (OpenGL *)p;
	mOpenGL->mClock->start();
}



MediaPlayer::MediaPlayer()
{
	mDecoder = new Decoder();
	mOpenGL = new OpenGL();

}

void MediaPlayer::setRes(char *res)
{
	mDecoder->setRes(res);
}

void MediaPlayer::start()
{
	pthread_t mDecodeThread;
	pthread_t mClockThread;

	mOpenGL->setFrequence(mDecoder->getFrameRate());


	pthread_create(&mDecodeThread, NULL, &threadLoopDecoder,mDecoder);
	pthread_detach(mDecodeThread);  
	pthread_join(mDecodeThread, NULL);  
	pthread_create(&mClockThread, NULL, &threadLoopClock,mOpenGL);
	pthread_detach(mClockThread);  
	pthread_join(mClockThread, NULL);

	mOpenGL->createWindow();

}