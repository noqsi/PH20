#include <iio.h>
#include <stdio.h>
#include "ad4080.h"

struct ad4080 connect_4080( void ) {

	struct ad4080 adc;

	char *zed_uri = getenv( "AD4080URI" );
	if( !zed_uri ) {
		fprintf( stderr, "No AD4080URI in environment\n" );
		exit( 1 );
	}

	adc.context = iio_create_context_from_uri( zed_uri );
	if( !adc.context ) {
		perror( zed_uri );
		exit( 1 );
	}
//	printf( "Created context.\n" );
	
	const char device_name[] = "ad4080";
	
	struct iio_device *device =
		iio_context_find_device( adc.context, device_name );		
	if( !device ) {
		fprintf( stderr, "No %s device.\n", device_name );
		exit( 1 );
	}
//	printf( "Found pulse ADC.\n" );

	
	// AD4080 has only one channel
	
	struct iio_channel* pulsechannel = 
		iio_device_get_channel( device, 0 );
	
	if( !pulsechannel ) {
		fprintf( stderr, "Can't open pulse channel." );
		exit( 1 );
	}
//	printf( "Opened pulse channel.\n" );

	iio_channel_enable( pulsechannel );
	
//	(void) getchar();	

	adc.buffer= 
		iio_device_create_buffer( device, BUFFER_SAMPLES, false );
	if( !adc.buffer ) {
		perror( "Cannot create ADC buffer" );
		exit( 1 );
	}
//	printf( "Created pulse buffer.\n" );
	
	return adc;
}


int32_t *get_4080_samples( struct ad4080 adc ) {
	
	ssize_t pbytes = iio_buffer_refill( adc.buffer );
	if( pbytes < 0 ) {
		char err[100];
		iio_strerror( pbytes, err, sizeof err );
		fprintf( stderr, "Cannot refill: %s\n", err );
		exit( 1 );
	}
	if( pbytes != BUFFER_SAMPLES * sizeof( int32_t )) {
		fprintf( stderr, "Only %ld bytes filled\n", pbytes );
		exit( 1 );
	}
	
	return iio_buffer_start( adc.buffer );
}


void disconnect_4080( struct ad4080 adc ) {
	
	iio_buffer_destroy( adc.buffer );
	iio_context_destroy( adc.context );
}
	
