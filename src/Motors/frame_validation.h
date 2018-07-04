/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   frame_validation.h
 * Author: yovany
 *
 * Created on July 2, 2018, 2:00 PM
 */

#ifndef FRAME_VALIDATION_H
#define FRAME_VALIDATION_H

#ifdef __cplusplus
extern "C" {
#endif

#define CRC16 0x8005
#define masterHeader 'M'
#define slaveHeader 'M'
#define frameTail ';'

uint16_t getCRC16(const uint8_t *data, uint16_t size) {
    uint16_t out = 0;
    int bits_read = 0, bit_flag;

    /* Sanity check: */
    if(data == NULL)
        return 0;

    while(size > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> bits_read) & 1; // item a) work from the least significant bits

        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if(bit_flag)
            out ^= CRC16;

    }

    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i) {
        bit_flag = out >> 15;
        out <<= 1;
        if(bit_flag)
            out ^= CRC16;
    }

    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>=1, j <<= 1) {
        if (i & out) crc |= j;
    }

    return crc;
}

uint16_t validFrame(uint8_t *received, uint8_t size) {
	uint16_t frameCRC = ((uint16_t) (received[size - 1] << 8)) | received[size - 2];
	uint16_t computedCRC = getCRC16(received, size - 2);
	if (frameCRC == computedCRC) {
		return computedCRC;
	}
	return 0;
}


#ifdef __cplusplus
}
#endif

#endif /* FRAME_VALIDATION_H */

