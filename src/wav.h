#ifndef ENGINE_WAV_H
#define ENGINE_WAV_H

typedef struct {
    unsigned int sample_rate;
    unsigned short num_channels;
    unsigned short bits_per_sample;
    unsigned char *data;
    unsigned int data_size;
} WavAudio;

int wav_load(const char *path, WavAudio *out);
void wav_free(WavAudio *w);

#endif
