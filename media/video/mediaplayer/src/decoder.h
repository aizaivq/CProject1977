extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}
class PacketQueue
{
public:
	PacketQueue();
	void put(AVPacket *packet);
	void get(AVPacket *packet);
		int sizeMaxPacket = 30;

private:
	int sizePacket = 0;
	int sizeData = 0;
	AVPacketList *listFirst = NULL;
	AVPacketList *listLast = NULL;
	pthread_mutex_t     mLock;
	pthread_cond_t      mCondition;

};


class Decoder{
public:
	Decoder();
	void start();
	void setRes(char *res);
	bool decodeFrame(AVPacket *packet,AVFrame *frame );
	int getFrameRate();
	PacketQueue *mQueue;

private:
	AVCodecContext *video_context= NULL;
	AVCodecContext *audio_context= NULL;
	pthread_mutex_t     mLock;
	pthread_cond_t      mCondition;
	char *mRes;

};
