#include "extract.h"

#define NSAMP 100
#define WHEN 30

void mathout( char *name, sample_t *s, int n ) {
	printf( "%s = {\n", name );
	while( --n ) printf( "%d,\n", *s++ );
	printf( "%d };\n", *s )
}

void main( int argc, char *argv[] ) {
	sample_t samples[ NSAMP ], summed[ NSAMP ], diffed[ NSAMP ];
	int i;
	
	for( i = 0; i < NSAMP; i += 1 ) {
		if( i < WHEN ) samples[ i ] = -1;
		else samples[ i ] = 2;
	}
	
	mathout( "samples", samples, NSAMP );
	exit( 0 );
}