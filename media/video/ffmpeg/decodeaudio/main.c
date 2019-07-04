#include <stdio.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libavresample/internal.h>

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
	AVCodec *codec;
	codec = avcodec_find_decoder(AV_CODEC_ID_AAC);
	AVFormatContext *mAVFormatContext = avformat_alloc_context();
	AVInputFormat *fmt = NULL;
	AVDictionary *opts  = NULL;
	avformat_open_input(&mAVFormatContext, argv[1], fmt, &opts);
	avformat_find_stream_info(mAVFormatContext,NULL);
	context = mAVFormatContext->streams[1]->codec;
	av_dump_format(mAVFormatContext,0,argv[1],0);
	int avcodec_open2_ret = 0;
	avcodec_open2_ret = avcodec_open2(context, codec, &opts);
	printf("avcodec_open2_ret: %d\n",avcodec_open2_ret);
	AVPacket avpkt;
 	av_init_packet(&avpkt);
	int decode_ret;
	AVFrame *frame;
	int got_frame;
	int readframe_ret = -1;
	decode_ret = -1;
	int count = 0;
	FILE * fd = fopen("test.pcm","w");
    	AVAudioResampleContext *mAVAudioResampleCtx = avresample_alloc_context();
	printf("channels layout: %d\n",context->channel_layout);
	printf("sample_rate: %d\n",context->sample_rate);
	av_opt_set_int(mAVAudioResampleCtx, "in_channel_layout",    context->channel_layout, 0);
    	av_opt_set_int(mAVAudioResampleCtx, "in_sample_rate",   context->sample_rate , 0);
    	av_opt_set_int(mAVAudioResampleCtx, "in_sample_fmt", context->sample_fmt, 0);
    	av_opt_set_int(mAVAudioResampleCtx, "out_channel_layout",    AV_CH_LAYOUT_MONO, 0);
    	av_opt_set_int(mAVAudioResampleCtx, "out_sample_rate",       8000, 0);
    	av_opt_set_int(mAVAudioResampleCtx, "out_sample_fmt", AV_SAMPLE_FMT_S16, 0);
	int resampleopen_ret = avresample_open(mAVAudioResampleCtx);
	printf("resampleopen_ret: %d\n",resampleopen_ret);
#if 1
	while(1)
	{
		frame = av_frame_alloc();
		readframe_ret = av_read_frame(mAVFormatContext, &avpkt);
		if(readframe_ret < 0)
			break;
		printf("readframe_ret: %d\n",readframe_ret);
		printf("avpkt->stream_index=%d\n",avpkt.stream_index);
		if(avpkt.stream_index == 1)
		{
			count++;
			decode_ret = avcodec_decode_audio4(context,frame,&got_frame,&avpkt);
			if(decode_ret < 0)
				break;
			printf("frame size: %d\n",mAVFormatContext->streams[1]->codec->frame_size);
			printf("decode ret: %d\n",decode_ret);
			printf("frame->linesize[0]: %d\n",frame->linesize[0]);
			printf("sample_rate: %d\n",context->sample_rate);
			printf("channels: %d\n",context->channels);
			if(got_frame)
			{
				 char* output = (char*) malloc(48000 * 2 * 2*10);
                		int plane_size;
                              printf("frame->nb_samples: %d\n",frame->nb_samples);
		               int av_samples_get_buffer_size_ret =  av_samples_get_buffer_size(&plane_size, context->channels,
                                                           frame->nb_samples,
                                                           context->sample_fmt, 1);
               		 printf("av_samples_get_buffer_size: %d",av_samples_get_buffer_size_ret);
 		                int nb_sample = avresample_convert(mAVAudioResampleCtx, frame->data, plane_size, frame->nb_samples,
                                                        frame->data, plane_size, frame->nb_samples);
		                printf("nb_sample: %d\n",nb_sample);
		         	 int outLenInBytes = nb_sample * 2;
		                printf("plane_size: %d\n",plane_size);
		                memcpy(output, frame->data[0], outLenInBytes);
				fwrite(output, 1, outLenInBytes, fd);
			printf("audio pts: %d\n",avpkt.pts);
			}
		}
		av_frame_free(&frame);
		printf("count: %d\n",count);
	}
#endif
	return 1;
}

