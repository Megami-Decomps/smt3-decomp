/* scripts/vu2c.py 1.1, from build/vu/images.jsonl, options: --program FA33 --vu 0 --clean --flow --keep-nops.
 * 1 program, 177 instructions.  No timestamp: one input and one set of options
 * always give the same file. */
/* Pseudo-C for VU0 microcode, one statement per instruction.  Nothing here links or runs; the
 * declarations are what lets an editor parse it, and a swizzle is one float rather than the lanes
 * it names, which is all a parser needs.
 *
 *   vfNN four float lanes .x .y .z .w   viNN 16-bit integer, writes truncate   ACC accumulator
 *   I Q P R  immediate, divide, EFU, random      status mac clip  flag registers
 *   mem[a]  data memory, quadword a              mem_w[a].x  the same word as an integer
 *
 *   as_float(v)  a bit copy, not a conversion (MFIR, RGET).  A lane out of memory prints this way
 *                when it cannot be a number, so it is a packed field: a GIF tag among them
 *   lo16(f)      low 16 bits of the bit pattern (MTIR)      mr32(v)  (v.y, v.z, v.w, v.x)
 *   ftoiN/itofN  convert with N fraction bits; the result stays raw bits, so only the scale shows
 *   opmul(a, b)  (a.y*b.z, a.z*b.x, a.x*b.y), the halves an OPMULA/OPMSUB pair crosses
 *   cross(a, b)  the cross product those halves come to    clamp(v, lo, hi)  min(max(v, lo), hi)
 *   mul3x3(m, v) m[0]*v.x + m[1]*v.y + m[2]*v.z, m the registers from m[0] up; mul4x4 adds m[3]*v.w
 *   clip_bits(v, w)  x > w, x < -w, y > w, y < -w, z > w, z < -w, with w = |ft.w|
 *   xgkick(a)    start the GS transfer at mem[a]           goto_micro(a)  jump away for good
 *   subroutine(f, ...)  a routine entry: f, then what f reads before writing it, which is what
 *                a caller has to set.  A BAL to f prints as f().  The map above says where f ends.
 *
 * A statement reads all of its right side before writing any lane, so a register on both sides is
 * the value the instruction started with.  Both halves of a pair read the register file as it
 * stood before the pair, so where the lower half reads what the upper writes, the lower prints
 * first.  A branch takes after its delay slot, so the condition is captured at the branch and the
 * goto printed after the delay slot.
 */
#include <stdbool.h>

typedef union {
    struct { float x, y, z, w; };
    float xy, xz, xw, yz, yw, zw, xyz, xyw, xzw, yzw, xyzw;
} vec4;

typedef union {
    struct { int x, y, z, w; };
    int xy, xz, xw, yz, yw, zw, xyz, xyw, xzw, yzw, xyzw;
} ivec4;

vec4 vf00, vf01, vf02, vf03, vf04, vf05, vf06, vf07, vf08, vf09, vf10, vf11, vf12, vf13, vf14, vf15;
vec4 vf16, vf17, vf18, vf19, vf20, vf21, vf22, vf23, vf24, vf25, vf26, vf27, vf28, vf29, vf30, vf31;
vec4 ACC;
int vi00, vi01, vi02, vi03, vi04, vi05, vi06, vi07, vi08, vi09, vi10, vi11, vi12, vi13, vi14, vi15;
float I, Q, P;
unsigned R, status, mac, clip;
int VIF1_TOP, VIF1_ITOP;
union { vec4 f[1024]; ivec4 w[1024]; } vu_mem;

#define mem vu_mem.f
#define mem_w vu_mem.w

float max(float a, float b), min(float a, float b), fabsf(float v), sqrtf(float v);
float ftoi0(float v), ftoi4(float v), ftoi12(float v), ftoi15(float v);
float itof0(float v), itof4(float v), itof12(float v), itof15(float v);
float as_float(int raw), opmul(vec4 a, vec4 b);
vec4 mr32(vec4 v);
int lo16(float v);
unsigned bits(float v), clip_bits(float v, float w), lfsr_next(unsigned r);
float esin(float v), eatan(float v), eexp(float v), eatan2(float a, float b);
float vec2f(float x, float y), vec3f(float x, float y, float z);
float vec4f(float x, float y, float z, float w);
float clamp(float v, float lo, float hi), cross(vec4 a, vec4 b);
float mul3x3(vec4 m, float v), mul4x4(vec4 m, float v);
void xgkick(int addr), goto_micro(int addr);

/* This declares f and drops the rest, so `subroutine(f, vf13, vi06)` parses and f() below it
 * resolves; the registers are a macro argument nothing expands, read rather than resolved. */
#define subroutine(f, ...)  void f(void)

/* simplified, and nothing is folded that was not asked for:
 *   const   vf00 is (0, 0, 0, 1) and vi00 is 0; I comes from the nearest LOI and a viNN from the
 *           IADDIU that set it, dropped at every label and after every jump
 *   join    a constant crosses a label when every path into it carries the same value
 *   alias   a register loaded from a named location prints under that name until it is loaded
 *           from another, kept through a step and through an update from itself
 *   acc     a run of ACC writes ending in one vf write prints as one expression
 *   mat     an ACC chain over consecutive registers broadcasting x, y, z (, w) prints as
 *           mul3x3/mul4x4
 *   cross   OPMULA(a, b) then OPMSUB(b, a) prints as cross(a, b)
 *   ptr     LQI/SQI/LQD/SQD print as mem[viNN++] or mem[--viNN]
 *   clamp   an adjacent MAX/MINI pair against known constants prints as clamp(v, lo, hi)
 *   flow    a branch whose shape is unambiguous prints as do/while, if or break
 *   else    an if whose body ends in a branch past the next block prints as if/else
 *   call    a BAL prints as a call, the routine map above says what each entry reaches, and a JR
 *           on a register only ever written as a link prints as return
 *   loops   a comment over each loop names its counter and step
 *   branch  a branch condition is inlined where the delay slot cannot change it */

/* FA3335F3E7425E51: micro 0x0000..0x00B0, 177 instructions, uploaded to 0x0000 0 times */
/* reads before writing (linear scan): clip mem vf01 vf02 vf03 vf09 vf10 vf24 vf25 vf26 vf28 vf29 vf30 vf31 */
/* writes: ACC Q clip mem vf02 vf03 vf04 vf05 vf06 vf07 vf08 vf09 vf10 vf11 vf12 vf13 vf16 vf17 vf18 vf19 vf20 vf21 vf24 vf25 vf26 vf28 vf29 vf30 vf31 vi01 vi02 vi03 vi04 vi08 vi09 vi10 vi11 vi12 vi13 */
/* routines: entry, name, size, returns, callers, callees, other entries, what a call
 * writes, and whether one entry and a return are all there is to it. */
/*   0000 sub_0000               22, leaves 0014, writes unknown, not a function: it leaves without coming back */
/*   0042 sub_0042               17, leaves 0051, from 0014, writes unknown, not a function: it leaves without coming back */
/*   0057 sub_0057               27, leaves 006C, from 0051, writes unknown, not a function: it leaves without coming back */
/*   0072 sub_0072               81, leaves 006C, from 006C, +1 entries, writes unknown, not a function: the walk could not follow every exit */
/*   44 addresses no entry reaches: 0016 0017 0018 0019 001A 001B 001C 001D 001E 001F 0020 0021 0022 0023 0024 0025 ... */
/* loads from fixed addresses: mem[24] mem[97] mem[220] mem[221] mem[222] mem[223] mem[224] mem[225] mem[226] mem[227] mem[228] mem[229] mem[230] mem[231] mem[232] mem[255] */
void vu0_fa3335f3(void)
{
  bool take = false;
/* 0000 01C2195B 81E6133C MULw.xyz    */   vf05.xyz = vf03.xyz * vf02.w;
/*                        MOVE.xyzw   */   vf06.xyzw = vf02.xyzw;
/* 0001 01E2E1BC 8000033C             */   /* MULAx.xyzw folded into 0004 */
/* 0002 01E2E8BD 8000033C             */   /* MADDAy.xyzw folded into 0004 */
/* 0003 01E2F0BE 8000033C             */   /* MADDAz.xyzw folded into 0004 */
/* 0004 01E0F90B 8000033C MADDw.xyzw  */   vf04.xyzw = vf28.xyzw * vf02.x + vf29.xyzw * vf02.y + vf30.xyzw * vf02.z + vf31.xyzw; // folds 0001..0004
/* 0005 01C5C1BC 8000033C             */   /* MULAx.xyz folded into 0007 */
/* 0006 01C5C8BD 8000033C             */   /* MADDAy.xyz folded into 0007 */
/* 0007 41C5D1CA 8000033C MADDz.xyz   */   vf07.xyz = mul3x3(vf24, vf05.xyz);  // folds 0005..0007
/* 0008 01C6211B 8000033C MULw.xyz    */   vf04.xyz = vf04.xyz * vf06.w;
/*                                    */   return;  /* [E] at 0007 */
/* 0009 01C2109B 8000033C MULw.xyz    */   vf02.xyz = vf02.xyz * vf02.w;
/* 000A 01C3C1BC 8000033C             */   /* MULAx.xyz folded into 000C */
/* 000B 01C3C8BD 8000033C             */   /* MADDAy.xyz folded into 000C */
/* 000C 01C3D0CA 8000033C MADDz.xyz   */   vf03.xyz = mul3x3(vf24, vf03.xyz);  // folds 000A..000C
/* 000D 01E029BF 8000033C             */   /* MULAw.xyzw folded into 0011 */
/* 000E 01E2E0BC 8000033C             */   /* MADDAx.xyzw folded into 0011 */
/* 000F 01E2E8BD 8000033C             */   /* MADDAy.xyzw folded into 0011 */
/* 0010 01E2F0BE 8000033C             */   /* MADDAz.xyzw folded into 0011 */
/* 0011 01E2F90B 8000033C MADDw.xyzw  */   vf04.xyzw = vf05.xyzw + vf28.xyzw * vf02.x + vf29.xyzw * vf02.y + vf30.xyzw * vf02.z + vf31.xyzw * vf02.w; // folds 000D..0011
/* 0012 41C031BF 8000033C MULAw.xyz   */   ACC.xyz = vf06.xyz;
/* 0013 01C218CB 8000033C MADDw.xyz   */   vf03.xyz = ACC.xyz + vf03.xyz * vf02.w;
/*                                    */   return;  /* [E] at 0012 */
/* 0015 000002FF 100A0000 IADDIU      */   vi10 = 0;                           // delay slot of 0014
/* 0014 000002FF 4000002D B           */   goto L_0042;                        // call sub_0042 through vi10, returns to 0000
/* 0016 000002FF 01EC0018 LQ.xyzw     */   vf12.xyzw = mem[24].xyzw;
/* 0017 000002FF 01F300E3 LQ.xyzw     */   vf19.xyzw = mem[227].xyzw;
/* 0018 000002FF 10080000 IADDIU      */   vi08 = 0;
/* 0019 000002FF 090300FF ILW.x       */   vi03 = mem_w[255].x;
/* 001A 01CC636A 100B0060 MUL.xyz     */   vf13.xyz = vf12.xyz * vf12.xyz;
/*                        IADDIU      */   vi11 = 96;
/* 001B 010D683D 81E2437C LQI.xyzw    */   vf02.xyzw = mem[vi08++].xyzw;       // ADDAy.x folded into 001C
/* 001C 010D0B4A 019000E6 MADDz.x     */   vf13.x = vf13.x + vf13.y + vf01.x * vf13.z; // folds 001B..001C
/*                        LQ.xy       */   vf16.xy = mem[230].xy;
/* 001D 00389C18 019100E7 MULx.w      */   vf16.w = vf19.w * vf24.x;
/*                        LQ.xy       */   vf17.xy = mem[231].xy;
/* 001E 00399C58 019200E8 MULx.w      */   vf17.w = vf19.w * vf25.x;
/*                        LQ.xy       */   vf18.xy = mem[232].xy;
/* 001F 003A9C98 806D03BC MULx.w      */   vf18.w = vf19.w * vf26.x;
/*                        DIV         */   Q = 1.0f / vf13.x;
/* 0020 01C060DC 800003BF MULq.xyz    */   vf03.xyz = vf12.xyz * Q;
/*                        WAITQ       */   /* WAITQ */
/* 0021 003301BD 01EC4018 MULAy.w     */   ACC.w = vf19.y;
/*                        LQ.xyzw     */   vf12.xyzw = mem[vi08 + 24].xyzw;
/* 0022 002380BC 01F400E4 MADDAx.w    */   ACC.w = ACC.w + vf16.w * vf03.x;
/*                        LQ.xyzw     */   vf20.xyzw = mem[228].xyzw;
/* 0023 002388BD 40000006 MADDAy.w    */   ACC.w = ACC.w + vf17.w * vf03.y;
/* 0024 0023914A 01F500E5 MADDz.w     */   vf05.w = ACC.w + vf18.w * vf03.z;   // delay slot of 0023
/*                        LQ.xyzw     */   vf21.xyzw = mem[229].xyzw;
/*                        B           */   goto L_002A;
L_0025:  /* from 003B */
/* 0025 01E9AA4B 01EC4018 MADDw.xyzw  */   vf09.xyzw = ACC.xyzw + vf21.xyzw * vf09.w;
/*                        LQ.xyzw     */   vf12.xyzw = mem[vi08 + 24].xyzw;
/* 0026 003301BD 38010000 FCGET       */   vi01 = clip & 0xfff;                // MULAy.w folded into 0029
/* 0027 002380BC 038B57FE SQ.xy       */   mem[vi11 - 2].xy = vf10.xy;         // MADDAx.w folded into 0029
/* 0028 002388BD 0A215FFE ISW.w       */   mem_w[vi11 - 2].w = vi01;           // MADDAy.w folded into 0029
/* 0029 0023914A 03EB4FFF MADDz.w     */   vf05.w = vf19.y + vf16.w * vf03.x + vf17.w * vf03.y + vf18.w * vf03.z; // folds 0026..0029
/*                        SQ.xyzw     */   mem[vi11 - 1].xyzw = vf09.xyzw;
L_002A:  /* from 0023 */
/* 002A 01CC636A 8000033C MUL.xyz     */   vf13.xyz = vf12.xyz * vf12.xyz;
/* 002B 01E2E1BC 8000033C MULAx.xyzw  */   ACC.xyzw = vf28.xyzw * vf02.x;
/* 002C 01E2E8BD 8000033C MADDAy.xyzw */   ACC.xyzw = ACC.xyzw + vf29.xyzw * vf02.y;
/* 002D 01E2F0BE 100B5804 MADDAz.xyzw */   ACC.xyzw = ACC.xyzw + vf30.xyzw * vf02.z;
/*                        IADDIU      */   vi11 = vi11 + 4;
/* 002E 01E0F90B 81E2437C MADDw.xyzw  */   vf04.xyzw = ACC.xyzw + vf31.xyzw;
/*                        LQI.xyzw    */   vf02.xyzw = mem[vi08++].xyzw;
/* 002F 010D683D 8000033C             */   /* ADDAy.x folded into 0030 */
/* 0030 010D0B4A 8000033C MADDz.x     */   vf13.x = vf13.x + vf13.y + vf01.x * vf13.z; // folds 002F..0030
/* 0031 0025283F 01EA402E LQ.xyzw     */   vf10.xyzw = mem[vi08 + 46].xyzw;    // ADDAw.w folded into 0032
/* 0032 002000CF 8000033C MSUBw.w     */   vf03.w = vf05.w + vf05.w - 1.0f;    // folds 0031..0032
/* 0033 00202A50 806D03BC MAXx.w      */   vf09.w = max(vf05.w, 0.0f);
/*                        DIV         */   Q = 1.0f / vf13.x;
/* 0034 01E421FF 8000033C CLIP.xyzw   */   clip = (clip << 6) | clip_bits(vf04.xyz, vf04.w);
/* 0035 018A81BC 03EB27FC MULAx.xy    */   ACC.xy = vf16.xy * vf10.x;
/*                        SQ.xyzw     */   mem[vi11 - 4].xyzw = vf04.xyzw;
/* 0036 010302C3 8000033C ADDw.x      */   vf11.x = vf03.w;
/* 0037 00204A57 12031801 MINIw.w     */   vf09.w = min(vf09.w, 1.0f);
/*                        ISUBIU      */   vi03 = vi03 - 1;
/* 0038 018A88BD 03EB1FFD MADDAy.xy   */   ACC.xy = ACC.xy + vf17.xy * vf10.y;
/*                        SQ.xyzw     */   mem[vi11 - 3].xyzw = vf03.xyzw;
/* 0039 0180928B 03E85017 SQ.xyzw     */   mem[vi08 + 23].xyzw = vf10.xyzw;
/*                        MADDw.xy    */   vf10.xy = ACC.xy + vf18.xy;
/* 003A 01E059FF 8000033C CLIP.xyzw   */   clip = (clip << 6) | clip_bits(vf11.xyz, 1.0f);
/* 003B 01C060DC 5A001FE9 MULq.xyz    */   vf03.xyz = vf12.xyz * Q;
/* 003C 01E0A1BF 8000033C             */   /* MULAw.xyzw folded into 003D */
/*                        IBGTZ       */   if (vi03 > 0) goto L_0025;
/* 003D 01E9AA4B 038B57FE MADDw.xyzw  */   vf09.xyzw = vf20.xyzw + vf21.xyzw * vf09.w; // folds 003C..003D
/*                        SQ.xy       */   mem[vi11 - 2].xy = vf10.xy;
/* 003E 000002FF 38010000 FCGET       */   vi01 = clip & 0xfff;
/* 003F 000002FF 0A215FFE ISW.w       */   mem_w[vi11 - 2].w = vi01;
/* 0040 400002FF 03EB4FFF SQ.xyzw     */   mem[vi11 - 1].xyzw = vf09.xyzw;
/* 0041 000002FF 8000033C             */   /* nop */
/*                                    */   return;  /* [E] at 0040 */
subroutine(sub_0042, nothing);                                         /* 0042, from 0014 */
L_0042:  /* from 0014 */
/* 0042 000002FF 090400FF ILW.x       */   vi04 = mem_w[255].x;
/* 0043 000002FF 088300FF ILW.y       */   vi03 = mem_w[255].y;
/* 0044 000002FF 100C00C0 IADDIU      */   vi12 = 192;
/* 0045 000002FF 81FC637C LQI.xyzw    */   vf28.xyzw = mem[vi12++].xyzw;
/* 0046 000002FF 81FD637C LQI.xyzw    */   vf29.xyzw = mem[vi12++].xyzw;
/* 0047 000002FF 81FE637C LQI.xyzw    */   vf30.xyzw = mem[vi12++].xyzw;
/* 0048 000002FF 81FF637C LQI.xyzw    */   vf31.xyzw = mem[vi12++].xyzw;
/* 0049 000002FF 81E2537C LQI.xyzw    */   vf02.xyzw = mem[vi10++].xyzw;
/* 004A 000002FF 01E35017 LQ.xyzw     */   vf03.xyzw = mem[vi10 + 23].xyzw;
/* 004B 000002FF 81F8637C LQI.xyzw    */   vf24.xyzw = mem[vi12++].xyzw;
/* 004C 000002FF 81F9637C LQI.xyzw    */   vf25.xyzw = mem[vi12++].xyzw;
/* 004D 000002FF 81FA637C LQI.xyzw    */   vf26.xyzw = mem[vi12++].xyzw;
/* 004E 01C2195B 81E6133C MULw.xyz    */   vf05.xyz = vf03.xyz * vf02.w;
/*                        MOVE.xyzw   */   vf06.xyzw = vf02.xyzw;
/* 004F 01E2E1BC 80021FF2 MULAx.xyzw  */   ACC.xyzw = vf28.xyzw * vf02.x;
/*                        IADDI       */   vi02 = vi03 - 1;
/* 0050 000002FF 80001370 IADD        */   vi13 = vi02;
/* 0051 01E2E8BD 40000005 MADDAy.xyzw */   ACC.xyzw = ACC.xyzw + vf29.xyzw * vf02.y;
/* 0052 01E2F0BE 100B0060 MADDAz.xyzw */   ACC.xyzw = ACC.xyzw + vf30.xyzw * vf02.z; // delay slot of 0051
/*                        IADDIU      */   vi11 = 96;
/*                        B           */   goto L_0057;                        // call sub_0057 through vi11, returns to 0060
L_0053:  /* from 005A */
/* 0053 01C2195B 81E6133C MULw.xyz    */   vf05.xyz = vf03.xyz * vf02.w;
/*                        MOVE.xyzw   */   vf06.xyzw = vf02.xyzw;
/* 0054 01E2E1BC 8000033C MULAx.xyzw  */   ACC.xyzw = vf28.xyzw * vf02.x;
/* 0055 01E2E8BD 03EB3FFD MADDAy.xyzw */   ACC.xyzw = ACC.xyzw + vf29.xyzw * vf02.y;
/*                        SQ.xyzw     */   mem[vi11 - 3].xyzw = vf07.xyzw;
/* 0056 01E2F0BE 03EB27FC MADDAz.xyzw */   ACC.xyzw = ACC.xyzw + vf30.xyzw * vf02.z;
/*                        SQ.xyzw     */   mem[vi11 - 4].xyzw = vf04.xyzw;
subroutine(sub_0057, ACC, vf05, vf06, vf24, vf25, vf26, vf28, vf29, vf30, vf31, vi02, vi03, vi12); /* 0057, from 0051 */
L_0057:  /* from 0051 */
/* 0057 01E0F90B 81E2537C MADDw.xyzw  */   vf04.xyzw = ACC.xyzw + vf31.xyzw;
/*                        LQI.xyzw    */   vf02.xyzw = mem[vi10++].xyzw;
/* 0058 01C5C1BC 01E35017 MULAx.xyz   */   ACC.xyz = vf24.xyz * vf05.x;
/*                        LQ.xyzw     */   vf03.xyzw = mem[vi10 + 23].xyzw;
/* 0059 01C5C8BD 100B5804 MADDAy.xyz  */   ACC.xyz = ACC.xyz + vf25.xyz * vf05.y;
/*                        IADDIU      */   vi11 = vi11 + 4;
/* 005A 01C5D1CA 5A0017F8 MADDz.xyz   */   vf07.xyz = ACC.xyz + vf26.xyz * vf05.z;
/*                        IBGTZ       */   take = (vi02 > 0);
/* 005B 01C6211B 12021001 MULw.xyz    */   vf04.xyz = vf04.xyz * vf06.w;       // delay slot of 005A
/*                        ISUBIU      */   vi02 = vi02 - 1;
/*                        IBGTZ       */   if (take) goto L_0053;
/* 005C 000002FF 03EB3FFD SQ.xyzw     */   mem[vi11 - 3].xyzw = vf07.xyzw;
/* 005D 400002FF 03EB27FC SQ.xyzw     */   mem[vi11 - 4].xyzw = vf04.xyzw;
/* 005E 000002FF 10080030 IADDIU      */   vi08 = 48;
/*                                    */   return;  /* [E] at 005D */
/* 005F 000002FF 10090000 IADDIU      */   vi09 = 0;
L_0060:  /* from 007E */
/* 0060 000002FF 81FC637C LQI.xyzw    */   vf28.xyzw = mem[vi12++].xyzw;
/* 0061 000002FF 81FD637C LQI.xyzw    */   vf29.xyzw = mem[vi12++].xyzw;
/* 0062 000002FF 81FE637C LQI.xyzw    */   vf30.xyzw = mem[vi12++].xyzw;
/* 0063 000002FF 81FF637C LQI.xyzw    */   vf31.xyzw = mem[vi12++].xyzw;
/* 0064 000002FF 800042B0 IADD        */   vi10 = vi08;
/* 0065 000002FF 81E2537C LQI.xyzw    */   vf02.xyzw = mem[vi10++].xyzw;
/* 0066 000002FF 01E35017 LQ.xyzw     */   vf03.xyzw = mem[vi10 + 23].xyzw;
/* 0067 000002FF 81F8637C LQI.xyzw    */   vf24.xyzw = mem[vi12++].xyzw;
/* 0068 000002FF 81F9637C LQI.xyzw    */   vf25.xyzw = mem[vi12++].xyzw;
/* 0069 000002FF 81FA637C LQI.xyzw    */   vf26.xyzw = mem[vi12++].xyzw;
/* 006A 01C2121B 80021FF2 MULw.xyz    */   vf08.xyz = vf02.xyz * vf02.w;
/*                        IADDI       */   vi02 = vi03 - 1;
/* 006B 01C3C1BC 8028133C MULAx.xyz   */   ACC.xyz = vf24.xyz * vf03.x;
/*                        MOVE.w      */   vf08.w = vf02.w;
/* 006C 01C3C8BD 40000005 MADDAy.xyz  */   ACC.xyz = ACC.xyz + vf25.xyz * vf03.y;
/* 006D 01C3D14A 100B0060 MADDz.xyz   */   vf05.xyz = ACC.xyz + vf26.xyz * vf03.z; // delay slot of 006C
/*                        IADDIU      */   vi11 = 96;
/*                        B           */   goto L_0072;                        // call sub_0072 through vi11, returns to 0060
L_006E:  /* from 0077 */
/* 006E 01C2121B 8000033C MULw.xyz    */   vf08.xyz = vf02.xyz * vf02.w;
/* 006F 01C3C1BC 8028133C MOVE.w      */   vf08.w = vf02.w;                    // MULAx.xyz folded into 0071
/* 0070 01C3C8BD 03EB27FC SQ.xyzw     */   mem[vi11 - 4].xyzw = vf04.xyzw;     // MADDAy.xyz folded into 0071
/* 0071 01C3D14A 03EB4FFD MADDz.xyz   */   vf05.xyz = mul3x3(vf24, vf03.xyz);  // folds 006F..0071
/*                        SQ.xyzw     */   mem[vi11 - 3].xyzw = vf09.xyzw;
subroutine(sub_0072, vf01, vf05, vf08, vf24, vf25, vf26, vf28, vf29, vf30, vf31, vi02, vi03, vi04, vi08, vi09, vi12); /* 0072, from 006C */
L_0072:  /* from 006C */
/* 0072 01E8E1BC 01E65800 MULAx.xyzw  */   ACC.xyzw = vf28.xyzw * vf08.x;
/*                        LQ.xyzw     */   vf06.xyzw = mem[vi11].xyzw;
/* 0073 01E8E8BD 01E75801 MADDAy.xyzw */   ACC.xyzw = ACC.xyzw + vf29.xyzw * vf08.y;
/*                        LQ.xyzw     */   vf07.xyzw = mem[vi11 + 1].xyzw;
/* 0074 01E8F0BE 81E2537C MADDAz.xyzw */   ACC.xyzw = ACC.xyzw + vf30.xyzw * vf08.z;
/*                        LQI.xyzw    */   vf02.xyzw = mem[vi10++].xyzw;
/* 0075 01E8F8BF 01E35017 MADDAw.xyzw */   ACC.xyzw = ACC.xyzw + vf31.xyzw * vf08.w;
/*                        LQ.xyzw     */   vf03.xyzw = mem[vi10 + 23].xyzw;
/* 0076 01E0310B 100B5804 MADDw.xyzw  */   vf04.xyzw = ACC.xyzw + vf06.xyzw;
/*                        IADDIU      */   vi11 = vi11 + 4;
/* 0077 01C039BF 5A0017F6 MULAw.xyz   */   ACC.xyz = vf07.xyz;
/*                        IBGTZ       */   take = (vi02 > 0);
/* 0078 01C82A4B 12021001 MADDw.xyz   */   vf09.xyz = ACC.xyz + vf05.xyz * vf08.w; // delay slot of 0077
/*                        ISUBIU      */   vi02 = vi02 - 1;
/*                        IBGTZ       */   if (take) goto L_006E;
/* 0079 000002FF 03EB27FC SQ.xyzw     */   mem[vi11 - 4].xyzw = vf04.xyzw;
/* 007A 400002FF 03EB4FFD SQ.xyzw     */   mem[vi11 - 3].xyzw = vf09.xyzw;
/* 007B 000002FF 80004070 IADD        */   vi01 = vi08;
/*                                    */   return;  /* [E] at 007A */
/* 007C 000002FF 12042001 ISUBIU      */   vi04 = vi04 - 1;
/* 007D 000002FF 80004A30 IADD        */   vi08 = vi09;
/* 007F 000002FF 80000A70 IADD        */   vi09 = vi01;                        // delay slot of 007E
/* 007E 000002FF 520027E1 IBNE        */   if (vi04 != 0) goto L_0060;
/* 0080 000002FF 01EC0061 LQ.xyzw     */   vf12.xyzw = mem[97].xyzw;
/* 0081 000002FF 01F300E3 LQ.xyzw     */   vf19.xyzw = mem[227].xyzw;
/* 0082 01CC636A 100B0060 MUL.xyz     */   vf13.xyz = vf12.xyz * vf12.xyz;
/*                        IADDIU      */   vi11 = 96;
/* 0083 000002FF 01F800E0 LQ.xyzw     */   vf24.xyzw = mem[224].xyzw;
/* 0084 000002FF 01F900E1 LQ.xyzw     */   vf25.xyzw = mem[225].xyzw;
/* 0085 000002FF 01FA00E2 LQ.xyzw     */   vf26.xyzw = mem[226].xyzw;
/* 0086 010D683D 01E25800 LQ.xyzw     */   vf02.xyzw = mem[96].xyzw;           // ADDAy.x folded into 0087
/* 0087 010D0B4A 01FC00DC MADDz.x     */   vf13.x = vf13.x + vf13.y + vf01.x * vf13.z; // folds 0086..0087
/*                        LQ.xyzw     */   vf28.xyzw = mem[220].xyzw;
/* 0088 00389E18 01FD00DD MULx.w      */   vf24.w = vf19.w * vf24.x;
/*                        LQ.xyzw     */   vf29.xyzw = mem[221].xyzw;
/* 0089 00399E58 01FE00DE MULx.w      */   vf25.w = vf19.w * vf25.x;
/*                        LQ.xyzw     */   vf30.xyzw = mem[222].xyzw;
/* 008A 003A9E98 806D03BC MULx.w      */   vf26.w = vf19.w * vf26.x;
/*                        DIV         */   Q = 1.0f / vf13.x;
/* 008B 000002FF 01FF00DF LQ.xyzw     */   vf31.xyzw = mem[223].xyzw;
/* 008C 000002FF 01F000E6 LQ.xyzw     */   vf16.xyzw = mem[230].xyzw;
/* 008D 000002FF 01F100E7 LQ.xyzw     */   vf17.xyzw = mem[231].xyzw;
/* 008E 000002FF 01F200E8 LQ.xyzw     */   vf18.xyzw = mem[232].xyzw;
/* 008F 01C060DC 800003BF MULq.xyz    */   vf03.xyz = vf12.xyz * Q;
/*                        WAITQ       */   /* WAITQ */
/* 0090 003301BD 01EC5805 MULAy.w     */   ACC.w = vf19.y;
/*                        LQ.xyzw     */   vf12.xyzw = mem[96 + 5].xyzw;
/* 0091 0023C0BC 01F400E4 MADDAx.w    */   ACC.w = ACC.w + vf24.w * vf03.x;
/*                        LQ.xyzw     */   vf20.xyzw = mem[228].xyzw;
/* 0092 0023C8BD 40000006 MADDAy.w    */   ACC.w = ACC.w + vf25.w * vf03.y;
/* 0093 0023D14A 01F500E5 MADDz.w     */   vf05.w = ACC.w + vf26.w * vf03.z;   // delay slot of 0092
/*                        LQ.xyzw     */   vf21.xyzw = mem[229].xyzw;
/*                        B           */   goto L_0099;
L_0094:  /* from 00AA */
/* 0094 01E9AA4B 01EC5805 MADDw.xyzw  */   vf09.xyzw = ACC.xyzw + vf21.xyzw * vf09.w;
/*                        LQ.xyzw     */   vf12.xyzw = mem[vi11 + 5].xyzw;
/* 0095 003301BD 38010000 FCGET       */   vi01 = clip & 0xfff;                // MULAy.w folded into 0098
/* 0096 0023C0BC 038B57FE SQ.xy       */   mem[vi11 - 2].xy = vf10.xy;         // MADDAx.w folded into 0098
/* 0097 0023C8BD 0A215FFE ISW.w       */   mem_w[vi11 - 2].w = vi01;           // MADDAy.w folded into 0098
/* 0098 0023D14A 03EB4FFF MADDz.w     */   vf05.w = vf19.y + vf24.w * vf03.x + vf25.w * vf03.y + vf26.w * vf03.z; // folds 0095..0098
/*                        SQ.xyzw     */   mem[vi11 - 1].xyzw = vf09.xyzw;
L_0099:  /* from 0092 */
/* 0099 01CC636A 8000033C MUL.xyz     */   vf13.xyz = vf12.xyz * vf12.xyz;
/* 009A 01E2E1BC 8000033C MULAx.xyzw  */   ACC.xyzw = vf28.xyzw * vf02.x;
/* 009B 01E2E8BD 8000033C MADDAy.xyzw */   ACC.xyzw = ACC.xyzw + vf29.xyzw * vf02.y;
/* 009C 01E2F0BE 100B5804 MADDAz.xyzw */   ACC.xyzw = ACC.xyzw + vf30.xyzw * vf02.z;
/*                        IADDIU      */   vi11 = vi11 + 4;
/* 009D 01E0F90B 01E25800 MADDw.xyzw  */   vf04.xyzw = ACC.xyzw + vf31.xyzw;
/*                        LQ.xyzw     */   vf02.xyzw = mem[vi11].xyzw;
/* 009E 010D683D 8000033C             */   /* ADDAy.x folded into 009F */
/* 009F 010D0B4A 8000033C MADDz.x     */   vf13.x = vf13.x + vf13.y + vf01.x * vf13.z; // folds 009E..009F
/* 00A0 0025283F 81EA437C LQI.xyzw    */   vf10.xyzw = mem[vi08++].xyzw;       // ADDAw.w folded into 00A1
/* 00A1 002000CF 8000033C MSUBw.w     */   vf03.w = vf05.w + vf05.w - 1.0f;    // folds 00A0..00A1
/* 00A2 00202A50 806D03BC MAXx.w      */   vf09.w = max(vf05.w, 0.0f);
/*                        DIV         */   Q = 1.0f / vf13.x;
/* 00A3 01E421FF 8000033C CLIP.xyzw   */   clip = (clip << 6) | clip_bits(vf04.xyz, vf04.w);
/* 00A4 018A81BC 03EB27FC MULAx.xy    */   ACC.xy = vf16.xy * vf10.x;
/*                        SQ.xyzw     */   mem[vi11 - 4].xyzw = vf04.xyzw;
/* 00A5 010302C3 8000033C ADDw.x      */   vf11.x = vf03.w;
/* 00A6 00204A57 12031801 MINIw.w     */   vf09.w = min(vf09.w, 1.0f);
/*                        ISUBIU      */   vi03 = vi03 - 1;
/* 00A7 018A88BD 03EB1FFD MADDAy.xy   */   ACC.xy = ACC.xy + vf17.xy * vf10.y;
/*                        SQ.xyzw     */   mem[vi11 - 3].xyzw = vf03.xyzw;
/* 00A8 0180928B 03E85017 SQ.xyzw     */   mem[vi08 + 23].xyzw = vf10.xyzw;
/*                        MADDw.xy    */   vf10.xy = ACC.xy + vf18.xy;
/* 00A9 01E059FF 8000033C CLIP.xyzw   */   clip = (clip << 6) | clip_bits(vf11.xyz, 1.0f);
/* 00AA 01C060DC 5A001FE9 MULq.xyz    */   vf03.xyz = vf12.xyz * Q;
/* 00AB 01E0A1BF 8000033C             */   /* MULAw.xyzw folded into 00AC */
/*                        IBGTZ       */   if (vi03 > 0) goto L_0094;
/* 00AC 01E9AA4B 038B57FE MADDw.xyzw  */   vf09.xyzw = vf20.xyzw + vf21.xyzw * vf09.w; // folds 00AB..00AC
/*                        SQ.xy       */   mem[vi11 - 2].xy = vf10.xy;
/* 00AD 000002FF 38010000 FCGET       */   vi01 = clip & 0xfff;
/* 00AE 000002FF 0A215FFE ISW.w       */   mem_w[vi11 - 2].w = vi01;
/* 00AF 400002FF 03EB4FFF SQ.xyzw     */   mem[vi11 - 1].xyzw = vf09.xyzw;
/* 00B0 000002FF 8000033C             */   /* nop */
/*                                    */   return;  /* [E] at 00AF */
}
