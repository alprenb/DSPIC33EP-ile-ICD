/*
 * File:   dma_buffer.c
 * Author: YAZILIM-06
 *
 * Created on 22 Eylül 2026 Sal?, 10:24
 */


#include "xc.h"
#include "dma_buffer.h"
#include <xc.h>

#define DMA_BUFFER_SIZE 256

uint8_t dmaBuffer[DMA_BUFFER_SIZE] __attribute__((space(xmemory), aligned(256)));
static uint16_t tail = 0;

void UART_DMA_Init(void)
{
    DMA0CONbits.CHEN = 0;       
    
    DMA0CONbits.AMODE = 0;      
    DMA0CONbits.MODE  = 0;      
    DMA0CONbits.DIR   = 0;      
    DMA0CONbits.SIZE  = 1;      
    
    DMA0PAD = (volatile unsigned int)&U1RXREG; 
    DMA0REQ = 11;               
    
    DMA0STAL = __builtin_dmaoffset(dmaBuffer); 
    DMA0STAH = __builtin_dmapage(dmaBuffer);
    
    DMA0CNT = DMA_BUFFER_SIZE - 1; 
    
    IFS0bits.DMA0IF = 0;        
    IEC0bits.DMA0IE = 1;        
    
    DMA0CONbits.CHEN = 1;       
}

uint16_t DMA_Buffer_Available(void)
{
    // dsPIC33EP'de o anki DMA yazma adresi DSADR register'? ile okunabilir
    uint16_t current_dma_addr = DSADR0;
    uint16_t head = current_dma_addr - __builtin_dmaoffset(dmaBuffer);
    
    if (head >= tail)
    {
        return head - tail;
    }
    else
    {
        return (DMA_BUFFER_SIZE - tail) + head;
    }
}

uint8_t DMA_Buffer_Get(void)
{
    uint8_t data = 0;
    uint16_t current_dma_addr = DSADR0;
    uint16_t head = current_dma_addr - __builtin_dmaoffset(dmaBuffer);
    
    if (head != tail)
    {
        data = dmaBuffer[tail];
        tail = (tail + 1) % DMA_BUFFER_SIZE;
    }
    
    return data;
}

void __attribute__((interrupt, no_auto_psv)) _DMA0Interrupt(void)
{
    IFS0bits.DMA0IF = 0;
}
int main(void) {
    return 0;
}
