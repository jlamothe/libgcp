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
 * FUNCTION DEFINITIONS
 */

uint16_t gen_crc(const uint8_t *data)
{
    return 0;                   /* TODO: implement this */
}

int check_crc(const uint8_t *data, uint16_t crc)
{
    if(data == NULL)
        return -1;
    return (crc == gen_crc(data)) ? 0 : 1;
}

/* jl */
