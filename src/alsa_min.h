#ifndef ENGINE_ALSA_MIN_H
#define ENGINE_ALSA_MIN_H

typedef struct _snd_pcm snd_pcm_t;

typedef enum {
    SND_PCM_STREAM_PLAYBACK = 0,
    SND_PCM_STREAM_CAPTURE = 1
} snd_pcm_stream_t;

typedef enum {
    SND_PCM_FORMAT_UNKNOWN = -1,
    SND_PCM_FORMAT_S8 = 0,
    SND_PCM_FORMAT_U8,
    SND_PCM_FORMAT_S16_LE,
    SND_PCM_FORMAT_S16_BE,
    SND_PCM_FORMAT_U16_LE,
    SND_PCM_FORMAT_U16_BE,
    SND_PCM_FORMAT_S24_LE,
    SND_PCM_FORMAT_S24_BE,
    SND_PCM_FORMAT_U24_LE,
    SND_PCM_FORMAT_U24_BE,
    SND_PCM_FORMAT_S32_LE,
    SND_PCM_FORMAT_S32_BE
} snd_pcm_format_t;

typedef enum {
    SND_PCM_ACCESS_MMAP_INTERLEAVED = 0,
    SND_PCM_ACCESS_MMAP_NONINTERLEAVED,
    SND_PCM_ACCESS_MMAP_COMPLEX,
    SND_PCM_ACCESS_RW_INTERLEAVED,
    SND_PCM_ACCESS_RW_NONINTERLEAVED
} snd_pcm_access_t;

extern int snd_pcm_open(snd_pcm_t **pcm, const char *name, snd_pcm_stream_t stream, int mode);
extern int snd_pcm_close(snd_pcm_t *pcm);
extern int snd_pcm_set_params(snd_pcm_t *pcm, snd_pcm_format_t format, snd_pcm_access_t access,
                               unsigned int channels, unsigned int rate, int soft_resample,
                               unsigned int latency_us);
extern int snd_pcm_prepare(snd_pcm_t *pcm);
extern long snd_pcm_writei(snd_pcm_t *pcm, const void *buffer, unsigned long size);
extern int snd_pcm_drain(snd_pcm_t *pcm);
extern int snd_pcm_recover(snd_pcm_t *pcm, int err, int silent);
extern const char *snd_strerror(int errnum);

#endif
