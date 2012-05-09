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

int gcp_init(GCPConnection *c)
{
    if(c == NULL)
        return 1;
    c->in_buf = NULL;
    c->inbuf_size = 0;
    c->bytes_read = 0;
    c->state = GCPConnection::preamble1;
    c->data_avail = 0;
    return 0;
}

int gcp_read_byte(GCPConnection *c, unsigned char b)
{
    return 0;
}

/* jl */
