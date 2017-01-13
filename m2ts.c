#include <stdio.h>
#include <time.h>

#define TS 188
#define M2TS 192

int main(int argc, char *argv[]) {
    unsigned long long int i = 0;
    unsigned long long int o = 0;
    unsigned char buffer[M2TS];

    setvbuf(stdin, NULL, _IOFBF, BUFSIZ / M2TS * M2TS);
    setvbuf(stdout, NULL, _IOFBF, BUFSIZ / TS * TS);

    clock_t ts0 = clock();

    while (fread(&buffer, M2TS, 1, stdin) == 1) {
        ++i;

        if (buffer[4] == 0x47 && // Transport Stream packet
            (buffer[5] & 0x80) == 0 && // Transport Error Indicator not set
            !((buffer[1] & 0x1F) == 0x1F && (buffer[2] & 0xFF) == 0xFF) // not Null Packet
            ) {
            if (fwrite(&buffer[4], TS, 1, stdout) != 1) {
                break;
            }
            ++o;
        }
    }

    fflush(stdout);

    clock_t ts = clock();
    double t = (double)(ts - ts0) / CLOCKS_PER_SEC;

    fprintf(stderr, "%llu M2TS packets in (%llu B)\n", i, M2TS * i);
    fprintf(stderr, "%llu TS packets out (%llu B)\n", o, TS * o);
    fprintf(stderr, "%llu B discarded\n", M2TS * i - TS * o);
    fprintf(stderr, "%.6f s, %.2f MiB/s\n", t, M2TS * i / t / 1024 / 1024);

    return 0;
}
