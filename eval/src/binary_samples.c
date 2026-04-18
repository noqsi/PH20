#include <iio.h>
#include <stdio.h>
#include <string.h>
#include "ad4080.h"

/*
 Grab buffers of binary samples, send to stdout
*/

void usage( void ) {
	fprintf( stderr, "usage: binary_samples <block count>\n" );
	fprintf( stderr, "Blocks are 16384 4-byte binary integers.\n" );
	fprintf( stderr, "Direct output to a file or pipe.\n" );
	exit( 1 );
}

int main( int argc, char **argv ) {

	int count;
	size_t code;

	if( argc != 2 || strcmp( argv[1], "--help" ) == 0 ) usage();
	
	if( 1 != sscanf( argv[1], "%d", &count )) usage();

	struct ad4080 adc = connect_4080();
		
	for( int i = 0; i < count; i += 1 ) {
	
		int32_t *samples = get_4080_samples( adc );
		code = fwrite( samples, sizeof( int32_t ), 
			BUFFER_SAMPLES, stdout );
		if( code != BUFFER_SAMPLES ) {
			perror( "binary_samples" );
			exit( 1 );
		}
	}
		
	disconnect_4080( adc );
			
	exit( 0 );
}
	
