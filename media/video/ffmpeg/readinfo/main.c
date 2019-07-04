#include<stdio.h>
#include<libavcodec/avcodec.h>
#include<libavformat/avformat.h>
#include<libavutil/opt.h>
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
//		AVFormatContext *mAVFormatContext = avformat_alloc_context();
		AVFormatContext *mAVFormatContext = NULL;



	AVInputFormat *fmt = NULL;
	AVDictionary *opts = NULL;
//	int ret_avformat_open_input =  avformat_open_input(&mAVFormatContext, argv[1], fmt, &opts);
	int ret_avformat_open_input =  avformat_open_input(&mAVFormatContext, argv[1], NULL, NULL);
	printf("mAVFormatContext duration: %d\n",mAVFormatContext->duration);

	int ret_avformat_find_stream_info = avformat_find_stream_info(mAVFormatContext,NULL);
	av_dump_format(mAVFormatContext,0,argv[1],0);

	printf("mAVFormatContext duration: %d\n",mAVFormatContext->duration);

	/*int vId = mAVFormatContext->streams[0]->codec->codec_id;
	printf("vId: %d\n",vId);

	int i = 0;
	    for (i = 0; i < mAVFormatContext->nb_streams; i++)
	    {
 	       if (mAVFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
 	       	{
 	       		printf("video stream index: %d\n",i);
 	       	}
}
	context = mAVFormatContext->streams[0]->codec;
		codec = avcodec_find_decoder(context->codec_id);
		printf("context->codec_id: %d\n",context->codec_id);
	printf("ret_avformat_find_stream_info: %d\n",ret_avformat_find_stream_info);
	printf("ret_avformat_open_input: %d\n",ret_avformat_open_input);

     int avcodec_open2_ret = 0;
	//avcodec_open2_ret = avcodec_open2(context, codec, &opts);
	//printf("avcodec_open2_ret: %d\n",avcodec_open2_ret);

	printf("mAVFormatContext->streams[0]->codec->width: %d\n",mAVFormatContext->streams[0]->codec->width);*/
	return 1;
}
