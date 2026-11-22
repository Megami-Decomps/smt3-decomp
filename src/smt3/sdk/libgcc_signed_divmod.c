/* SPDX-License-Identifier: GPL-2.0-or-later
 *
 * Signed EE integer division, adapted from GCC libgcc2.c at revision
 * 380f6ad3dde6db991434068c4f3776c6762fa564. Keep the original GNU notice
 * and its historical linking permission below. The normalization core and
 * the distinct longlong.h notice are in libgcc_divmod_internal.h.
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

#include "sdk_libgcc_divmod_internal.h"

typedef s64 DWtype;
typedef int word_type __attribute__((mode(__word__)));

extern const u8 libgccDivdiBitLengths[256];
extern const u8 libgccModdiBitLengths[256];

/* Form the magnitude modulo 2^64 without signed word negation. The low
 * word is zero exactly when no high-word borrow is needed. Keep the actual
 * 0/1 borrow as its own value before the unsigned high-word subtraction.
 * The EE signed word members preserve the narrowed result bits.
 */
static inline DWtype
__negdi2_inline (DWtype u)
{
  DWunion w;
  DWunion uu;
  UWtype negatedHigh, borrow;

  uu.ll = u;

  w.s.low = 0U - (UWtype) uu.s.low;
  negatedHigh = 0U - (UWtype) uu.s.high;
  borrow = (UWtype) w.s.low > 0;
  w.s.high = negatedHigh - borrow;

  return w.ll;
}

DWtype
__divdi3 (DWtype u, DWtype v)
{
  word_type c = 0;
  DWunion uu, vv;
  DWtype w;

  uu.ll = u;
  vv.ll = v;

  if (uu.s.high < 0)
    c = ~c,
    uu.ll = __negdi2_inline (uu.ll);
  if (vv.s.high < 0)
    c = ~c,
    vv.ll = __negdi2_inline (vv.ll);

  w = __udivmoddi4 (uu.ll, vv.ll, (UDWtype *) 0, libgccDivdiBitLengths);
  if (c)
    w = __negdi2_inline (w);

  return w;
}

DWtype
__moddi3 (DWtype u, DWtype v)
{
  word_type c = 0;
  DWunion uu, vv;
  UDWtype w;

  uu.ll = u;
  vv.ll = v;

  if (uu.s.high < 0)
    c = ~c,
    uu.ll = __negdi2_inline (uu.ll);
  if (vv.s.high < 0)
    vv.ll = __negdi2_inline (vv.ll);

  (void) __udivmoddi4 (uu.ll, vv.ll, &w, libgccModdiBitLengths);
  if (c)
    w = __negdi2_inline (w);

  return w;
}
