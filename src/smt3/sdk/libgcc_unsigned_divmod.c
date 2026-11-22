/* SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Unsigned EE integer division, adapted 2026-11-22 from the GCC sources
 * gcc/libgcc2.c and gcc/longlong.h at upstream revision
 * 380f6ad3dde6db991434068c4f3776c6762fa564. Scoped to the native EE32 word
 * layout and the normalized denominator path. Existing native bit-length
 * table definitions stay in libgcc_longlong.c. The libgcc2.c linking
 * permission below is retained as written; it is not attributed to
 * longlong.h. See docs/licenses/GCC-frame-COPYING.
 */
/* More subroutines needed by GCC output code on some machines.  */
/* Compile this one with gcc.  */
/* Copyright (C) 1989, 92, 93, 94, 95, 96, 97, 98, 1999, 2000
   Free Software Foundation, Inc.

This file is part of GNU CC.

GNU CC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

In addition to the permissions in the GNU General Public License, the
Free Software Foundation gives you unlimited permission to link the
compiled version of this file into combinations with other programs,
and to distribute those combinations without any restriction coming
from the use of this file.  (The General Public License restrictions
do apply in other respects; for example, they cover modification of
the file, and distribution when not linked into a combine
executable.)

GNU CC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU CC; see the file COPYING.  If not, write to
the Free Software Foundation, 59 Temple Place - Suite 330,
Boston, MA 02111-1307, USA.  */

/* longlong.h -- definitions for mixed size 32/64 bit arithmetic.
   Copyright (C) 1991,92,94,95,96,97,98,99, 2000 Free Software Foundation, Inc.

   This definition file is free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public
   License as published by the Free Software Foundation; either
   version 2, or (at your option) any later version.

   This definition file is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied
   warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
   See the GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place - Suite 330,
   Boston, MA 02111-1307, USA.  */

#include "sdk_libgcc_divmod_internal.h"

extern const u8 libgccUdivdiBitLengths[256];
extern const u8 libgccUmoddiBitLengths[264];

UDWtype
__udivdi3 (UDWtype n, UDWtype d)
{
  return __udivmoddi4 (n, d, (UDWtype *) 0, libgccUdivdiBitLengths);
}

UDWtype
__umoddi3 (UDWtype u, UDWtype v)
{
  UDWtype w;

  (void) __udivmoddi4 (u, v, &w, libgccUmoddiBitLengths);

  return w;
}
