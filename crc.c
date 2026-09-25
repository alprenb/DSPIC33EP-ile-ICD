/*
 * File:   crc.c
 * Author: YAZILIM-06
 *
 * Created on 21 Eylül 2026 Pazartesi, 14:18
 */

#include "icd.h"
#include "crc.h" // Art?k fonksiyonu header dosyas?ndan tan?yor


#include "xc.h"


uint16_t CRC16_CCITT(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF; // ba?lang?ç deger ?art CRC-16/CCITT-FALSE algoritmas?n?n standard? ba?lang?ç de?erini 0xFFFF olarak tan?mlar.!!

    for(uint16_t i = 0; i < length; i++)
    {
        crc ^= ((uint16_t)data[i] << 8); // Gelen byte'? 16 bit yap?yoruz CRC'nin üst 8 bitine yerle?tirip XOR'la CCITT algoritmas? byte'? önce CRC register'?n high byte k?sm?na XOR eder.

        for(uint8_t bit = 0; bit < 8; bit++) // Her byte için 8 bitlik test döngüsü
        {
            if(crc & 0x8000) // MSB 1 mi? 100 0000
            {
                crc = (crc << 1) ^ 0x1021; // Kayd?r ve Polinom ile XOR'la polynomial uygula.0x1021 polynomial'? yaln?zca CRC register'?n en üst biti 1 oldu?unda uygulan?r. Bu, CRC-CCITT algoritmas?n?n kural?d?r.
            }
            else
            {
                crc = (crc << 1); // Sadece kayd?r msb 1 de?il polynimoal uygulama
            }
        }
    }

    return crc;
}


