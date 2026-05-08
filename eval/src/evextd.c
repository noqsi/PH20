#include "evextd.h"
#include "ad4080.h"
#include "extract.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

void check_id( uint32_t id, uint32_t expected ) {
	if( id == expected ) return;
	fprintf( stderr, "Expected packet ID %X, saw %X\n", expected, id );
	exit( 1 );
}

void go( struct ad4080 adc, struct parameters p ) {
// These are a bit bigger than necessary, but it's simple to make them fixed size
	sample_t sums[ BUFFER_SAMPLES ], diffs[ BUFFER_SAMPLES ];
	const uint32_t datid = DATA_ID;
	size_t written;
	
	for(;;) {
		int32_t *samples = get_4080_samples( adc );
		running_sum( samples, sums, BUFFER_SAMPLES, p.shape );
		running_difference( sums, diffs, 
			BUFFER_SAMPLES - p.shape + 1, p.shape );
		struct event *e;
		while ( e = trigger_search( 
			diffs, BUFFER_SAMPLES - 2 * p.shape + 1, &p )) {
			
			written = fwrite( &datid, sizeof datid, 1, stdout );
			if( written != 1 ) return;
			written = fwrite( e, sizeof *e, 1, stdout );
			if( written != 1 ) return;
			written =  fwrite( samples + e->peak, 
				sizeof( sample_t ), 2 * p.shape, stdout );
			if( written != 2 * p.shape ) return;
		}
	}
}

int main( int argc, char *argv[] ) {

	struct ad4080 adc = connect_4080();
	
//	signal(SIGPIPE, SIG_IGN);	// let stdio see the break
	
// Main loop

	for(;;) {
		
// Read the start message
		uint32_t id;
		int items = fread( &id, sizeof id, 1, stdin );
		if( items != 1 ) {
			perror( "start");
			exit( 1 );
		}
		check_id( id, START_ID );
		
		struct parameters p;
		items = fread( &p, sizeof p, 1, stdin );
		if( items != 1 ) {
			perror( "parameters");
			exit( 1 );
		}
		
		go( adc, p );	// do the work
		
	}	
}
