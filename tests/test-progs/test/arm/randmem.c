#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

int nop( int delay ){
    int tmp=0,i;
    for( i=0; i<delay; i++ )
        tmp = i + tmp;
    return tmp;
}

int main(int argc, char **argv) {
    int SEED = 2;
    int MEM_SIZE = 10 * 1024 * 1024; // *32 bits (number of ints)
    int DURATION = 6 * 1000 * 1000; //us
    int DELAY_OPS = 1000; //us between mem requests

    int opt;
    while( ( opt = getopt( argc, argv, "s:d:p:" ) ) != -1 ){
        switch( opt ){
            case 's':
                MEM_SIZE = 1024 * 1024 * atoi(optarg); // *32 MB
                break;
            case 'd':
                DURATION = atoi(optarg); // seconds
                break;
            case 'p':
                DELAY_OPS = atoi(optarg); // us
                break;
            default:
                break;
        }
    } 

    int * mem = ( int* ) malloc( sizeof( int ) * MEM_SIZE );
    int elapsed = 0;
    int tmp;
    int count = 0;
    srand( SEED );

    while( elapsed < DURATION ){
        //int read_addr  = rand() % MEM_SIZE;
        int read_addr = count % MEM_SIZE;
        tmp += mem[read_addr];
        elapsed += DELAY_OPS;
        tmp += nop(DELAY_OPS);
        count += 16;
    }
    printf("Sum is %d\n", tmp);
}

