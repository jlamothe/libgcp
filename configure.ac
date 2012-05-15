#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

dnl GCP (Generic Communications Protocol) Library

dnl Copyright (C) 2012 Jonathan Lamothe <jonathan@jlamothe.net>

dnl This program is free software: you can redistribute it and/or
dnl modify it under the terms of the GNU Lesser General Public License
dnl as published by the Free Software Foundation, either version 3 of
dnl the License, or (at your option) any later version.

dnl This program is distributed in the hope that it will be useful,
dnl but WITHOUT ANY WARRANTY; without even the implied warranty of
dnl MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
dnl Lesser General Public License for more details.

dnl You should have received a copy of the GNU Lesser General Public
dnl License along with this program.  If not, see:
dnl http://www.gnu.org/licenses/

AC_PREREQ([2.68])
AC_INIT([libgcp], [0.1], [jonathan@jlamothe.net])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
LT_INIT
AC_CONFIG_SRCDIR([src/gcp.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Checks for programs.
AC_PROG_CC

# Checks for libraries.

# Checks for header files.
AC_CHECK_HEADERS([stdint.h stdlib.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 doc/Makefile
                 src/Makefile])
AC_OUTPUT
