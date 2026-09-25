#ifndef CMD_HANDLER_H
#define CMD_HANDLER_H

#include <stdint.h>
#include "icd.h" // ICD_Packet'i tan?mas? için bu ?art

#define CMD_GET_STATUS       0x01
#define CMD_SET_LED          0x02
#define CMD_SET_MOTOR_SPEED  0x03
#define CMD_GET_SENSOR       0x04
#define CMD_RESET_COUNTERS   0x05

// Derleyici art?k icd.h'den dolay? ICD_Packet'in ne oldu?unu biliyor
void CommandHandler(ICD_Packet *packet);

#endif /* CMD_HANDLER_H */