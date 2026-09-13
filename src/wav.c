#include "wav.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static unsigned int read_u32(FILE *f) {
    unsigned char b[4];
    if (fread(b, 1, 4, f) != 4) return 0;
    return (unsigned int)b[0] | ((unsigned int)b[1] << 8) |
           ((unsigned int)b[2] << 16) | ((unsigned int)b[3] << 24);
}

static unsigned short read_u16(FILE *f) {
    unsigned char b[2];
    if (fread(b, 1, 2, f) != 2) return 0;
    return (unsigned short)(b[0] | (b[1] << 8));
}

int wav_load(const char *path, WavAudio *out) {
    memset(out, 0, sizeof(*out));

    FILE *f = fopen(path, "rb");
    if (!f) {
        fprintf(stderr, "wav: cannot open %s\n", path);
        return 0;
    }

    char tag[4];
    if (fread(tag, 1, 4, f) != 4 || memcmp(tag, "RIFF", 4) != 0) {
        fprintf(stderr, "wav: %s is missing a RIFF header\n", path);
        fclose(f);
        return 0;
    }
    read_u32(f);
    if (fread(tag, 1, 4, f) != 4 || memcmp(tag, "WAVE", 4) != 0) {
        fprintf(stderr, "wav: %s is not a WAVE file\n", path);
        fclose(f);
        return 0;
    }

    int have_fmt = 0, have_data = 0;
    unsigned short audio_format = 0;

    while (!feof(f) && !(have_fmt && have_data)) {
        char chunk_id[4];
        if (fread(chunk_id, 1, 4, f) != 4) break;
        unsigned int chunk_size = read_u32(f);

        if (memcmp(chunk_id, "fmt ", 4) == 0) {
            long chunk_start = ftell(f);
            audio_format = read_u16(f);
            out->num_channels = read_u16(f);
            out->sample_rate = read_u32(f);
            read_u32(f);
            read_u16(f);
            out->bits_per_sample = read_u16(f);
            fseek(f, chunk_start + (long)chunk_size, SEEK_SET);
            have_fmt = 1;
        } else if (memcmp(chunk_id, "data", 4) == 0) {
            out->data = malloc(chunk_size);
            if (!out->data) {
                fprintf(stderr, "wav: out of memory loading %s\n", path);
                fclose(f);
                return 0;
            }
            if (fread(out->data, 1, chunk_size, f) != chunk_size) {
                fprintf(stderr, "wav: truncated data chunk in %s\n", path);
                free(out->data);
                out->data = NULL;
                fclose(f);
                return 0;
            }
            out->data_size = chunk_size;
            have_data = 1;
        } else {
            fseek(f, (long)chunk_size, SEEK_CUR);
        }
        if (chunk_size & 1) fseek(f, 1, SEEK_CUR);
    }
    fclose(f);

    if (!have_fmt || !have_data) {
        fprintf(stderr, "wav: %s is missing a fmt or data chunk\n", path);
        free(out->data);
        memset(out, 0, sizeof(*out));
        return 0;
    }
    if (audio_format != 1) {
        fprintf(stderr, "wav: %s uses format %u; only uncompressed PCM (1) is supported\n",
                path, audio_format);
        free(out->data);
        memset(out, 0, sizeof(*out));
        return 0;
    }
    return 1;
}

void wav_free(WavAudio *w) {
    free(w->data);
    w->data = NULL;
    w->data_size = 0;
}
