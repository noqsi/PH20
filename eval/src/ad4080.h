#ifndef AD4080_H

#define BUFFER_SAMPLES 16384

struct ad4080 {
	struct iio_context* context;
	struct iio_buffer* buffer;
};

struct ad4080 connect_4080( void );
void disconnect_4080( struct ad4080 adc );
int32_t *get_4080_samples( struct ad4080 adc );

#endif
