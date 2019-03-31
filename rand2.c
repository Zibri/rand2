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
Speed test
sudo nice -n -20 ./rand2 | pv -ptebaSs 800G >/dev/null
on my pc the bandwidth is about 7.86 GB/sec :D
*****************************************************************************/
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sched.h>
#include <pthread.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <stdint.h>
#include <sys/syscall.h>

#ifndef SLOWRND
#define SLOWRND 0		// 0 Ultrafast, 1 Fast
#endif

#ifndef ALGO
#define ALGO 0			// 0 Fib/C, 1 Fib/Rot
#endif

#if (SLOWRND > 1)
#error SLOWRND can be 0 or 1
#endif

#define BUFF1 16

#define rotr(x, n) (( x>>n  ) | (x<<((sizeof(x)<<3)-n)))

#pragma GCC optimize ("unroll-loops")
struct sched_attr
{
  uint32_t size;
  uint32_t sched_policy;
  uint64_t sched_flags;
  int32_t sched_nice;
  uint32_t sched_priority;
  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};
unsigned int flags = 0;

static int
sched_setattr (pid_t pid, const struct sched_attr *attr, unsigned int flags)
{
  return syscall (SYS_sched_setattr, pid, attr, flags);
}

static inline int
ioprio_set (int which, int who, int ioprio)
{
  return syscall (SYS_ioprio_set, which, who, ioprio);
}

int
main (int argc, char **argv)
{
  unsigned int ncores = 0, nthreads = 0;

  asm volatile ("cpuid":"=a" (ncores), "=b" (nthreads):"a" (0xb), "c" (0x1):);

  long pipe_size = (long) fcntl (1, F_GETPIPE_SZ);

  if (pipe_size != -1)
    {
      fcntl (1, F_SETPIPE_SZ, pipe_size << 3);
      pipe_size = (long) fcntl (1, F_GETPIPE_SZ);
    }

#ifdef __x86_64
  long unsigned int init;
  asm volatile ("rdtsc":"=a" (init):);
#else
  long unsigned int init = 0;
  init = (long unsigned int) &init << 16 >> 24;
#endif

  if (ncores > 1)
    fork ();
  if (ncores > 3)
    fork ();

  struct sched_attr attr;
  attr.size = sizeof (attr);
  attr.sched_flags = 0;
  attr.sched_nice = 99;
  attr.sched_priority = 99;

  attr.sched_policy = SCHED_RR;
  attr.sched_runtime = 0;
  attr.sched_period = attr.sched_deadline = 0;

  sched_setattr (0, &attr, flags);	// chrt -r 99

  ioprio_set (1, 0, (1 << 13) | 7);	// ionice -c 1 7
  setpriority (0, 0, -20);	// nice -n -20


  char buf[0x4000];
  setvbuf (stdout, buf, _IOFBF, 0x4000);

#ifdef __x86_64
  long long unsigned int varr[BUFF1 + 2 + SLOWRND];
#else
  long unsigned int varr[BUFF1 + 2 + SLOWRND];
#endif


  if (argc > 1)
    {
      varr[BUFF1] = strtoull (argv[1], NULL, 16);
      if (argc > 2)
	varr[BUFF1 + 1] = strtoull (argv[2], NULL, 16);
#if (SLOWRND >0)
      if (argc > 3)
	varr[BUFF1 + 2] = strtoull (argv[3], NULL, 16);
#endif
    }

  if ((argc == 1) || ((varr[BUFF1] | varr[BUFF1 + 1]) == 0))
    {

      for (int x = 0; x < (2 + SLOWRND); x++)
	{
	  asm volatile ("rdrand %0\n":"=r" (varr[BUFF1 + x]):);
	}

    }


  if (argc == 1)
    {
      for (int i = 0; i < (init && 31); i++)
	{
	  varr[0] = varr[BUFF1] + varr[BUFF1 + 1];
#if (ALGO==0)
	  varr[0] ^= (varr[0] < varr[BUFF1]);
#else
	  varr[0] = rotr (varr[0], 8);
#endif
#if (SLOWRND > 0)
	  varr[0] = varr[0] + varr[BUFF1 + 2];
	  varr[0] ^= (varr[0] < varr[BUFF1 + 2]);
	  varr[BUFF1 + 2] = varr[BUFF1 + 1];
#endif
	  varr[BUFF1 + 1] = varr[BUFF1];
	  varr[BUFF1] = varr[0];
	}
    }

  while (1)
    {

      for (int i = 0; i < BUFF1; i++)
	{
	  varr[i] = varr[BUFF1] + varr[BUFF1 + 1];
#if (ALGO==0)
	  varr[i] ^= (varr[i] < varr[BUFF1]);
#else
	  varr[i] = rotr (varr[i], 8);
#endif
#if (SLOWRND > 0)
	  varr[i] = varr[i] + varr[BUFF1 + 2];
	  varr[i] ^= (varr[i] < varr[BUFF1 + 2]);
	  varr[BUFF1 + 2] = varr[BUFF1 + 1];
#endif
	  varr[BUFF1 + 1] = varr[BUFF1];
	  varr[BUFF1] = varr[i];
	}

      fwrite (&varr, BUFF1 * sizeof (varr[0]), 1, stdout);
    }
}
