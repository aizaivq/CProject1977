#include <stdio.h>
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
}
#include <unistd.h>
#include <decoder.h>
#include <pthread.h>
#include <time.h>


int mVideoFameCount = 0;
int mAudioFrameCount = 0; 


PacketQueue::PacketQueue()
{
	pthread_mutex_init(&mLock, NULL);
	pthread_cond_init(&mCondition, NULL);
}

Decoder::Decoder()
{
	mQueue = new PacketQueue();
	printf("PacketQueue sizeMaxPacket: %d\n",mQueue->sizeMaxPacket);
	pthread_mutex_init(&mLock, NULL);
	pthread_cond_init(&mCondition, NULL);
}

void Decoder::setRes(char *res)
{
	mRes = res;
}

bool Decoder::decodeFrame(AVPacket *packet,AVFrame *frame )
{

	mQueue->get(packet);
	int gotFrame; 
	int decode_ret;
	if(packet->stream_index == 0)
	{
		time_t timep;
		struct tm *p;
		time(&timep);
		p = gmtime(&timep);
		printf("time: %d:%d:%d\n",8 + p->tm_hour,p->tm_min,p->tm_sec);
		decode_ret = avcodec_decode_video2(video_context,frame,&gotFrame,packet);
		printf("frame size: %dx%d",frame->width,frame->height);
	}
	else if(packet->stream_index == 1)
	{
		decode_ret = avcodec_decode_audio4(audio_context,frame,&gotFrame,packet);
	}

	return gotFrame;
}


void PacketQueue::get(AVPacket *packet)
{
	printf("PacketQueue get sizePacket: %d\n",sizePacket);
	pthread_mutex_lock(&mLock);
	AVPacketList *node;
	for (;;) {
		node = listFirst;
		if (node) {
			listFirst = node->next;
			if (!listFirst)
				listLast = NULL;
			sizePacket--;
			sizeData -= node->pkt.size + sizeof(*node);
			*packet = node->pkt;
			pthread_cond_signal(&mCondition);
			av_free(node);
			break;
		} 
		else {
			pthread_cond_wait(&mCondition, &mLock);
		}
		break;
	}
	pthread_mutex_unlock(&mLock);

}

void PacketQueue::put(AVPacket *packet)
{
	printf("PacketQueue put sizePacket: %d\n",sizePacket);
	printf("PacketQueue put sizeMaxPacket: %d\n",sizeMaxPacket);

	pthread_mutex_lock(&mLock);

	printf("0\n");

	if(sizePacket < sizeMaxPacket)
	{
			printf("1\n");

		AVPacketList *pPacket = (AVPacketList*)av_malloc(sizeof(AVPacketList));
		pPacket->pkt = *packet;
		pPacket->next = NULL;
		if (!listLast)
			listFirst = pPacket;
		else
			listLast->next = pPacket;
		listLast = pPacket;
		sizePacket++;
		sizeData += pPacket->pkt.size + sizeof(*pPacket);
	}
	else
	{
			printf("2\n");

		pthread_cond_signal(&mCondition);
					printf("20\n");

		pthread_cond_wait(&mCondition, &mLock);
							printf("21\n");

		pthread_mutex_unlock(&mLock);
									printf("22\n");

		put(packet);


	}
		printf("3\n");

	pthread_mutex_unlock(&mLock);

}



int Decoder::getFrameRate()
{
	AVCodec *video_codec = NULL;
	AVCodec *audio_codec = NULL;

	AVInputFormat *fmt = NULL;
	AVDictionary *opts  = NULL;
	AVFormatContext *mAVFormatContext = NULL;

	mAVFormatContext = avformat_alloc_context();
	int ret_avformat_open_input =  avformat_open_input(&mAVFormatContext, mRes, NULL, NULL);
	int ret_avformat_find_stream_info = avformat_find_stream_info(mAVFormatContext,NULL);
	av_dump_format(mAVFormatContext,0,mRes,0);

	return mAVFormatContext->streams[0]->avg_frame_rate.num;
}





void Decoder::start()
{
	AVCodec *video_codec = NULL;
	AVCodec *audio_codec = NULL;

	AVInputFormat *fmt = NULL;
	AVDictionary *opts  = NULL;
	AVFormatContext *mAVFormatContext = NULL;
	AVFrame *frame = NULL;
	AVPacket avpkt;
	int got_frame;

	mAVFormatContext = avformat_alloc_context();
	int ret_avformat_open_input =  avformat_open_input(&mAVFormatContext, mRes, NULL, NULL);
	printf("ret_avformat_open_input: %d\n",ret_avformat_open_input);
	int ret_avformat_find_stream_info = avformat_find_stream_info(mAVFormatContext,NULL);
	printf("ret_avformat_find_stream_info: %d\n",ret_avformat_find_stream_info);
	av_dump_format(mAVFormatContext,0,mRes,0);
	printf("duration: %d\n",mAVFormatContext->duration);

	printf("video fame rate num: %d\n",mAVFormatContext->streams[0]->avg_frame_rate.num);
	printf("audio fame rate num: %d\n",mAVFormatContext->streams[1]->codec->sample_rate);



	video_context = mAVFormatContext->streams[0]->codec;
	audio_context = mAVFormatContext->streams[1]->codec;


	video_codec = avcodec_find_decoder(video_context->codec_id);
	audio_codec = avcodec_find_decoder(audio_context->codec_id);
	
	


	int 	ret_video_avcodec_open2 = avcodec_open2(video_context, video_codec, &opts);
	int 	ret_audio_avcodec_open2 = avcodec_open2(audio_context, audio_codec, &opts);


	printf("ret_video_avcodec_open2: %d\n",ret_video_avcodec_open2);
	printf("ret_audio_avcodec_open2: %d\n",ret_audio_avcodec_open2);

#if 1
	while(1)
	{
		av_init_packet(&avpkt);
		frame = av_frame_alloc();
		int ret_av_read_frame = av_read_frame(mAVFormatContext, &avpkt);
						printf("video packet pts: %d\n",frame->pts);

		if(ret_av_read_frame < 0)
		{
			break;
		}

		if(avpkt.stream_index == 0)
		{
			mQueue->put(&avpkt);
		/*	printf("video avcodec_decode_video2\n");
			int ret_avcodec_decode_video2 = avcodec_decode_video2(video_context,frame,&got_frame,&avpkt);
			printf("ret_avcodec_decode_video2: %d\n",ret_avcodec_decode_video2);
			printf("got_frame: %d\n",got_frame);
			if(got_frame)
			{
				mVideoFameCount++;
				printf("video frame size: %dx%d\n",frame->width,frame->height );
				printf("video frame pts: %d\n",frame->pts);
				printf("video frame count: %d\n",mVideoFameCount);
				long timestamp = frame->pts * av_q2d(mAVFormatContext->streams[0]->time_base);
				printf("video frame real time: %ld\n",timestamp);
			}*/
			
		}
		else if(avpkt.stream_index == 1)
		{
			mQueue->put(&avpkt);
			/*int ret_avcodec_decode_audio4 = avcodec_decode_audio4(audio_context,frame,&got_frame,&avpkt);
			printf("ret_avcodec_decode_audio4: %d\n",ret_avcodec_decode_audio4);
			if(got_frame)
			{
				mAudioFrameCount++;
				printf("audio frame nb_samples: %d\n",frame->nb_samples);
				printf("audio frame pts: %d\n",frame->pts);

				printf("audio frame count: %d\n",mAudioFrameCount);
				
				long timestamp = frame->pts * av_q2d(mAVFormatContext->streams[1]->time_base);
				printf("audio frame real time: %ld\n",timestamp);


			}*/
		}

	}

#endif


}






