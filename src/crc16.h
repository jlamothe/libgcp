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

#ifndef CRC16_H
#define CRC16_H

/*
 * INCLUDES
 */

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

    /*
     * FUNCTION PROTOTYPES
     */

    /**
       \brief Generates a CRC code for a block of data.

       \param data The data being used to generate the checksum.

       \param size The size (in bytes) of the data being CRC'd.

       \param poly The polynomial to be used for the CRC.

       \param pre The value to prepend to the data string.

       \return The generated CRC code.
     */
    uint16_t gen_crc16(const uint8_t *data,
                       uint16_t size,
                       uint16_t poly,
                       uint16_t pre);

    /**
       \brief Process a single byte in a CRC16 checksum.

       \param prev The previously calculated value.

       \param byte The byte being processed.

       \param msb_first Set to a non-zero value when the most
       significant bit of the data is to be processed first.

       \param poly The polynomial being used for the CRC.
     */
    uint16_t process_crc16_byte(uint16_t prev,
                                uint8_t byte,
                                uint16_t poly,
                                int msb_first);

    /**
       \breif Reverses the bits in a uint16_t value.

       \param val The value to be flipped.

       \return The flipped value.
     */
    uint16_t flip_16bit(uint16_t val);

    /**
       \brief Validates a CRC code for a block of data.

       \param data The data being checked.

       \param size The size (in bytes) of the data being checked.

       \param poly The polynomial to be used for the CRC.

       \param pre The value to prepend to the data string.

       \param crc The CRC being checked.

       \return 0 if the CRC code is valid, a non-zero value otherwise.
     */
    int check_crc16(const uint8_t *data,
                    uint16_t size,
                    uint16_t poly,
                    uint16_t pre,
                    uint16_t crc);

#ifdef __cplusplus
}
#endif

#endif  /* CRC16_H */

/* jl */
