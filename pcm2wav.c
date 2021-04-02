#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "wav.h"

void usage(const char *process_name)
{
  printf("usage:\n"
         "\t%s pcm_file wav_file channels sample_rate sample_length\n",
         process_name
  );
}

int main(int argc,char *argv[])
{
  const char *pcm_file=NULL;
  const char *wav_file=NULL;
  unsigned short channels, sample_rate, sample_length;

  if(argc == 2)
  {
    if(strcmp(argv[1],"-h")==0 || strcmp(argv[1],"--help")==0)
    {
      usage(argv[0]);
      exit(EXIT_SUCCESS);
    }
  }

  if(argc != 6)
  {
    printf("error: invalid argument\n");
    usage(argv[0]);
    exit(EXIT_FAILURE);
  }

  pcm_file=argv[1];
  wav_file=argv[2];
  channels=atoi(argv[3]);
  sample_rate=atoi(argv[4]);
  sample_length=atoi(argv[5]);

  printf("******************* pcm2wav *********************\n"
         "  pcm file     : %s\n"
         "  wav file     : %s\n"
         "  channels     : %d\n"
         "  sample rate  : %d\n"
         "  sample length: %d\n",pcm_file,wav_file,channels,sample_rate,sample_length
  );
  printf(" ----------------------------------------------- \n");

  if(Pcm2Wav(pcm_file, wav_file, channels, sample_rate, sample_length)==0)
  {
    printf("  Convert Result: \033[1;32mSuccess\033[0m\n");
  }
  else
  {
    printf("  Convert Result: \033[1;31mFailed\033[0m\n");
  }
  printf("*************************************************\n");

  return 0;
}
