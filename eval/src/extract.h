#include "evextd.h"

void running_sum( 
	sample_t *samples, 	// input array of length sample_size
	sample_t *sums, 	// output array of len sample_size-block_size+1
	unsigned sample_size, 
	unsigned block_size);

void running_difference(
	sample_t *samples, 	// input array of length sample_size
	sample_t *differences, 	// output array of length sample_size - delta
	unsigned sample_size, 
	unsigned delta);

struct event *trigger_search( 
	sample_t *samples,	// array of processed samples
	unsigned sample_size,	// length of array
	struct parameters *p;
	);
