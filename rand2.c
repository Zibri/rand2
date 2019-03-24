/*****************************************************************************
Very fast random generator streaming output on stdout.
Example usage:
Color white noise 1080p@60
./rand2 | ffplay -fs -f rawvideo -pixel_format rgb24 -video_size 1920x1080 -framerate 60 -
Black & White (gray) white noise 1080p@60
./rand2 | ffplay -fs -f rawvideo -pixel_format gray -video_size 1920x1080 -framerate 60 -
Audio white noise
./rand2 | ffplay -f u16be -ar 48000 -ac 1 -
Audio + video color white noise
./rand2 | ffmpeg -f u16be -ar 48000 -ac 1 -i - -f rawvideo -pixel_format yuv420p -video_size 1920x1080 -framerate 60 -i - -c:a ac3 -vcodec copy -f avi - |ffplay -fs -volume 10 -
on my pc the bandwidth is about 1.95 GB/sec :D
*****************************************************************************/  
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#ifndef SLOWRND
#define SLOWRND 0               // 0 Ultrafast, 1 Fast
#endif

#if (SLOWRND > 1)
#error SLOWRND can be 0 or 1
#endif

#define BUFF1 16

#pragma GCC optimize ("unroll-loops")

int main(int argc, char **argv)
{
  unsigned int ncores = 0;

  long unsigned int init;

  asm volatile ("cpuid":"=a" (ncores):"a"(0xb), "c"(0x1):);


  asm volatile ("rdtsc":"=a" (init):);

  if (ncores > 1)
    fork();
  if (ncores > 3)
    fork();


  char buf[0x1000];
  setbuf(stdout, buf);

#ifdef __x86_64
  long long unsigned int varr[BUFF1 + 2 + SLOWRND];
#else
  long unsigned int varr[BUFF1 + 2 + SLOWRND];
#endif


  if (argc > 1) {
    varr[BUFF1] = strtoull(argv[1], NULL, 16);
    if (argc > 2)
      varr[BUFF1 + 1] = strtoull(argv[2], NULL, 16);
#if (SLOWRND >0)
    if (argc > 3)
      varr[BUFF1 + 2] = strtoull(argv[3], NULL, 16);
#endif
  }

  if ((argc == 0) || ((varr[BUFF1] | varr[BUFF1 + 1]) == 0)) {
    /* This gets the random seed from the cpu */

    for (int x = 0; x < (2 + SLOWRND); x++) {
      asm volatile ("rdrand %0\n":"=r" (varr[BUFF1 + x]):);
    }

  }

/* This routine adds a random initialization */

  if (argc == 0) {
    for (int i = 0; i < (init && 31); i++) {
      varr[0] = varr[BUFF1] + varr[BUFF1 + 1];
      varr[0] ^= (varr[0] < varr[BUFF1]);
#if (SLOWRND > 0)
      varr[0] = varr[0] + varr[BUFF1 + 2];
      varr[0] ^= (varr[0] < varr[BUFF1 + 2]);
      varr[BUFF1 + 2] = varr[BUFF1 + 1];
#endif
      varr[BUFF1 + 1] = varr[BUFF1];
      varr[BUFF1] = varr[0];
    }
  }

  while (1) {

    for (int i = 0; i < BUFF1; i++) {
      varr[i] = varr[BUFF1] + varr[BUFF1 + 1];
      varr[i] ^= (varr[i] < varr[BUFF1]);
#if (SLOWRND > 0)
      varr[i] = varr[i] + varr[BUFF1 + 2];
      varr[i] ^= (varr[i] < varr[BUFF1 + 2]);
      varr[BUFF1 + 2] = varr[BUFF1 + 1];
#endif
      varr[BUFF1 + 1] = varr[BUFF1];
      varr[BUFF1] = varr[i];
    }

    fwrite(&varr, BUFF1 * sizeof(varr[0]), 1, stdout);
  }
}
