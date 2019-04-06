[ -z "$1" ] && SR=0 || SR=$1
[ -z "$2" ] && A=0 || A=$2
gcc -Wall -DSLOWRND=$SR -DALGO=$A -pthread -Wall -o rand2.o -funroll-loops -mtune=native -march=corei7 -msse4.2 -c -O3 -flto -pipe -fPIC -fstack-protector-strong rand2.c
gcc -pthread -o rand2 -funroll-loops -mtune=native -march=corei7 -msse4.2 -Wl,-flto -lm rand2.o
rm rand2.o
strip -s rand2
