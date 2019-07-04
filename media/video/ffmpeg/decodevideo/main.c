#include<stdio.h>
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libavutil/opt.h>
#define INBUF_SIZE 4096


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
	AVCodecContext *context= NULL;
	AVCodec *codec = NULL;
	AVInputFormat *fmt = NULL;
	AVDictionary *opts  = NULL;
	AVFormatContext *mAVFormatContext = avformat_alloc_context();
	avformat_open_input(&mAVFormatContext, argv[1], fmt, &opts);
	int ret_avformat_find_stream_info = avformat_find_stream_info(mAVFormatContext,NULL);
	printf("ret_avformat_find_stream_info: %d\n",ret_avformat_find_stream_info);
				//	codec = avcodec_find_decoder(AV_CODEC_ID_H264);

			
		printf("AV_CODEC_ID_H264: %d\n",AV_CODEC_ID_H264);

//		context = avcodec_alloc_context3(codec);

		context = mAVFormatContext->streams[0]->codec;
		printf("codec id: %d\n",context->codec_id);
							printf("context->codec_id: %d\n",context->codec_id);

							codec = avcodec_find_decoder(context->codec_id);
								if(!codec)
				{
					printf("codec == null\n");
				}
				else
				{
					printf("codec != null\n");
				}

		context->codec_id = AV_CODEC_ID_H264;
				printf("codec id: %d\n",codec->id);
				printf("codec type: %d\n",codec->type);
				printf("context coder_type: %d\n",context->coder_type);
				printf("context codec_id: %d\n",context->codec_id);



	//avformat_find_stream_info(mAVFormatContext,NULL);

//			codec = avcodec_find_decoder(AV_CODEC_ID_H264);




	//printf("codec_id: %d\n" + context->codec_id);
//	codec = avcodec_find_decoder(context->codec_id);
	//av_dump_format(mAVFormatContext,0,argv[1],0);
	int avcodec_open2_ret = 0;
	avcodec_open2_ret = avcodec_open2(context, codec, &opts);
			printf("mAVFormatContext->streams[0]->codec->width: %d\n",mAVFormatContext->streams[0]->codec->width);

	printf("avcodec_open2_ret: %d\n",avcodec_open2_ret);
	AVPacket avpkt;
	int decode_ret;
	AVFrame *frame;
	int got_frame;
	int readframe_ret = -1;
	decode_ret = -1;
	int count = 0;



#if 1
	while(1)
	{
		 	av_init_packet(&avpkt);
		readframe_ret = av_read_frame(mAVFormatContext, &avpkt);
		if(readframe_ret < 0)
			break;
		printf("readframe_ret: %d\n",readframe_ret);
		printf("avpkt->stream_index=%d\n",avpkt.stream_index);
		if(avpkt.stream_index == 0)
		{
			count++;
					frame = av_frame_alloc();

			decode_ret = avcodec_decode_video2(context,frame,&got_frame,&avpkt);
			if(decode_ret < 0)
				break;
//							printf("video key frame: %d\n", frame->key_frame);
			printf("read frame width: %d\n",frame->width);
			printf("read frame height: %d\n",frame->height);


			printf("decode ret: %d\n",decode_ret);
			FILE * fd = fopen("test.yuv","w");
			fwrite(frame->data[0],1,1280*720,fd);
			fwrite(frame->data[1],1,1280*720 /4,fd);
			fwrite(frame->data[2],1,1280*720 /4,fd);
			fclose(fd);
		}
		av_frame_free(&frame);
		printf("count: %d\n",count);
	}
#endif
	return 1;
}

