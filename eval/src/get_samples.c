#include <iio.h>
#include <stdio.h>
#include "ad4080.h"

/*
 Grab a buffer full of samples and print the values.
*/

int main( int argc, char **argv ) {

	struct ad4080 adc = connect_4080();
	
	int32_t *samples = get_4080_samples( adc );
	
	for( int i = 0; i < BUFFER_SAMPLES; i += 1 )
		printf( "%d\n", samples[ i ] );
	
	disconnect_4080( adc );
			
	exit( 0 );
}
	
