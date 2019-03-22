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

#define BUFF1 16

int main()
{

  fork();
  fork();

  char buf[0x80000];
  setbuf(stdout, buf);

  long long unsigned int varr[BUFF1 + 2];

  // random seed is 128 bit wide 

  /*  This gets the random seed from /dev/urandom

     int fd = open("/dev/urandom", O_RDWR);
     v=read(fd, &varr[BUFF1], 8);
     v=read(fd, &varr[BUFF1+1], 8);
     close(fd);

   */

  /* This gets the random seed from the cpu */

  __asm__ __volatile__("rdrand %0\nrdrand %1\n":"=r"(varr[BUFF1]), "=r"(varr[BUFF1 + 1]):);

  /* The real seed should be 65 bit wide (64 + carry) */

  while (1) {

    // Generation is way faster by repeating the code 8 times than with a for loop.
    // compile with -funroll-loops !

    for (int i = 0; i < BUFF1; i++) {

      varr[i] = varr[BUFF1] + varr[BUFF1 + 1];
      varr[i] |= (varr[i] < varr[BUFF1]);       // without this there will be a visible bit error 
      varr[BUFF1 + 1] = varr[BUFF1];
      varr[BUFF1] = varr[i];
    }

    fwrite(&varr, BUFF1 * sizeof(varr[0]), 1, stdout);
  }
}
