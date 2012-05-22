/** \file crc16.h */

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
     * TYPEDEFS
     */

    /** \brief CRC-16 parameters. */
    typedef struct
    {

        /** \brief Prefix to be added to the data. */
        uint16_t prefix;

        /** \brief Polynomial to be used. */
        uint16_t poly;

        /** \brief When true, indicates that the most significant bit
            of each byte should be processed first. */
        unsigned flip_bits : 1;

        /** \brief When true, indicates that the the bytes at the
            highest index in the buffer should be processed first. */
        unsigned flip_bytes : 1;

        /** \brief When true, indicates that the bits in the output
            should be reversed after calculating. */
        unsigned flip_output : 1;

    } CRC16Params;

    /*
     * FUNCTION PROTOTYPES
     */

    /**
       \brief Generates a CRC-16 checksum for a block of data.

       \param data The data being used to generate the checksum.

       \param size The size (in bytes) of the data being CRC'd.

       \param params A pointer to the checksum parameters.

       \return The generated checksum on success; 0 on failure (data
       or params are NULL pointers).
     */
    uint16_t crc16_gen(const uint8_t *data,
                       uint16_t size,
                       const CRC16Params *params);

    /**
       \brief Validates a CRC-16 checksum for a block of data.

       \param data The data being checked.

       \param size The size (in bytes) of the data being checked.

       \param params A pointer to the checksum parameters.

       \param crc The checksum being checked.

       \return 0 if the checksum is valid; a negative value if data or
       params are NULL pointers; a positive value on an incorrect
       checksum.
     */
    int crc16_check(const uint8_t *data,
                    uint16_t size,
                    const CRC16Params *params,
                    uint16_t crc);

    /**
       \brief Process a single byte in a CRC-16 checksum calculation.

       \param prev The previously calculated value.

       \param byte The byte being processed.

       \param msb_first Set to a non-zero value when the most
       significant bit of the data is to be processed first.

       \param poly The polynomial being used for the checksum.

       \return The newly calculated value.
     */
    uint16_t crc16_process_byte(uint16_t prev,
                                uint8_t byte,
                                uint16_t poly,
                                int msb_first);

    /**
       \brief Processes the remaining 16 bits of padding in a CRC-16
       checksum.

       \param prev The value calculated before the flush.

       \param poly The polynomial to use.

       \param flip If non-zero, reverses the bit order of the output.

       \return The calculated checksum.
     */
    uint16_t crc16_flush(uint16_t prev, uint16_t poly, int flip);

#ifdef __cplusplus
}
#endif

#endif  /* CRC16_H */

/* jl */
