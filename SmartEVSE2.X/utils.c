/*
 * File:   utils.c
 * Author: Mike
 *
 * Created on 11 januari 2020, 21:44
 */

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"

void sprintfd(unsigned char *str, unsigned char *format, signed double value, unsigned char precision) {
    signed long val;

    val = (signed long)(value * pow10[precision + 1]);
    // Round value
    if(val < 0) val -= 5;
    else val += 5;
    val /= 10;
    // Split value
    if(precision > 0) sprintf(str, format, (signed int) (val / (signed long) pow10[precision]), (unsigned int) (abs(val) % pow10[precision]));
    else sprintf(str, format, (signed int) val);
}

/* triwave8: triangle (sawtooth) wave generator.  Useful for
           turning a one-byte ever-increasing value into a
           one-byte value that oscillates up and down.

           input         output
           0..127        0..254 (positive slope)
           128..255      254..0 (negative slope)
 */
unsigned char triwave8(unsigned char in) {
    if (in & 0x80) {
        in = 255u - in;
    }
    unsigned char out = in << 1;
    return out;
}

unsigned char scale8(unsigned char i, unsigned char scale) {
    return (((unsigned int) i) * (1 + (unsigned int) (scale))) >> 8;
}

/* easing functions; see http://easings.net

    ease8InOutQuad: 8-bit quadratic ease-in / ease-out function
 */
unsigned char ease8InOutQuad(unsigned char i) {
    unsigned char j = i;
    if (j & 0x80) {
        j = 255u - j;
    }
    unsigned char jj = scale8(j, j);
    unsigned char jj2 = jj << 1;
    if (i & 0x80) {
        jj2 = 255u - jj2;
    }
    return jj2;
}

// calculates 16-bit CRC of given data
// used for Frame Check Sequence on data frame
unsigned int crc16(unsigned char *buf, unsigned char len) {
    unsigned int crc = 0xffff;
    
    // Poly used is x^16+x^15+x^2+x
    for (int pos = 0; pos < len; pos++) {
        crc ^= (unsigned int)buf[pos];                                          // XOR byte into least sig. byte of crc

        for (int i = 8; i != 0; i--) {                                          // Loop over each bit
            if ((crc & 0x0001) != 0) {                                          // If the LSB is set
                crc >>= 1;                                                      // Shift right and XOR 0xA001
                crc ^= 0xA001;
            } else                                                              // Else LSB is not set
                crc >>= 1;                                                      // Just shift right
        }
    }        

    return crc;
}
