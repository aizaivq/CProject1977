#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alhelpers.h>
#include <AL/alut.h>
#include <alutInternal.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <libswresample/swresample.h>
#include <libavresample/internal.h>

int main (int argc, char **argv)
{
 if(argc < 2)
 {
  fprintf(stderr, "Usage: %s [-device <name>] <filename>\n", argv[0]);
  return 1;
}
if (!alutInit (&argc, argv))
{
  ALenum error = alutGetError ();
  fprintf (stderr, "%s\n", alutGetErrorString (error));
  exit (EXIT_FAILURE);
}
if(InitAL(&argv, &argc) != 0)
  return 1;

if (argc != 2)
{
  fprintf (stderr, "usage: playfile <fileName>\n");
  alutExit ();
  exit (EXIT_FAILURE);
}
ALuint source = 0;
ALuint mul_buf[1024 * 100];
ALint status;
ALuint buffer;
ALfloat *paramFrequency;
ALfloat *paramChannel;
ALfloat *paramSize;
ALfloat *paramBits;
ALint queued;
queued = 0;
ALint processed;
ALuint tmp_buff;

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

int fcount = 0;
source = 0;
alGenSources (1, &source);

  #if 1
while(1)
{

  
  frame = av_frame_alloc();
  readframe_ret = av_read_frame(mAVFormatContext, &avpkt);
  if(readframe_ret < 0)
    break;
  if(avpkt.stream_index == 1)
  {
    count++;
    decode_ret = avcodec_decode_audio4(context,frame,&got_frame,&avpkt);
    if(decode_ret < 0)
      break;
    if(got_frame)
    {
     
      char* output = (char*) malloc(48000 * 2 * 2);
      int plane_size;
      int av_samples_get_buffer_size_ret =  av_samples_get_buffer_size(&plane_size, context->channels,
       frame->nb_samples,
       context->sample_fmt, 1);
      int nb_sample = avresample_convert(mAVAudioResampleCtx, frame->data, plane_size, frame->nb_samples,
        frame->data, plane_size, frame->nb_samples);
      int outLenInBytes = nb_sample * 2;
      memcpy(output, frame->data[0], outLenInBytes);
      fwrite(output, 1, outLenInBytes, fd);
      fcount++;
            //]alutCreateBufferFromFileImage (const ALvoid *data, ALsizei length);
      ALuint tmp_buff;
              //                    buffer = alutCreateBufferFromFile ("testm.raw");

      buffer = alutCreateBufferFromFileImage(output,outLenInBytes);
      mul_buf[queued] = buffer;
      printf("queued: %d\n",queued);

      ALfloat *paramFrequency;
      ALfloat *paramChannel;
      ALfloat *paramSize;
      ALfloat *paramBits;
      alGetBufferi(mul_buf[queued],AL_FREQUENCY,&paramFrequency);
      printf("alGetBufferi AL_FREQUENCY: %d\n",paramFrequency);
      alGetBufferi(mul_buf[queued],AL_CHANNELS,&paramChannel);
      printf("alGetBufferi AL_CHANNELS: %0x\n",paramChannel);
      alGetBufferi(mul_buf[queued],AL_SIZE,&paramSize);
      printf("alGetBufferi AL_SIZE: %d\n",paramSize);
      alGetBufferi(mul_buf[queued],AL_BITS,&paramBits);
      printf("alGetBufferi AL_BITS: %d\n",paramBits);
      ALuint bufCount = sizeof(mul_buf[queued])/sizeof(ALuint) ;  
      printf("alGetBufferi bufCount: %d\n",bufCount);

       //       alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);  
              //                                                alSourcei (source, AL_BUFFER, mul_buf[queued]);
  //alSourcePlay (source);
      
     /* if(queued > 1000)
      {
       alGenSources (1, &source);
       alSourceQueueBuffers(source,queued,mul_buf);
       alSourcePlay (source);
       while(1)
       {
        alutSleep (0.1f);
        printf("status: %0x\n",status);
        printf("play processed:%d\n",processed);

      }*/
    queued++;

  }
}
av_frame_free(&frame);
printf("count: %d\n",count);
}
printf("play queued:%d\n",queued);
  alGenSources (1, &source);
       alSourceQueueBuffers(source,queued,mul_buf);
       alSourcePlay (source);
do
{
  printf("play AL_SOURCE_STATE:%0x\n",status);

  alutSleep (0.1f);
  alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);  
  printf("play processed:%d\n",processed);

  alGetSourcei (source, AL_SOURCE_STATE, &status);
}
while (status == AL_PLAYING);

#endif


return EXIT_SUCCESS;
}
