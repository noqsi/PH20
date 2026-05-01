#include "extract.h"
#include <stdlib.h>
#include <stdio.h>

#define NSAMP 100
#define SHAPE 10
#define WHEN 30

void mathout( char *name, sample_t *s, int n ) {
	printf( "%s = {\n", name );
	while( --n ) printf( "%d,\n", *s++ );
	printf( "%d };\n", *s );
}

void main( int argc, char *argv[] ) {
	sample_t samples[ NSAMP ], sums[ NSAMP ], diffs[ NSAMP ];
	int i;
	
	for( i = 0; i < NSAMP; i += 1 ) {
		if( i < WHEN ) samples[ i ] = -1;
		else samples[ i ] = 2;
	}
	
	mathout( "samples", samples, NSAMP );
	
	running_sum( samples, sums, NSAMP, SHAPE );
	mathout( "sums", sums, NSAMP-SHAPE+1 );
	
	running_difference( sums, diffs, NSAMP-SHAPE+1, SHAPE );
	mathout( "diffs", diffs, NSAMP-2*SHAPE+1 );
	
	struct parameters p = { SHAPE, 5, 2, 100 };
	
	struct event *e = trigger_search( diffs, NSAMP-2*SHAPE+1, &p );
	
	sample_t ev[4];
	ev[0] = e->rise;
	ev[1] = e->peak;
	ev[2] = e->fall;
	ev[3] = e->height;
	
	mathout( "event", ev, 4 );
	
	exit( 0 );
}
