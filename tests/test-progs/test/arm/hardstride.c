#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int main(int argc, char **argv) {
    int MEM_SIZE = 10 * 1024 * 1024; // *32 bits (number of ints)
    int DURATION = 60 * 1000 * 1000; // us
    int DELAY_OPS = 1; //us between mem requests
    int count = 0;

    int * mem = ( int* ) malloc( sizeof( int ) * MEM_SIZE );
    int elapsed = 0;
    int tmp;

    while( elapsed < DURATION ){
        int read_addr  = count % MEM_SIZE;
        tmp = mem[read_addr];
        elapsed += DELAY_OPS;
        count += 16;
    }

}

