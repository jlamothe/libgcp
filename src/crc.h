/** \file crc.h */

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

#ifndef CRC_H
#define CRC_H

/*
 * INCLUDES
 */

#include <stdint.h>

#ifdef __cplusplus
#extern "C" {
#endif

/*
 * FUNCTION PROTOTYPES
 */

/**
   \brief Generates a CRC code for a block of data.

   \param data The data being used to generate the checksum.

   \return The generated CRC code.
 */
uint16_t gen_crc(const uint8_t *data);

/**
   \brief Validates a CRC code for a block of data.

   \param data The data being checked.

   \param crc The CRC being checked.

   \return 0 if the CRC code is valid, a non-zero value otherwise.
 */
int check_crc(const uint8_t *data, uint16_t crc);

#ifdef __cplusplus
}
#endif

#endif  /* CRC_H */

/* jl */
