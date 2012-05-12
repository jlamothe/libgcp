/** \file gcp.c */

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

#include "gcp.h"

#if HAVE_STDLIB_H
#include <stdlib.h>
#else
#error missing stdlib.h
#endif

#include "crc16.h"

/*
 * DEFINES
 */

/** \brief The polynomial to be used for the CRC calculation. */
#define POLY 0x8005

/** \brief The prepend value to be used for the CRC calculation. */
#define PRE 1

/*
 * FUNCTION PROTOTYPES
 */

/**
   \brief Reads the first byte of the preamble.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void recv_preamble1(GCPConn *c, uint8_t b);

/**
   \brief Reads the second byte of the preamble.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void recv_preamble2(GCPConn *c, uint8_t b);

/**
   \brief Reads the first byte of the data size.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void recv_size1(GCPConn *c, uint8_t b);

/**
   \brief Reads the second byte of the data size.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void recv_size2(GCPConn *c, uint8_t b);

/**
   \brief Reads the payload data.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void recv_payload(GCPConn *c, uint8_t b);

/**
   \brief Reads the first byte of the checksum.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void recv_crc1(GCPConn *c, uint8_t b);

/**
   \brief Reads the second byte of the checksum.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void recv_crc2(GCPConn *c, uint8_t b);

/**
   \brief Returns the first byte of the preamble to be sent.

   \param c A pointer to the GCPConn object.

   \return The first byte of the preamble.
 */
static uint8_t send_preamble1(GCPConn *c);

/**
   \brief Returns the second byte of the preamble to be sent.

   \param c A pointer to the GCPConn object.

   \return The second byte of the preamble.
 */
static uint8_t send_preamble2(GCPConn *c);

/**
   \brief Returns the first byte of the payload size to be sent.

   \param c A pointer to the GCPConn object.

   \return The first byte of the payload size.
 */
static uint8_t send_size1(GCPConn *c);

/**
   \brief Returns the second byte of the payload size to be sent.

   \param c A pointer to the GCPConn object.

   \return The second byte of the payload size.
 */
static uint8_t send_size2(GCPConn *c);

/**
   \brief Returns the next byte of the payload to be sent.

   \param c A pointer to the GCPConn object.

   \return The next byte of the payload.
 */
static uint8_t send_payload(GCPConn *c);

/**
   \brief Returns the first byte of the checksum to be sent.

   \param c A pointer to the GCPConn object.

   \return The first byte of the checksum.
 */
static uint8_t send_crc1(GCPConn *c);

/**
   \brief Returns the second byte of the checksum to be sent.

   \param c A pointer to the GCPConn object.

   \return The second byte of the checksum.
 */
static uint8_t send_crc2(GCPConn *c);

/*
 * FUNCTION DEFINITIONS
 */

int gcp_init(GCPConn *c)
{
    if(c == NULL)
        return 1;
    c->recv_buf = NULL;
    c->send_buf = NULL;
    c->recv_size = 0;
    c->send_size = 0;
    c->data_size = 0;
    c->bytes_rcvd = 0;
    c->bytes_sent = 0;
    c->recv_crc = 0;
    c->send_crc = 0;
    c->recv_state = gcp_preamble1;
    c->send_state = gcp_preamble1;
    c->recv_lock = 1;
    c->send_lock = 0;
    return 0;
}

int gcp_recv_byte(GCPConn *c, uint8_t b)
{
    if(c == NULL)
        return 1;
    c->recv_lock = 1;
    switch(c->recv_state)
    {
    case gcp_preamble1:
        recv_preamble1(c, b);
        break;
    case gcp_preamble2:
        recv_preamble2(c, b);
        break;
    case gcp_size1:
        recv_size1(c, b);
        break;
    case gcp_size2:
        recv_size2(c, b);
        break;
    case gcp_payload:
        recv_payload(c, b);
        break;
    case gcp_crc1:
        recv_crc1(c, b);
        break;
    case gcp_crc2:
        recv_crc2(c, b);
        break;
    }
    return 0;
}

void recv_preamble1(GCPConn *c, uint8_t b)
{
    if(b == 0x17)
        c->recv_state = gcp_preamble2;
}

void recv_preamble2(GCPConn *c, uint8_t b)
{
    if(b == 0x01)
        c->recv_state = gcp_size1;
    else if(b != 0x17)
        c->recv_state = gcp_preamble1;
}

void recv_size1(GCPConn *c, uint8_t b)
{
    c->data_size = b;
    c->data_size <<= 8;
    c->recv_state = gcp_size2;
}

void recv_size2(GCPConn *c, uint8_t b)
{
    c->data_size |= b;
    if(c->data_size == 0)
        c->recv_state = gcp_crc1;
    else
    {
        c->recv_state = gcp_payload;
        c->bytes_rcvd = 0;
    }
}

void recv_payload(GCPConn *c, uint8_t b)
{
    if(c->recv_buf != NULL && c->recv_size > c->bytes_rcvd)
        c->recv_buf[c->bytes_rcvd] = b;
    c->bytes_rcvd++;
    if(c->bytes_rcvd >= c->data_size)
        c->recv_state = gcp_crc1;
}

void recv_crc1(GCPConn *c, uint8_t b)
{
    c->recv_crc = b;
    c->recv_crc <<= 8;
    c->recv_state = gcp_crc2;
}

void recv_crc2(GCPConn *c, uint8_t b)
{
    c->recv_crc |= b;
    if(check_crc16(c->recv_buf,
                   c->data_size,
                   POLY,
                   PRE,
                   c->recv_crc))
        c->recv_lock = 0;
    c->recv_state = gcp_preamble1;
}

uint8_t gcp_send_byte(GCPConn *c)
{
    if(c == NULL)
        return 0;
    c->send_lock = 1;
    switch(c->send_state)
    {
    case gcp_preamble1:
        return send_preamble1(c);
    case gcp_preamble2:
        return send_preamble2(c);
    case gcp_size1:
        return send_preamble1(c);
    case gcp_size2:
        return send_preamble1(c);
    case gcp_payload:
        return send_payload(c);
    case gcp_crc1:
        return send_crc1(c);
    case gcp_crc2:
        return send_crc2(c);
    }
    return 0;
}

uint8_t send_preamble1(GCPConn *c)
{
    c->send_state = gcp_preamble2;
    return 0x17;
}

uint8_t send_preamble2(GCPConn *c)
{
    c->send_state = gcp_size1;
    return 0x01;
}

uint8_t send_size1(GCPConn *c)
{
    c->send_state = gcp_size2;
    return c->send_size >> 8;
}

uint8_t send_size2(GCPConn *c)
{
    if(c->send_size == 0)
        c->send_state = gcp_crc1;
    else
    {
        c->send_state = gcp_payload;
        c->bytes_sent = 0;
    }
    return c->send_size;
}

uint8_t send_payload(GCPConn *c)
{
    c->bytes_sent++;
    if(c->bytes_sent >= c->send_size)
        c->send_state = gcp_crc1;
    if(c->send_buf == NULL)
        return 0;
    return c->send_buf[c->bytes_sent - 1];
}

uint8_t send_crc1(GCPConn *c)
{
    c->send_crc = gen_crc16(c->send_buf, c->send_size, POLY, PRE);
    c->send_state = gcp_crc2;
    return c->send_crc >> 8;
}

uint8_t send_crc2(GCPConn *c)
{
    c->send_state = gcp_preamble1;
    c->send_lock = 0;
    return c->send_crc;
}

/* jl */
