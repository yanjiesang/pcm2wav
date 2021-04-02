#ifndef __WAV_H__
#define __WAV_H__

#define LITTLE_ENDIAN_BYTE_ORDER    0
#define BIG_ENDIAN_BYTE_ORDER       1
#define PCM_BYTE_ORDER              LITTLE_ENDIAN_BYTE_ORDER

#if PCM_BYTE_ORDER == LITTLE_ENDIAN_BYTE_ORDER
    #define COMPOSE_ID(a,b,c,d)	    ((a) | ((b)<<8) | ((c)<<16) | ((d)<<24))
    #define LE_SHORT(v)		        (v)
    #define LE_INT(v)		        (v)
    #define BE_SHORT(v)		        bswap_16(v)
    #define BE_INT(v)		        bswap_32(v)
#elif PCM_BYTE_ORDER == BIG_ENDIAN_BYTE_ORDER
    #define COMPOSE_ID(a,b,c,d)	    ((d) | ((c)<<8) | ((b)<<16) | ((a)<<24))
    #define LE_SHORT(v)		        bswap_16(v)
    #define LE_INT(v)		        bswap_32(v)
    #define BE_SHORT(v)		        (v)
    #define BE_INT(v)		        (v)
#else
    #error "Wrong endian"
#endif

#define WAV_RIFF		COMPOSE_ID('R','I','F','F')
#define WAV_WAVE		COMPOSE_ID('W','A','V','E')
#define WAV_FMT			COMPOSE_ID('f','m','t',' ')
#define WAV_DATA		COMPOSE_ID('d','a','t','a')
 
/* WAVE fmt block constants from Microsoft mmreg.h header */
#define WAV_FMT_PCM             0x0001
#define WAV_FMT_IEEE_FLOAT      0x0003
#define WAV_FMT_DOLBY_AC3_SPDIF 0x0092
#define WAV_FMT_EXTENSIBLE      0xfffe

typedef struct
{
    unsigned int magic;		        /* 'RIFF' */
    unsigned int length;	        /* filelen */
    unsigned int type;		        /* 'WAVE' */
}WavHeader;

typedef struct 
{
    unsigned int magic;             /* 'FMT '*/
    unsigned int fmt_size;          /* 16 or 18 */
    unsigned int format;		    /* see WAV_FMT_* */
    unsigned short channels;
    unsigned short sample_rate;	    /* frequence of sample */
    unsigned int bytes_p_second;
    unsigned short blocks_align;	/* samplesize; 1 or 2 bytes */
    unsigned short sample_length;	/* 8, 12 or 16 bit */
} WavFmt;

typedef struct 
{
    unsigned int type;		        /* 'data' */
    unsigned int length;		    /* samplecount */
} WavChunkHeader;

typedef struct {
    WavHeader header;
    WavFmt format;
    WavChunkHeader chunk;
} WavContainer, *PWavContainer;

int Pcm2Wav(const char *pcm_file, const char *wav_file, unsigned short channels, unsigned short sample_rate, unsigned short sample_length);

#endif