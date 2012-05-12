/** \file crc.c */

/*
GCP (Generic Communications Protocol)

Copyright (C) 2012 Jonathan Lamothe <jonathan@jlamothe.net>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as
published by the Free Software Foundation, either version 3 of the
License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this program.  If not, see:
http://www.gnu.org/licenses/
*/

/*
 * INCLUDES
 */

#include <config.h>

#if !HAVE_STDINT_H
#error missing stdint.h
#endif

#include "crc16.h"

#if HAVE_STDLIB_H
#include <stdlib.h>
#else
#error missing stdlib.h
#endif

/*
 * DEFINES
 */

#define CRC16 0x8005

/*
 * FUNCTION DEFINITIONS
 */

uint16_t gen_crc16(const uint8_t *data, uint16_t size)
{
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
        out |= (*data >> (7 - bits_read)) & 1;

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
    return out;
}

int check_crc16(const uint8_t *data, uint16_t size, uint16_t crc)
{
    if(data == NULL)
        return -1;
    return (crc == gen_crc16(data, size)) ? 0 : 1;
}

/* jl */
