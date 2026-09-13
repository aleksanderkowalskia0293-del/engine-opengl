#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PI 3.14159265358979323846
#define CHECKER_SIZE 256
#define TILE 32

static int write_checker_png(const char *path) {
    FILE *fp = fopen(path, "wb");
    if (!fp) { perror(path); return 0; }

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);
    if (!png || !info || setjmp(png_jmpbuf(png))) {
        fprintf(stderr, "gen_assets: libpng error writing %s\n", path);
        fclose(fp);
        return 0;
    }

    png_init_io(png, fp);
    png_set_IHDR(png, info, CHECKER_SIZE, CHECKER_SIZE, 8, PNG_COLOR_TYPE_RGBA,
                 PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);
    png_write_info(png, info);

    png_bytep row = malloc((size_t)CHECKER_SIZE * 4);
    for (int y = 0; y < CHECKER_SIZE; y++) {
        for (int x = 0; x < CHECKER_SIZE; x++) {
            int tile = ((x / TILE) + (y / TILE)) % 2;
            unsigned char r, g, b;
            if (tile == 0) { r = 235; g = 140; b = 52; }
            else           { r = 35;  g = 42;  b = 64; }
            row[x * 4 + 0] = r;
            row[x * 4 + 1] = g;
            row[x * 4 + 2] = b;
            row[x * 4 + 3] = 255;
        }
        png_write_row(png, row);
    }
    free(row);
    png_write_end(png, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
    return 1;
}

static void write_u32(FILE *f, unsigned int v) {
    unsigned char b[4] = { (unsigned char)v, (unsigned char)(v >> 8),
                            (unsigned char)(v >> 16), (unsigned char)(v >> 24) };
    fwrite(b, 1, 4, f);
}
static void write_u16(FILE *f, unsigned short v) {
    unsigned char b[2] = { (unsigned char)v, (unsigned char)(v >> 8) };
    fwrite(b, 1, 2, f);
}

static int write_beep_wav(const char *path) {
    const unsigned int sample_rate = 44100;
    const double duration = 0.5;
    const double freq = 660.0;
    unsigned int num_samples = (unsigned int)(sample_rate * duration);
    unsigned int data_size = num_samples * 2;

    FILE *f = fopen(path, "wb");
    if (!f) { perror(path); return 0; }

    fwrite("RIFF", 1, 4, f);
    write_u32(f, 36 + data_size);
    fwrite("WAVE", 1, 4, f);

    fwrite("fmt ", 1, 4, f);
    write_u32(f, 16);
    write_u16(f, 1);
    write_u16(f, 1);
    write_u32(f, sample_rate);
    write_u32(f, sample_rate * 2);
    write_u16(f, 2);
    write_u16(f, 16);

    fwrite("data", 1, 4, f);
    write_u32(f, data_size);

    for (unsigned int i = 0; i < num_samples; i++) {
        double t = (double)i / sample_rate;
        double envelope = 1.0 - (double)i / num_samples;
        double sample = sin(2.0 * PI * freq * t) * envelope * 0.5;
        write_u16(f, (unsigned short)(short)(sample * 32767.0));
    }
    fclose(f);
    return 1;
}

int main(void) {
    int ok = write_checker_png("assets/checker.png");
    ok &= write_beep_wav("assets/beep.wav");
    if (ok) {
        printf("generated assets/checker.png and assets/beep.wav\n");
    }
    return ok ? 0 : 1;
}
