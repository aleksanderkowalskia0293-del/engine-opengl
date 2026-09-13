#include "audio.h"
#include "wav.h"
#include <windows.h>
#include <mmsystem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct { char path[512]; } PlayArgs;

static DWORD WINAPI play_thread(LPVOID arg) {
    PlayArgs *pa = arg;
    WavAudio wav;
    if (!wav_load(pa->path, &wav)) {
        free(pa);
        return 0;
    }

    WAVEFORMATEX fmt;
    ZeroMemory(&fmt, sizeof(fmt));
    fmt.wFormatTag = WAVE_FORMAT_PCM;
    fmt.nChannels = wav.num_channels;
    fmt.nSamplesPerSec = wav.sample_rate;
    fmt.wBitsPerSample = wav.bits_per_sample;
    fmt.nBlockAlign = (WORD)(fmt.nChannels * (fmt.wBitsPerSample / 8));
    fmt.nAvgBytesPerSec = fmt.nSamplesPerSec * fmt.nBlockAlign;

    HANDLE event = CreateEventA(NULL, FALSE, FALSE, NULL);
    HWAVEOUT hwo = NULL;
    if (waveOutOpen(&hwo, WAVE_MAPPER, &fmt, (DWORD_PTR)event, 0, CALLBACK_EVENT) != MMSYSERR_NOERROR) {
        fprintf(stderr, "audio: waveOutOpen failed for %s\n", pa->path);
        CloseHandle(event);
        wav_free(&wav);
        free(pa);
        return 0;
    }

    WAVEHDR hdr;
    ZeroMemory(&hdr, sizeof(hdr));
    hdr.lpData = (LPSTR)wav.data;
    hdr.dwBufferLength = wav.data_size;

    waveOutPrepareHeader(hwo, &hdr, sizeof(hdr));
    waveOutWrite(hwo, &hdr, sizeof(hdr));

    while (!(hdr.dwFlags & WHDR_DONE)) {
        WaitForSingleObject(event, INFINITE);
    }

    waveOutUnprepareHeader(hwo, &hdr, sizeof(hdr));
    waveOutClose(hwo);
    CloseHandle(event);
    wav_free(&wav);
    free(pa);
    return 0;
}

bool audio_play_wav(const char *path) {
    PlayArgs *pa = malloc(sizeof(PlayArgs));
    if (!pa) return false;
    strncpy(pa->path, path, sizeof(pa->path) - 1);
    pa->path[sizeof(pa->path) - 1] = '\0';

    HANDLE h = CreateThread(NULL, 0, play_thread, pa, 0, NULL);
    if (!h) {
        free(pa);
        return false;
    }
    CloseHandle(h);
    return true;
}
