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

#include "common.h"
#include "ps2asm.h"

typedef u32 UWtype;
typedef u64 UDWtype;

typedef union {
    struct { s32 low, high; } s;
    s64 ll;
} DWunion;

extern const u8 libgccUdivdiBitLengths[256];
extern const u8 libgccUmoddiBitLengths[264];

/* Translate the GCC machine-mode definitions to this EE's fixed widths. */
#define W_TYPE_SIZE 32
#define __BITS4 (W_TYPE_SIZE / 4)

#define __ll_B ((UWtype) 1 << (W_TYPE_SIZE / 2))

#define __ll_lowpart(t) ((UWtype) (t) & (__ll_B - 1))

#define __ll_highpart(t) ((UWtype) (t) >> (W_TYPE_SIZE / 2))

#define sub_ddmmss(sh, sl, ah, al, bh, bl) \
  do {                                  \
    UWtype __x;                             \
    __x = (al) - (bl);                          \
    (sh) = (ah) - (bh) - (__x > (al));                  \
    (sl) = __x;                             \
  } while (0)

#define __udiv_qrnnd_c(q, r, n1, n0, d) \
  do {                                  \
    UWtype __d1, __d0, __q1, __q0;                  \
    UWtype __r1, __r0, __m;                     \
    __d1 = __ll_highpart (d);                       \
    __d0 = __ll_lowpart (d);                        \
                                    \
    __r1 = (n1) % __d1;                         \
    __q1 = (n1) / __d1;                         \
    __m = (UWtype) __q1 * __d0;                     \
    __r1 = __r1 * __ll_B | __ll_highpart (n0);              \
    if (__r1 < __m)                         \
      {                                 \
    __q1--, __r1 += (d);                        \
    if (__r1 >= (d)) /* i.e. we didn't get carry when adding to __r1 */\
      if (__r1 < __m)                       \
        __q1--, __r1 += (d);                    \
      }                                 \
    __r1 -= __m;                            \
                                    \
    __r0 = __r1 % __d1;                         \
    __q0 = __r1 / __d1;                         \
    __m = (UWtype) __q0 * __d0;                     \
    __r0 = __r0 * __ll_B | __ll_lowpart (n0);               \
    if (__r0 < __m)                         \
      {                                 \
    __q0--, __r0 += (d);                        \
    if (__r0 >= (d))                        \
      if (__r0 < __m)                       \
        __q0--, __r0 += (d);                    \
      }                                 \
    __r0 -= __m;                            \
                                    \
    (q) = (UWtype) __q1 * __ll_B | __q0;                \
    (r) = __r0;                             \
  } while (0)

#define count_leading_zeros(count, x) \
  do {                                  \
    UWtype __xr = (x);                          \
    UWtype __a;                             \
                                    \
    if (W_TYPE_SIZE <= 32)                      \
      {                                 \
    __a = __xr < ((UWtype)1<<2*__BITS4)             \
      ? (__xr < ((UWtype)1<<__BITS4) ? 0 : __BITS4)         \
      : (__xr < ((UWtype)1<<3*__BITS4) ?  2*__BITS4 : 3*__BITS4);   \
      }                                 \
    else                                \
      {                                 \
    for (__a = W_TYPE_SIZE - 8; __a > 0; __a -= 8)          \
      if (((__xr >> __a) & 0xff) != 0)              \
        break;                          \
      }                                 \
                                    \
    (count) = W_TYPE_SIZE - (bitLengths[__xr >> __a] + __a);        \
  } while (0)

#define udiv_qrnnd __udiv_qrnnd_c
#define umul_ppmm(hi, lo, lhs, rhs) PS2_MULTU_WORDS(hi, lo, lhs, rhs)

static inline UDWtype
__udivmoddi4 (UDWtype n, UDWtype d, UDWtype *rp, const u8 *bitLengths)
{
  DWunion ww;
  DWunion nn, dd;
  DWunion rr;
  UWtype d0, d1, n0, n1, n2;
  UWtype q0, q1;
  UWtype b, bm;

  nn.ll = n;
  dd.ll = d;

  d0 = dd.s.low;
  d1 = dd.s.high;
  n0 = nn.s.low;
  n1 = nn.s.high;

/* UDIV_NEEDS_NORMALIZATION */

  if (d1 == 0)
    {
      if (d0 > n1)
        {
          /* 0q = nn / 0D */

          count_leading_zeros (bm, d0);

          if (bm != 0)
            {
              /* Normalize, i.e. make the most significant bit of the
                 denominator set.  */

              d0 = d0 << bm;
              n1 = (n1 << bm) | (n0 >> (W_TYPE_SIZE - bm));
              n0 = n0 << bm;
            }

          udiv_qrnnd (q0, n0, n1, n0, d0);
          q1 = 0;

          /* Remainder in n0 >> bm.  */
        }
      else
        {
          /* qq = NN / 0d */

          if (d0 == 0)
            d0 = 1 / d0;  /* Pinned EE division expansion deliberately traps. */

          count_leading_zeros (bm, d0);

          if (bm == 0)
            {
              /* From (n1 >= d0) /\ (the most significant bit of d0 is set),
                 conclude (the most significant bit of n1 is set) /\ (the
                 leading quotient digit q1 = 1).

                 This special case is necessary, not an optimization.
                 (Shifts counts of W_TYPE_SIZE are undefined.)  */

              n1 -= d0;
              q1 = 1;
            }
          else
            {
              /* Normalize.  */

              b = W_TYPE_SIZE - bm;

              d0 = d0 << bm;
              n2 = n1 >> b;
              n1 = (n1 << bm) | (n0 >> b);
              n0 = n0 << bm;

              udiv_qrnnd (q1, n1, n2, n1, d0);
            }

          /* n1 != d0...  */

          udiv_qrnnd (q0, n0, n1, n0, d0);

          /* Remainder in n0 >> bm.  */
        }

      if (rp != 0)
        {
          rr.s.low = n0 >> bm;
          rr.s.high = 0;
          *rp = rr.ll;
        }
    }

  else
    {
      if (d1 > n1)
        {
          /* 00 = nn / DD */

          q0 = 0;
          q1 = 0;

          /* Remainder in n1n0.  */
          if (rp != 0)
            {
              rr.s.low = n0;
              rr.s.high = n1;
              *rp = rr.ll;
            }
        }
      else
        {
          /* 0q = NN / dd */

          count_leading_zeros (bm, d1);
          if (bm == 0)
            {
              /* From (n1 >= d1) /\ (the most significant bit of d1 is set),
                 conclude (the most significant bit of n1 is set) /\ (the
                 quotient digit q0 = 0 or 1).

                 This special case is necessary, not an optimization.  */

              /* The condition on the next line takes advantage of that
                 n1 >= d1 (true due to program flow).  */
              if (n1 > d1 || n0 >= d0)
                {
                  q0 = 1;
                  sub_ddmmss (n1, n0, n1, n0, d1, d0);
                }
              else
                q0 = 0;

              q1 = 0;

              if (rp != 0)
                {
                  rr.s.low = n0;
                  rr.s.high = n1;
                  *rp = rr.ll;
                }
            }
          else
            {
              UWtype m1, m0;
              /* Normalize.  */

              b = W_TYPE_SIZE - bm;

              d1 = (d1 << bm) | (d0 >> b);
              d0 = d0 << bm;
              n2 = n1 >> b;
              n1 = (n1 << bm) | (n0 >> b);
              n0 = n0 << bm;

              udiv_qrnnd (q0, n1, n2, n1, d1);
              umul_ppmm (m1, m0, q0, d0);

              if (m1 > n1 || (m1 == n1 && m0 > n0))
                {
                  q0--;
                  sub_ddmmss (m1, m0, m1, m0, d1, d0);
                }

              q1 = 0;

              /* Remainder in (n1n0 - m1m0) >> bm.  */
              if (rp != 0)
                {
                  sub_ddmmss (n1, n0, n1, n0, m1, m0);
                  rr.s.low = (n1 << b) | (n0 >> bm);
                  rr.s.high = n1 >> bm;
                  *rp = rr.ll;
                }
            }
        }
    }

  ww.s.low = q0;
  ww.s.high = q1;
  return ww.ll;
}

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
