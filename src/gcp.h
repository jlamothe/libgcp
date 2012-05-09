/** \file gcp.h */

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

#ifndef GCP_H
#define GCP_H

/*
 * INCLUDES
 */

#include<stdint.h>

#ifdef _cplusplus
extern "C" {
#endif

    /*
     * TYPEDEFS
     */
    typedef struct
    {
        /** \brief Input buffer. */
        uint8_t *in_buf;
        /** \brief Input buffer size. */
        uint16_t inbuf_size;
        /** \brief Size of the data in the input buffer. */
        uint16_t data_size;
        /** \brief Bytes read (not including packet overhead). */
        uint16_t bytes_read;
        /** \brief The crc checksum. */
        uint16_t crc;
        /** \brief Input state. */
        enum {
            /** \brief Reading first byte of the preamble. */
            preamble1,
            /** \brief Reading second byte of the preamble. */
            preamble2,
            /** \brief Reading first byte of the payload size. */
            size1,
            /** \brief Reading second byte of the payload size. */
            size2,
            /** \brief Reading payload data. */
            read_data,
            /** \brief Reading first byte of the checksum. */
            crc1,
            /** \brief Reading second byte of the checksum. */
            crc2,
        } state;
        /** \brief Indicates that the data from an input frame is
            available for reading. */
        unsigned data_avail : 1;
    } GCPConn;

    /*
     * FUNCTION PROTOTYPES
     */

    /**
       \brief Initializes a GCPConn object.

       \param A pointer to the object to be initialized.

       \return 0 on success; a non-zero value on failure.
     */
    int gcp_init(GCPConn *c);

    /**
       \brief Processes a byte from the stream.

       \param c A pointer to the connection.

       \param b The byte to be processed.

       \return 0 on success; a non-zero value on failure.
     */
    int gcp_read_byte(GCPConn *c, uint8_t b);

#ifdef __cplusplus
}
#endif

#endif  /* GCP_H */

/* jl */
