#include "audio.h"
#include "wav.h"
#include "alsa_min.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { char path[512]; } PlayArgs;

static snd_pcm_format_t format_for_bits(unsigned short bits) {
    switch (bits) {
        case 8:  return SND_PCM_FORMAT_U8;
        case 16: return SND_PCM_FORMAT_S16_LE;
        case 24: return SND_PCM_FORMAT_S24_LE;
        case 32: return SND_PCM_FORMAT_S32_LE;
        default: return SND_PCM_FORMAT_UNKNOWN;
    }
}

static void *play_thread(void *arg) {
    PlayArgs *pa = arg;
    WavAudio wav;
    if (!wav_load(pa->path, &wav)) {
        free(pa);
        return NULL;
    }

    snd_pcm_format_t fmt = format_for_bits(wav.bits_per_sample);
    if (fmt == SND_PCM_FORMAT_UNKNOWN) {
        fprintf(stderr, "audio: unsupported bit depth %u in %s\n", wav.bits_per_sample, pa->path);
        wav_free(&wav);
        free(pa);
        return NULL;
    }

    snd_pcm_t *pcm = NULL;
    int err = snd_pcm_open(&pcm, "default", SND_PCM_STREAM_PLAYBACK, 0);
    if (err < 0) {
        fprintf(stderr, "audio: cannot open ALSA device for %s: %s\n", pa->path, snd_strerror(err));
        wav_free(&wav);
        free(pa);
        return NULL;
    }

    err = snd_pcm_set_params(pcm, fmt, SND_PCM_ACCESS_RW_INTERLEAVED,
                              wav.num_channels, wav.sample_rate, 1, 200000);
    if (err < 0) {
        fprintf(stderr, "audio: cannot configure ALSA device for %s: %s\n", pa->path, snd_strerror(err));
        snd_pcm_close(pcm);
        wav_free(&wav);
        free(pa);
        return NULL;
    }

    unsigned int bytes_per_frame = (unsigned int)wav.num_channels * (wav.bits_per_sample / 8);
    unsigned long remaining = bytes_per_frame ? wav.data_size / bytes_per_frame : 0;
    unsigned char *cursor = wav.data;

    while (remaining > 0) {
        long written = snd_pcm_writei(pcm, cursor, remaining);
        if (written < 0) {
            written = snd_pcm_recover(pcm, (int)written, 1);
            if (written < 0) break;
            continue;
        }
        cursor += (unsigned long)written * bytes_per_frame;
        remaining -= (unsigned long)written;
    }

    snd_pcm_drain(pcm);
    snd_pcm_close(pcm);
    wav_free(&wav);
    free(pa);
    return NULL;
}

bool audio_play_wav(const char *path) {
    PlayArgs *pa = malloc(sizeof(PlayArgs));
    if (!pa) return false;
    strncpy(pa->path, path, sizeof(pa->path) - 1);
    pa->path[sizeof(pa->path) - 1] = '\0';

    pthread_t thread;
    if (pthread_create(&thread, NULL, play_thread, pa) != 0) {
        fprintf(stderr, "audio: failed to spawn playback thread for %s\n", path);
        free(pa);
        return false;
    }
    pthread_detach(thread);
    return true;
}
