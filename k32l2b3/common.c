#include "common.h"

#include <stdint.h>

uint32_t map(uint32_t value, uint32_t in_min, uint32_t in_max, uint32_t out_min, uint32_t out_max) {
	return (value - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
