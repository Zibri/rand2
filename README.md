# rand2
Fastest streaming (stdout) random generator

This is just another quick & dirty code to generate a random stream of data fastly.
It's useful for testing video compressors or effects.
But can be useful for anything.

rand2 passes all NIST tests for randomness
https://gerhardt.ch/downloads/randtests_python.tgz

# monobitfrequencytest	blockfrequencytest	runstest	longestrunones10000	binarymatrixranktest	spectraltest	nonoverlappingtemplatematchingtest	overlappingtemplatematchingtest	maurersuniversalstatistictest	linearcomplexitytest	serialtest	aproximateentropytest	cumultativesumstest	randomexcursionstest	randomexcursionsvarianttest	cumultativesumstestreverse
True	True	True	True	True	True	True	True	True	True	True	True	True	True	True	True

P.S.
The source can be optimized even more and gain far better speeds.
You are free to contribute.

Some comparisons:

    $ ./rand2|pv -S -s 2G >/dev/null
    2,00GiB 0:00:01 [1,94GiB/s] [========================================================================>] 100%            

    $ ./rand|pv -S -s 2G >/dev/null
    2,00GiB 0:00:01 [1,58GiB/s] [========================================================================>] 100%            

    $ cat /dev/urandom|pv -S -s 2G >/dev/null
    2,00GiB 0:00:11 [ 178MiB/s] [========================================================================>] 100%            

    $ dd if=/dev/zero bs=1k count=2M 2>/dev/null | openssl enc -rc4-40 -pass pass:weak |pv -S -s 2G >/dev/null
    2,00GiB 0:00:04 [ 413MiB/s] [========================================================================>] 100%            
