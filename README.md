# rand2
Fastest streaming (stdout) random generator

This is just another quick & dirty code to generate a random stream of data fastly.
It's useful for testing video compressors or effects.
But can be useful for anything.

rand2 passes all NIST tests for randomness
https://gerhardt.ch/downloads/randtests_python.tgz

White noise audio

./rand2|ffplay -f s16le -ar 48000 -ac 2 -

White noise video (full HD color 60fps)

./rand2 | ffplay -fs -f rawvideo -pixel_format rgb24 -video_size 1920x1080 -framerate 60 -

White noise audio+video

./rand2 | ffmpeg -f rawvideo -pixel_format rgb24 -video_size 1920x1080 -framerate 60 -i - -f s16le -ar 48000 -ac 2 -i - -c:a pcm_s16le -c:v rawvideo -f avi -|ffplay -fs -

P.S.
The source can be optimized even more and gain far better speeds.
You are free to contribute.

Some comparisons:

    $ sudo ./rand2 1 1| pv -ptebaSs 800G >/dev/null  # if run with sudo, rand2 will have maximum priority
    800GiB 0:01:40 [8,00GiB/s] [========================================================================>] 100%          

    $ ./rand|pv -S -s 2G >/dev/null
    2,00GiB 0:00:01 [1,58GiB/s] [========================================================================>] 100%            

    $ cat /dev/urandom|pv -S -s 2G >/dev/null
    2,00GiB 0:00:11 [ 178MiB/s] [========================================================================>] 100%            

    $ dd if=/dev/zero bs=1k count=2M 2>/dev/null | openssl enc -rc4-40 -pass pass:weak |pv -S -s 2G >/dev/null
    2,00GiB 0:00:04 [ 413MiB/s] [========================================================================>] 100%            

## Try it in Google Cloud Shell
[![Open in Cloud Shell](http://gstatic.com/cloudssh/images/open-btn.png)](https://console.cloud.google.com/cloudshell/open?git_repo=https:%2F%2Fgithub.com%2Fzibri%2Frand2&shellonly=true&page=shell)

Reference speed on Cloud Shell:
80GiB 0:00:49 [1.61GiB/s] [========================================================================>] 100%
