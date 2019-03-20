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

int main() {

	fork();
	fork();
	
	unsigned long long varr[8];

	unsigned long long var=0,p=0,v=0;

	// random seed is 128 bit wide and stored in "var" and "p"

	int fd = open("/dev/random", O_RDWR);
	v=read(fd, &var, 8);
	v=read(fd, &p, 8);
	close(fd);

	while(1) {   // Generation is way faster by repeating the code 8 times than with a for loop.

		var = (var >> 32) ^ var ;
		varr[0]=var;
		v=p;
		p=var;
		var+=v;
		var = (var >> 32) ^ var ;
		varr[1]=var;
		v=p;
		p=var;
		var+=v;
		var = (var >> 32) ^ var ;
		varr[2]=var;
		v=p;
		p=var;
		var+=v;
		var = (var >> 32) ^ var ;
		varr[3]=var;
		v=p;
		p=var;
		var+=v;
		var = (var >> 32) ^ var ;
		varr[4]=var;
		v=p;
		p=var;
		var+=v;
		var = (var >> 32) ^ var ;
		varr[5]=var;
		v=p;
		p=var;
		var+=v;
		var = (var >> 32) ^ var ;
		varr[6]=var;
		v=p;
		p=var;
		var+=v;
		var = (var >> 32) ^ var ;
		varr[7]=var;
		v=p;
		p=var;
		var+=v;

		fwrite(&varr,64,1,stdout);
	}
}
