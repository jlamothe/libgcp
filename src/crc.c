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

#include "crc.h"
#include <stdlib.h>

/*
 * DEFINES
 */

#define CRC16 0x18005

/*
 * FUNCTION PROTOTYPES
 */

/**
   \brief Determines if a one 16-bit value is greater than or equal to
   another (according to CRC arithmetic).

   \param a The lvalue.

   \param b The rvalue.

   \return a non-zero value if a is greater than or equal to b, 0 if b
   is greater than a.
 */
static int crc_ge(uint16_t a, uint16_t b);

/**
   \brief Counts the minimum number of bits to represent a value.

   \param n The value.

   \return The number of bits.
 */
static int count_bits(uint16_t n)

/*
 * FUNCTION DEFINITIONS
 */

uint16_t gen_crc(const uint8_t *data, uint16_t size)
{
    uint32_t out = 0;
    int bits_read = 0;

    /* Sanity check: */
    if(data == NULL)
        return 0;

    while(size > 0)
    {

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> (7 - bits_read)) & 1)
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if(crc_ge(out, CRC16))
            out = out ^ CRC16;

    }
    return out;
}

int crc_ge(uint16_t a, uint16_t b)
{
    return count_bits(a) >= count_bits(b);
}

int count_bits(uint16_t n)
{
    int out = 0;
    while(n)
    {
        n >>= 1;
        out++;
    }
    return out;
}

int check_crc(const uint8_t *data, uint16_t size, uint16_t crc)
{
    if(data == NULL)
        return -1;
    return (crc == gen_crc(data, size)) ? 0 : 1;
}

/* jl */
