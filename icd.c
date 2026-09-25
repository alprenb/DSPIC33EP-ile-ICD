#include "icd.h"
#include "crc.h"
#include "cmd_handler.h"

// main.c veya sistem sayaç dosyas?ndan gelen de?i?kenler
extern volatile uint32_t systemTimeMs; // Her 1 ms'de Timer kesmesiyle artan sayaç
extern uint32_t validPacketCount;
extern uint32_t crcErrorCount;
extern uint32_t timeoutCount;
extern uint32_t sequenceErrorCount;

static ParserState state = WAIT_START_1;
static ICD_Packet current_packet;
static uint8_t data_index = 0;
static uint32_t lastByteTime = 0;

// SEQ kontrolü için statik durum de?i?kenleri
static uint8_t last_received_seq = 0;
static bool is_first_packet = true;

static void Parser_Reset(void) { // static olarak yazd?k bulundu?u icd.c dosyas?na özel di?er dosyalarda çal??maz
    
    state = WAIT_START_1;
    data_index = 0;

    memset(&current_packet, 0, sizeof (current_packet));

    lastByteTime = systemTimeMs;
}

void ICD_ParseByte(uint8_t byte) {
    // WAIT_START_1 haricindeyken gelen her byte için zaman damgas?n? güncelle
    lastByteTime = systemTimeMs;

    switch (state) {
        case WAIT_START_1:
            if (byte == 0x53) {
                state = WAIT_START_2;
            }
            break;

        case WAIT_START_2:
            if (byte == 0x41) {
                state = READ_SEQ;
            } else if (byte == 0x53) {
                state = WAIT_START_2;
            } else {
                state = WAIT_START_1;
            }
            break;

        case READ_SEQ:
            current_packet.seq = byte;
            state = READ_CMD;
            break;

        case READ_CMD:
            current_packet.cmd = byte;
            state = READ_LEN;
            break;

        case READ_LEN:
            current_packet.len = byte;
            if (current_packet.len > 64) {
                Parser_Reset();
            } else if (current_packet.len > 0) {
                data_index = 0;
                state = READ_DATA;
            } else {
                state = READ_CRC_H;
            }
            break;

        case READ_DATA:
            current_packet.data[data_index++] = byte;
            if (data_index >= current_packet.len) {
                state = READ_CRC_H;
            }
            break;

        case READ_CRC_H:
            current_packet.received_crc = (uint16_t) (byte << 8);
            state = READ_CRC_L;
            break;

        case READ_CRC_L:
            current_packet.received_crc |= byte;
            ValidatePacket(&current_packet);
            state = WAIT_START_1; // Yeni paket için ba?a dön
            break;
    }
}

void ICD_CheckTimeout(void) {
    // Paket gövdesi okunurken 10 ms geçerse s?f?rla
    if (state != WAIT_START_1) {
        if ((systemTimeMs - lastByteTime) > 100) {
            timeoutCount++;
            Parser_Reset();
        }
    }
}

void ValidatePacket(ICD_Packet *packet) {
    uint8_t crc_buffer[70];
    uint16_t length = 0;

    crc_buffer[length++] = packet->seq;
    crc_buffer[length++] = packet->cmd;
    crc_buffer[length++] = packet->len;

    for (uint8_t i = 0; i < packet->len; i++) {
        crc_buffer[length++] = packet->data[i];
    }

    uint16_t calculated_crc = CRC16_CCITT(crc_buffer, length);

    if (calculated_crc == packet->received_crc) {
        validPacketCount++;

        // SEQ Kontrolü: Yaln?zca CRC do?ruland?ktan sonra de?erlendirilir
        if (is_first_packet) {
            is_first_packet = false;
            last_received_seq = packet->seq;
        } else {
            uint8_t expected_seq = (uint8_t) (last_received_seq + 1);
            if (packet->seq != expected_seq) {
                sequenceErrorCount++;
            }
            last_received_seq = packet->seq;
        }

        // CommandHandler(packet); // STEP 9
    } else {
        crcErrorCount++;
        // CRC hatal? paketin SEQ bilgisi güvenilmez oldu?u için last_received_seq güncellenmez
    }
}