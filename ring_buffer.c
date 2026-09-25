/*
 * File:   ring_buffer.c
 * Author: YAZILIM-06
 *
 * Created on 22 Eylül 2026 Sal?, 10:00
 */


#include "xc.h"


#include "ring_buffer.h"
#include <stdint.h>

#define BUFFER_SIZE 256

extern uint32_t bufferOverflowCount;

static uint8_t buffer[BUFFER_SIZE];
static uint16_t head = 0;
static uint16_t tail = 0;

void RingBuffer_Put(uint8_t data)
{
    uint16_t next_head = (head + 1) % BUFFER_SIZE;

    if (next_head == tail)
    {
        bufferOverflowCount++;
    }
    else
    {
        buffer[head] = data;
        head = next_head;
    }
}

uint8_t RingBuffer_Get(void)
{
    uint8_t data = 0;
    
    if (head != tail)
    {
        data = buffer[tail];
        tail = (tail + 1) % BUFFER_SIZE;
    }
    
    return data;
}

uint16_t RingBuffer_Available(void)
{
    if (head >= tail)
    {
        return head - tail;
    }
    else
    {
        return (BUFFER_SIZE - tail) + head;
    }
}