#include <stdint.h>

typedef int32_t sample_t;

#define EXTRACTOR_PORT 10951

// packet types

#define START_ID 0xC0DE0001
#define DUMP_ID 0xC0DE0002
#define DATA_ID 0xC0DE0003
#define DUMP_REQUEST_ID 0xC0DE0004

// Packet formats: see ExtractorPacket.md for more detail

struct parameters {
	uint32_t shape;
	sample_t lld, hys, dump;	
};

struct event {
	uint32_t rise, peak, fall;
	sample_t height;
}