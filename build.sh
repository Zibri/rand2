gcc -o rand2.o -funroll-loops -mtune=native -march=corei7 -msse4.2 -c -O3 -flto -pipe -fPIC -fno-plt -fstack-protector-strong -D_GNU_SOURCE rand2.c
gcc -o rand2 -funroll-loops -mtune=native -march=corei7 -msse4.2 -Wl,-flto -lm rand2.o
rm rand2.o
strip -s rand2
