#include "evextd.h"
#include "router_client.h"
#include "ad4080.h"
#include <stdio.h>
#include <stdlib.h>

char *genv( const char *name ) {
	char *r = getenv( name );
	if( !r ) r = "";
	return r;
}

void put_event( struct event *ev, sample_t *samples, size_t count ) {
	static int first = 1;
	if( first ) {
		printf( "{\n" );
		first = 0;
	} else {
		printf( "," );
	}
	
	printf( "  {\n" );
	printf( "    { %u, %u, %u, %d },\n", 
		ev->rise, ev->peak, ev->fall, ev->height );
	printf( "    {\n" );
	int i;
	for( i = 0; i < count; i += 1 ) {
		printf( "%d", samples[ i ] );
		if( i < count - 1 ) printf( ",\n" );
		else printf( "}\n" );
	}
	
	printf( "  }" );
}
	
	


int main( int argc, char argv[] ) {

	size_t count;
		
	int port = router_connect( getenv( "EVEXTD" ));
	// no worry about a default, router_connect provides it, but,
	if( port < 0 ) {
		fprintf( stderr, "can't connect to event server\n" );
		exit( 1  );
	}
	
	FILE *stream = fdopen( port, "r+" );
	if( !stream ) {
		perror( "opening adc connection");
		exit( 1 );
	}
	
	struct parameters p = { 64, 1000, 100, 300000 }; // defaults
	
	sscanf( genv( "PHA_SHAPE" ), "%u", &p.shape );
	sscanf( genv( "PHA_LLD" ), "%d", &p.lld );
	sscanf( genv( "PHA_HYSTERESIS" ), "%d", &p.hys );
	sscanf( genv( "SDD_DUMP" ), "%d", &p.dump );
	
	unsigned pc = 100; // default number of events to acquire
	
	sscanf( genv( "PHA_COUNT" ), "%u", &pc );

	const uint32_t start_id = START_ID;
	
	count = fwrite( &start_id, sizeof start_id, 1, stream );
	if( count != 1 ) {
		perror( "writing start" );
		exit( 1 );
	}
	
	count = fwrite( &p, sizeof p, 1, stream );
	if( count != 1 ) {
		perror( "writing parameters" );
		exit( 1 );
	}
	
	while( pc ) {
		uint32_t id;
		count = fread( &id, sizeof id, 1, stream );
		if( count != 1 ) {
			perror( "reading id" );
			exit( 1 );
		}
		
		switch( id ) {
		
		case DATA_ID:
			struct event ev;
			count = fread( &ev, sizeof ev, 1, stream );
			if( count != 1 ) {
				perror( "reading event" );
				exit( 1 );
			}
			sample_t samples[ BUFFER_SAMPLES ]; // can't be larger
			count = fread( samples, sizeof( sample_t ),
				2 * p.shape, stream );
			if( count != 2 * p.shape ) {
				perror( "reading samples" );
				exit( 1 );
			}
			put_event( &ev, samples, count );
			pc -= 1;
			break;
			
		default:
			fprintf( stderr, "ID %x not recognized", id );
			exit( 1 );
		}
	}
	
	printf( "}\n" );
	return(0);
}	
	
			
	

	
