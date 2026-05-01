int init_my_port (unsigned port)
{
// Create socket

    int             mys = socket (AF_INET, SOCK_STREAM, 0);
    if (mys < 0) {
	perror ("init_my_port; socket");
	exit (1);
    }

// Bind it to the given port

    struct sockaddr_in sa;
    memset((char *) &sa, 0, sizeof(struct sockaddr_in));
    sa.sin_family = PF_INET;
    /* in_addr has been zeroed into being INADDR_ANY */
    sa.sin_port = htons(port);
    if (bind (mys, (struct sockaddr *) &sa, sizeof (sa)) < 0) {
	perror ("init_my_port; bind");
	exit (1);
    }
    
// Make it listen

    if( listen (mys, 1) != 0 ) {
    	perror( "init_my_port; listen");
	exit( 1 );
    }
    
    return (mys);
}

FILE *connect_client( int sock ) {
	
	struct sockaddr remote;
	socklen_t remotel;
	
	int fd = accept( sock, &remote, &remotel );
	if( fd < 0 ) {
		perror( "accept" );
		exit( 1 );
	}
	FILE *stream = fdopen( fd, "r+" );
	if( !stream ) {
		perror( "open input");
		exit( 1 );
	}
}

void check_id( uint32 id, uint32 expected ) {
	if( id == expected ) return;
	fprintf( stderr, "Expected packet ID %X, saw %X\n", expected, id );
	exit( 1 );
}

void go( struct ad4080 adc, struct parameters p, FILE *client ) {
// These are a bit bigger than necessary, but it's simple to make them fixed size
	sample_t sums[ BUFFER_SAMPLES ], differences[ BUFFER_SAMPLES ];	
	
	for(;;) {
		int32t *samples = get_4080_samples( adc );
		running_sum( samples, sums, BUFFER_SAMPLES, p.shape );
		running_differences( sums, differences, 
			BUFFER_SAMPLES - p.shape + 1, p.shape );
		
		
	}
}

int main( int argc, char *argv[] ) {

	int sock = init_my_port ( EXTRACTOR_PORT );
	struct ad4080 adc = connect_4080();
	
// Main loop

	for(;;) {
		FILE *client = connect_client( sock );
		
// Read the start message
		uint32_t id;
		int items = fread( &id, sizeof id, 1, client );
		if( items != 1 ) {
			perror( "start");
			exit( 1 );
		}
		check_id( id, START_ID );
		
		struct parameters p;
		int items = fread( &p, sizeof p, 1, client );
		if( items != 1 ) {
			perror( "parameters");
			exit( 1 );
		}
		
		go( adc, p, client );	// do the work
		
		fclose( client );	// done	
	}	
}