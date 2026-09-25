#include <xc.h>
#include <stdint.h>
#include "ring_buffer.h"
#define FCY 3685000ULL
#define UART_BAUD 9600
extern uint32_t uartOverrunCount;

void __attribute__((interrupt, no_auto_psv)) _U1RXInterrupt(void)
{
    if (U1STAbits.OERR)
    {
        U1STAbits.OERR = 0;
        uartOverrunCount++;
    }

    while (U1STAbits.URXDA)
    {
        uint8_t byte = U1RXREG;
        RingBuffer_Put(byte);
    }

    IFS0bits.U1RXIF = 0;
}
void UART1_Init(void)
{
    ANSELE = 0x0000;

    __builtin_write_OSCCONL(OSCCON & 0xBF);   // PPS kilidi aç
    _U1RXR  = 81;                              // RPI81 -> U1RX
    _RP80R  = 0x01;                            // RP80  -> U1TX
    __builtin_write_OSCCONL(OSCCON | 0x40);   // PPS kilidi kapat

    U1MODE = 0;
    U1STA  = 0;
    U1MODEbits.BRGH = 1;
    U1BRG = (uint16_t)((FCY / (4UL * UART_BAUD)) - 1UL);

    U1STAbits.URXISEL = 0;
    IPC2bits.U1RXIP = 5;
    IFS0bits.U1RXIF = 0;
    IEC0bits.U1RXIE = 1;

    U1MODEbits.UARTEN = 1;
    U1STAbits.UTXEN   = 1;
}
void UART1_Write(uint8_t data)
{
    // while(U1STAbits.UTXBF);  // Kilitlenmeyi test etmek için ?imdilik kapal?
    U1TXREG = data;
}

void UART1_Print(const char *str) 
{
    while(*str) 
    {
        UART1_Write(*str++);
    }
}
