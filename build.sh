gcc -o rand2.o -c -Ofast -flto -pipe -fPIC -fno-plt -fstack-protector-strong -D_GNU_SOURCE rand2.c
gcc -o rand2 -Wl,-flto -lm rand2.o
rm rand2.o
strip -s rand2
