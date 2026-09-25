#include "cmd_handler.h"
#include <xc.h> // dsPIC donan?m registerlar? için

extern uint32_t validPacketCount;
extern uint32_t crcErrorCount;
extern uint32_t timeoutCount;
extern uint32_t sequenceErrorCount;
extern uint32_t uartOverrunCount;
extern uint32_t bufferOverflowCount;

uint16_t current_motor_rpm = 0;
uint8_t current_led_state = 0;

void CommandHandler(ICD_Packet *packet)
{
    switch(packet->cmd)
    {
        case CMD_GET_STATUS:
            // UART TX ile cihaz durumunu geri gönderme fonksiyonu ça?r?l?r
            break;

        case CMD_SET_LED:
            if(packet->len >= 1)
            {
                current_led_state = packet->data[0];
                // Örnek pin kontrolü: LATBbits.LATB15 = current_led_state;
            }
            break;

        case CMD_SET_MOTOR_SPEED:
            if(packet->len >= 2)
            {
                current_motor_rpm = ((uint16_t)packet->data[0] << 8) | packet->data[1];
                // Motor kontrol fonksiyonuna veya PWM register?na current_motor_rpm yaz?l?r
            }
            break;

        case CMD_GET_SENSOR:
            // Sensör verisini okuyup UART TX ile gönderme fonksiyonu ça?r?l?r
            break;

        case CMD_RESET_COUNTERS:
            validPacketCount = 0;
            crcErrorCount = 0;
            timeoutCount = 0;
            sequenceErrorCount = 0;
            uartOverrunCount = 0;
            bufferOverflowCount = 0;
            break;

        default:
            break;
    }
}