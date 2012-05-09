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

#include "gcp.h"
#include <stdlib.h>
#include "crc.h"

/*
 * FUNCTION PROTOTYPES
 */

/**
   \brief Reads the first byte of the preamble.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void read_preamble1(GCPConn *c, uint8_t b);

/**
   \brief Reads the second byte of the preamble.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void read_preamble2(GCPConn *c, uint8_t b);

/**
   \brief Reads the first byte of the data size.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void read_size1(GCPConn *c, uint8_t b);

/**
   \brief Reads the second byte of the data size.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void read_size2(GCPConn *c, uint8_t b);

/**
   \brief Reads the payload data.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void read_data(GCPConn *c, uint8_t b);

/**
   \brief Reads the first byte of the checksum.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void read_crc1(GCPConn *c, uint8_t b);

/**
   \brief Reads the second byte of the checksum.

   \param c A pointer to the GCPConn object.

   \param b The byte being read.
 */
static void read_crc2(GCPConn *c, uint8_t b);

/*
 * FUNCTION DEFINITIONS
 */

int gcp_init(GCPConn *c)
{
    if(c == NULL)
        return 1;
    c->in_buf = NULL;
    c->inbuf_size = 0;
    c->data_size = 0;
    c->bytes_read = 0;
    c->crc = 0;
    c->state = GCPConn::preamble1;
    c->data_avail = 0;
    return 0;
}

int gcp_read_byte(GCPConn *c, uint8_t b)
{
    if(c == NULL)
        return 1;
    c->data_avail = 0;
    switch(c->state)
    {
    case GCPConn::preamble1:
        read_preamble1(c, b);
        break;
    case GCPConn::preamble2:
        read_preamble2(c, b);
        break;
    case GCPConn::size1:
        read_size1(c, b);
        break;
    case GCPConn::size2:
        read_size2(c, b);
        break;
    case GCPConn::read_data:
        read_data(c, b);
        break;
    case GCPConn::crc1:
        read_crc1(c, b);
        break;
    case GCPConn::crc2:
        read_crc2(c, b);
        break;
    }
    return 0;
}

void read_preamble1(GCPConn *c, uint8_t b)
{
    if(b == 0x17)
        c->state = GCPConn::preamble2;
}

void read_preamble2(GCPConn *c, uint8_t b)
{
    if(b == 0x01)
        c->state = GCPConn::size1;
    else
        c->state = GCPConn::preamble1;
}

void read_size1(GCPConn *c, uint8_t b)
{
    c->data_size = b;
    c->data_size <<= 8;
    c->state = read_size2;
}

void read_size2(GCPConn *c, uint8_t b)
{
    c->data_size |= b;
    if(c->data_size == 0)
        c->state = GCPConn::crc1;
    else
    {
        c->state = GCPConn::read_data;
        c->bytes_read = 0;
    }
}

void read_data(GCPConn *c, uint8_t b)
{
    if(c->in_buf != NULL && c->inbuf_size > c->bytes_read)
        in_buf[c->bytes_read] = b;
    c->bytes_read++;
    if(c->bytes_read >= c->data_size)
        c->state = GCPConn::crc1;
}

void read_crc1(GCPConn *c, uint8_t b)
{
    c->crc = b;
    c->crc <<= 8;
    c->state = GCPConn::crc2;
}

void read_crc2(GCPConn *c, uint8_t b)
{
    c->crc |= b;
    if(check_crc(c->in_buf, c->in_buf->data_size, c->crc))
        data_avail = 1;
    c->state = GCPConn::preamble1;
}

/* jl */
