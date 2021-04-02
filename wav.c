#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include "wav.h"

static PWavContainer CreatWavContainer(void)
{
    PWavContainer pWavContainer=NULL;
    pWavContainer=(PWavContainer)calloc(1,sizeof(WavContainer));
    return pWavContainer;
}

static void DestoryWavContainer(PWavContainer pWavContainer)
{
    if(pWavContainer)
    {
        free(pWavContainer);
        pWavContainer=NULL;
    }
}

static int WavHeaderFill(const PWavContainer pWavContainer,unsigned short channels, unsigned short sample_rate, unsigned short sample_length, unsigned int chunk_data_length)
{   
    if(pWavContainer==NULL)
    {
        return -1;
    }

	pWavContainer->header.magic = WAV_RIFF;
	pWavContainer->header.type = WAV_WAVE;

	pWavContainer->format.magic = WAV_FMT;
	pWavContainer->format.fmt_size = LE_INT(16);
	pWavContainer->format.format = LE_SHORT(WAV_FMT_PCM);
    pWavContainer->format.channels = LE_SHORT(channels);
	pWavContainer->format.sample_rate = LE_INT(sample_rate);
    pWavContainer->format.blocks_align = LE_SHORT(channels * sample_length / 8);
    pWavContainer->format.bytes_p_second = LE_INT((unsigned short)(pWavContainer->format.blocks_align) * sample_rate);
    pWavContainer->format.sample_length = LE_SHORT(sample_length);

	pWavContainer->chunk.type = WAV_DATA;
	pWavContainer->chunk.length = LE_INT(chunk_data_length);

	pWavContainer->header.length = LE_INT((unsigned int)(pWavContainer->chunk.length) 
                                           + sizeof(pWavContainer->chunk) 
                                           + sizeof(pWavContainer->format) 
                                           + sizeof(pWavContainer->header) - 8);
 
	return 0;
}

int Pcm2Wav(const char *pcm_file, const char *wav_file, unsigned short channels, unsigned short sample_rate, unsigned short sample_length)
{
    int rc=0;
    int pcm_file_fd=-1;
    char *pcm_file_read_buffer=NULL;
    unsigned int pcm_file_read_size=0;
    int wav_file_fd=-1;
    unsigned int wav_file_write_size=0;
    struct stat pcm_file_stat;
    PWavContainer pWavContainer=NULL;

    if(pcm_file==NULL)
    {
        printf("  [error] pcm file path is NULL\n");
        return -1;
    }

    if(wav_file==NULL)
    {
        printf("  [error] wav file path is NULL\n");
        return -1;
    }

    if(channels!=1 && channels!=2)
    {
        printf("  [error] channel set %d, should set 1(Mono) or 2(Stereo)\n",channels);
        return -1;
    }

    pcm_file_fd=open(pcm_file,O_RDONLY);
    if(pcm_file_fd==-1)
    {
        printf("  [error] pcm file [%s] open failed, %s\n",pcm_file, strerror(errno));
        return -1;
    }

    wav_file_fd=open(wav_file,O_WRONLY|O_TRUNC|O_CREAT|O_APPEND,0664);
    if(wav_file_fd==-1)
    {
        printf("  [error] wav file [%s] open or creat failed, %s\n",wav_file,strerror(errno));
        close(pcm_file_fd);
        return -1;  
    }

    pWavContainer = CreatWavContainer();
    if(pWavContainer==NULL)
    {
        printf("  [error] creat wav container failed\n");
        rc=-1; 
        goto CleanUp;
    }

    if(fstat(pcm_file_fd,&pcm_file_stat)!=0)
    {
        printf("  [error] get pcm file stat failed, %s\n",strerror(errno));
        rc=-1;
        goto CleanUp;
    }

    WavHeaderFill(pWavContainer,channels, sample_rate, sample_length, pcm_file_stat.st_size);

    wav_file_write_size=write(wav_file_fd,pWavContainer,sizeof(WavContainer));
    if(wav_file_write_size!=sizeof(WavContainer))
    {
        printf("  [error] write wav header failed\n");
        rc=-1;
        goto CleanUp;
    }

    pcm_file_read_buffer=(char *)calloc(1,pcm_file_stat.st_size*sizeof(char));
    pcm_file_read_size=read(pcm_file_fd,pcm_file_read_buffer,pcm_file_stat.st_size*sizeof(char));
    if(pcm_file_read_size!=pcm_file_stat.st_size*sizeof(char))
    {
        printf("  [error] read pcm header failed\n");
        rc=-1;
        goto CleanUp;
    }

    wav_file_write_size=write(wav_file_fd,pcm_file_read_buffer,pcm_file_read_size);
    if(wav_file_write_size!=pcm_file_read_size)
    {
        printf("  [error] write wav chunk data failed\n");
        rc=-1;
        goto CleanUp;
    }

CleanUp:
    if(pcm_file_read_buffer)
    {
        free(pcm_file_read_buffer);
        pcm_file_read_buffer=NULL;
    }
    close(pcm_file_fd);  
    close(wav_file_fd);  
    DestoryWavContainer(pWavContainer);

    return rc;
}

