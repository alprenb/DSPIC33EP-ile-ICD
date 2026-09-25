 #include <xc.h>
#include <stdint.h>
#include "icd.h"
#include "ring_buffer.h"
#include "uart_isr.h"
void UART1_Init(void);
void UART1_Print(const char *str);
// 1 ms Tick Sayac? (Timer1 gibi bir timer kesmesinde art?r?lmal?)
volatile uint32_t systemTimeMs = 0;

// ?statistik Sayaçlar?
uint32_t validPacketCount = 0;
uint32_t crcErrorCount = 0;
uint32_t timeoutCount = 0;
uint32_t sequenceErrorCount = 0;
uint32_t uartOverrunCount = 0;
uint32_t bufferOverflowCount = 0;


    // Clock, Timer ve UART Init ça?r?lar?
int main(void)
{
    // Clock (PLL) ayar? burada veya config bitleriyle yap?lmal?
    UART1_Init();
    // Timer1 init (1 ms kesme, systemTimeMs++)

    UART1_Print("ICD READY\r\n");   // ilk test: terminalde görünmeli
    while(1)
    {
        // 1. Ring buffer'daki tüm baytlar? tüket
        while(RingBuffer_Available() > 0)
        {
            uint8_t b = RingBuffer_Get();
            ICD_ParseByte(b);
        }

        // 2. Yar?m paket kal?p kalmad???n? kontrol et
        ICD_CheckTimeout();

        // Di?er sistem görevleri
    }

    return 0;
}