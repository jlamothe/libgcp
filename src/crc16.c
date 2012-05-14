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
 * FUNCTION DEFINITIONS
 */

uint16_t gen_crc16(const uint8_t *data,
                   uint16_t size,
                   uint16_t poly,
                   uint16_t pre)
{
    uint16_t i;
    if(data == NULL)
        return 0;
    for(i = 0; i < size; i++)
        pre = process_crc16_byte(pre, data[i], poly);
    for(i = 0; i < 2; i++)
        pre = process_crc16_byte(pre, 0, poly);
    return pre;
}

uint16_t process_crc16_byte(uint16_t prev,
                            uint8_t byte,
                            uint16_t poly)
{
    int i, bit_flag;
    for(i = 0; i < 8; i++)
    {
        bit_flag = prev >> 15;
        prev <<= 1;
        prev |= (byte >> (7 - i)) & 1;
        if(bit_flag)
            prev ^= poly;
    }
    return prev;
}

int check_crc16(const uint8_t *data,
                uint16_t size,
                uint16_t poly,
                uint16_t pre,
                uint16_t crc)
{
    if(data == NULL)
        return -1;
    return (crc == gen_crc16(data, size, poly, pre)) ? 0 : 1;
}

/* jl */
