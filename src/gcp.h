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

    /** \brief Communication state. */
    typedef enum {

        /** \brief Reading first byte of the preamble. */
        gcp_preamble1,

        /** \brief Reading second byte of the preamble. */
        gcp_preamble2,

        /** \brief Reading first byte of the payload size. */
        gcp_size1,

        /** \brief Reading second byte of the payload size. */
        gcp_size2,

        /** \brief Reading payload data. */
        gcp_payload,

        /** \brief Reading first byte of the checksum. */
        gcp_crc1,

        /** \brief Reading second byte of the checksum. */
        gcp_crc2,

    } GCPFrameState;



    typedef struct
    {

        /** \brief Receive buffer. */
        uint8_t *recv_buf;

        /** \brief Send buffer. */
        uint8_t *send_buf;

        /** \brief Receive buffer size. */
        uint16_t recv_size;

        /**
           \brief Send buffer size.

           \note This is the size of the data in the send buffer, not
           the size of the buffer itself.
         */
        uint16_t send_size;

        /** \brief Size of the data in the receive buffer. */
        uint16_t data_size;

        /** \brief Number of payload bytes received. */
        uint16_t bytes_rcvd;

        /** \brief Number of payload bytes sent. */
        uint16_t bytes_sent;

        /** \brief The crc checksum of the received data. */
        uint16_t recv_crc;

        /** \brief The crc checksum of the data being sent. */
        uint16_t send_crc;

        /** \brief The receive state. */
        GCPFrameState recv_state;

        /** \brief The send state. */
        GCPFrameState send_state;

        /** \brief When true, indicates that the receive buffer is
            being written to and should not be read from. */
        unsigned recv_lock : 1;

        /** \brief When true, indicates that the receive buffer is
            being read from and should not be written to. */
        unsigned send_lock : 1;

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

       \param b The byte from the stream to be processed.

       \return 0 on success; a non-zero value on failure.
     */
    int gcp_recv_byte(GCPConn *c, uint8_t b);

    /**
       \brief Calculates the next byte to be sent to the stream.

       \param c A pointer to the connection.

       \return The next byte (or 0 on failure).
     */
    uint8_t gcp_send_byte(GCPConn *c);

#ifdef __cplusplus
}
#endif

#endif  /* GCP_H */

/* jl */
