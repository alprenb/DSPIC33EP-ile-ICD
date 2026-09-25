#ifndef ICD_H
#define ICD_H

#include <stdint.h>
#include <stdbool.h>

// Parser durumlar?
typedef enum {
    WAIT_START_1,
    WAIT_START_2,
    READ_SEQ,
    READ_CMD,
    READ_LEN,
    READ_DATA,
    READ_CRC_H,
    READ_CRC_L
} ParserState;

// Paket yap?s? (Buras? cmd_handler için çok kritik)
typedef struct {
    uint8_t seq;
    uint8_t cmd;
    uint8_t len;
    uint8_t data[64];
    uint16_t received_crc;
} ICD_Packet;

// Prototipler
void ICD_ParseByte(uint8_t byte);
void ICD_CheckTimeout(void);
void ValidatePacket(ICD_Packet *packet);

#endif /* ICD_H */