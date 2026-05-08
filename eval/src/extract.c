#include "extract.h"
#include <stddef.h>

void running_sum( 
	sample_t *samples, 	// input array of length sample_size
	sample_t *sums, 	// output array of len sample_size-block_size+1
	unsigned sample_size, 
	unsigned block_size) {
	
	sample_t sum = 0;
	unsigned i;
	
	// prime the pump
	
	for( i = 0; i < block_size; i += 1 ) sum += *samples++;
	*sums++ = sum;
	
	// do the rest
	
	for( i = block_size; i < sample_size; i += 1 ) {
		sum -= *(samples - block_size);
		sum += *samples++;
		*sums++ = sum;
	}
}

void running_difference(
	sample_t *samples, 	// input array of length sample_size
	sample_t *differences, 	// output array of length sample_size - delta
	unsigned sample_size, 
	unsigned delta) {
	
	int i;
	sample_t lookback;
	
	samples += delta;	// 
	
	for( i = delta; i < sample_size; i += 1 ) {
		lookback = *(samples - delta);
		*differences++ = *samples++ - lookback;
	}
}


struct event *trigger_search( 
	sample_t *samples,	// array of processed samples
	unsigned sample_size,	// length of array
	struct parameters *p
	) {

	static enum { initial, idle, triggered } trigger_state = initial;
	
	static int i = 0;
	static struct event e;
	
// adjust thresholds for filter gain
	
	sample_t trig = p->shape * p->lld;
	sample_t untrig = trig - p->shape * p-> hys;
	
	for( ; i < sample_size; i += 1 ) {
		
		switch( trigger_state ) {
	
		case initial:
			if( samples[i] < untrig )
				trigger_state = idle;
			break;
			
		case idle:
			if( samples[i] < trig ) break; // nothing to do
			trigger_state = triggered;
			e.peak = e.rise = i;
			e.height = samples[i];
			break;
			
		case triggered:
			if( samples[i] < untrig ) {
				e.fall = i;
				trigger_state = idle;
				return &e;
			}
			if( samples[i] > e.height ) {
				e.peak = i;
				e.height = samples[ i ];
			}
		}	
	}
	
	// didn't find anything
	
	trigger_state = initial;	// for next time
	i = 0;
	return NULL;
}
	
