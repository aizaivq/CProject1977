#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alhelpers.h>
#include <AL/alut.h>
#include <alutInternal.h>
/*
 * This program loads and plays a variety of files.
 */

static void
playFile (const char *fileName)
{
  FILE * fd;
  unsigned char * buf;
  fd = fopen(fileName,"r");
  fseek(fd,0,SEEK_END);
  int size = ftell(fd);
  printf("size: %d\n",size);
  read(fd,buf,size);
  ALuint buffer;
  ALuint source;
  ALenum error;
  ALint status;
  /* Create an AL buffer from the given sound file. */
  buffer = alutCreateBufferFromFile (fileName);
  if (buffer == AL_NONE)
  {
    error = alutGetError ();
    fprintf (stderr, "Error loading file: '%s'\n",
     alutGetErrorString (error));
    alutExit ();
    exit (EXIT_FAILURE);
  }
  /* Generate a single source, attach the buffer to it and start playing. */
  source = 0;
  alGenSources (1, &source);
  ALfloat *paramFrequency;
  ALfloat *paramChannel;
  ALfloat *paramSize;
  ALfloat *paramBits;
  alGetBufferi(buffer,AL_FREQUENCY,&paramFrequency);
  printf("AL_FREQUENCY: %d\n",paramFrequency);
  alGetBufferi(buffer,AL_CHANNELS,&paramChannel);
  printf("AL_CHANNELS: %0x\n",paramChannel);
  alGetBufferi(buffer,AL_SIZE,&paramSize);
  printf("AL_SIZE: %d\n",paramSize);
  alGetBufferi(buffer,AL_BITS,&paramBits);
  printf("AL_BITS: %d\n",paramBits);
  ALuint bufCount = sizeof(buffer)/sizeof(ALuint) ;  
  printf("bufCount: %d\n",bufCount);

ALuint mul_buf[1];
mul_buf[0] = buffer;
alSourceQueueBuffers(source,1,mul_buf);
//  alSourcei (source, AL_BUFFER, buffer);
  alSourcePlay (source);
  do
  {
    alutSleep (0.1f);
    alGetSourcei (source, AL_SOURCE_STATE, &status);
    printf("status: %d\n",status);
  }
  while (status == AL_PLAYING);
}
int main (int argc, char **argv)
{
 if(argc < 2)
 {
  fprintf(stderr, "Usage: %s [-device <name>] <filename>\n", argv[0]);
  return 1;
}
  /* Initialise ALUT and eat any ALUT-specific commandline flags. */
if (!alutInit (&argc, argv))
{
  ALenum error = alutGetError ();
  fprintf (stderr, "%s\n", alutGetErrorString (error));
  exit (EXIT_FAILURE);
}
if(InitAL(&argv, &argc) != 0)
  return 1;

  /* Check for correct usage. */
if (argc != 2)
{
  fprintf (stderr, "usage: playfile <fileName>\n");
  alutExit ();
  exit (EXIT_FAILURE);
}

playFile (argv[1]);
return EXIT_SUCCESS;
}
