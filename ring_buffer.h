#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stdint.h>

void RingBuffer_Put(uint8_t data);
uint8_t RingBuffer_Get(void);
uint16_t RingBuffer_Available(void);

#endif /* RING_BUFFER_H */