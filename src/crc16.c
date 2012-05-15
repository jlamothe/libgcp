/** \file crc.c */

/*
GCP (Generic Communications Protocol) Library

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
                   const CRC16Params *params)
{
    uint16_t out, i;
    if(data == NULL || params == NULL)
        return 0;
    out = params->prefix;
    if(params->flip_bytes)
        for(i = size - 1; i > 0; i--)
            out = process_crc16_byte(out,
                                     data[i],
                                     params->poly,
                                     params->flip_bits);
    else
        for(i = 0; i < size; i++)
            out = process_crc16_byte(out,
                                     data[i],
                                     params->poly,
                                     params->flip_bits);
    for(i = 0; i < 2; i++)
        out = process_crc16_byte(out, 0, params->poly, 0);
    if(params->flip_output)
        out = flip_16bit(out);
    return out;
}

uint16_t process_crc16_byte(uint16_t prev,
                            uint8_t byte,
                            uint16_t poly,
                            int msb_first)
{
    int i, bit_flag;
    if(msb_first)
        byte = flip_16bit(byte);
    for(i = 0; i < 8; i++)
    {
        bit_flag = prev >> 15;
        prev <<= 1;
        prev |= (byte >> i) & 1;
        if(bit_flag)
            prev ^= poly;
    }
    return prev;
}

uint16_t flip_16bit(uint16_t val)
{
    uint16_t out = 0;
    int i;
    for(i = 0; i < 16; i++)
        if(val & (1 << i))
            out |= 0x8000 >> i;
    return out;
}

int check_crc16(const uint8_t *data,
                uint16_t size,
                const CRC16Params *params,
                uint16_t crc)
{
    if(data == NULL || params == NULL)
        return -1;
    return (crc == gen_crc16(data, size, params)) ? 0 : 1;
}

/* jl */
