#include "common.h"
#include "scr.h"

extern s32 CodeFunc_PushI(scrProcessWork_s *pwork);
extern s32 CodeFunc_PushF(scrProcessWork_s *pwork);
extern s32 CodeFunc_PushIX(scrProcessWork_s *pwork);
extern s32 CodeFunc_PushFX(scrProcessWork_s *pwork);
extern s32 CodeFunc_PushReg(scrProcessWork_s *pwork);
extern s32 CodeFunc_PopIX(scrProcessWork_s *pwork);
extern s32 CodeFunc_PopFX(scrProcessWork_s *pwork);
extern s32 CodeFunc_Proc(scrProcessWork_s *pwork);
extern s32 CodeFunc_Comm(scrProcessWork_s *pwork);
extern s32 CodeFunc_End(scrProcessWork_s *pwork);
extern s32 CodeFunc_Jump(scrProcessWork_s *pwork);
extern s32 CodeFunc_Call(scrProcessWork_s *pwork);
extern s32 CodeFunc_Run(scrProcessWork_s *pwork);
extern s32 CodeFunc_Goto(scrProcessWork_s *pwork);
extern s32 CodeFunc_Add(scrProcessWork_s *pwork);
extern s32 CodeFunc_Sub(scrProcessWork_s *pwork);
extern s32 CodeFunc_Mul(scrProcessWork_s *pwork);
extern s32 CodeFunc_Div(scrProcessWork_s *pwork);
extern s32 CodeFunc_Minus(scrProcessWork_s *pwork);
extern s32 CodeFunc_Not(scrProcessWork_s *pwork);
extern s32 CodeFunc_Or(scrProcessWork_s *pwork);
extern s32 CodeFunc_And(scrProcessWork_s *pwork);
extern s32 CodeFunc_EQ(scrProcessWork_s *pwork);
extern s32 CodeFunc_NEQ(scrProcessWork_s *pwork);
extern s32 CodeFunc_S(scrProcessWork_s *pwork);
extern s32 CodeFunc_L(scrProcessWork_s *pwork);
extern s32 CodeFunc_SE(scrProcessWork_s *pwork);
extern s32 CodeFunc_LE(scrProcessWork_s *pwork);
extern s32 CodeFunc_IF(scrProcessWork_s *pwork);
extern s32 CodeFunc_PushIS(scrProcessWork_s *pwork);
extern s32 CodeFunc_PushLIX(scrProcessWork_s *pwork);
extern s32 CodeFunc_PushLFX(scrProcessWork_s *pwork);
extern s32 CodeFunc_PopLIX(scrProcessWork_s *pwork);
extern s32 CodeFunc_PopLFX(scrProcessWork_s *pwork);
extern s32 CodeFunc_PushSTR(scrProcessWork_s *pwork);

/* Native opcode slots; the following permutation bytes remain separate. */
s32 (*codefunc[36])(scrProcessWork_s *pwork) = {
    CodeFunc_PushI,
    CodeFunc_PushF,
    CodeFunc_PushIX,
    CodeFunc_PushFX,
    CodeFunc_PushReg,
    CodeFunc_PopIX,
    CodeFunc_PopFX,
    CodeFunc_Proc,
    CodeFunc_Comm,
    CodeFunc_End,
    CodeFunc_Jump,
    CodeFunc_Call,
    CodeFunc_Run,
    CodeFunc_Goto,
    CodeFunc_Add,
    CodeFunc_Sub,
    CodeFunc_Mul,
    CodeFunc_Div,
    CodeFunc_Minus,
    CodeFunc_Not,
    CodeFunc_Or,
    CodeFunc_And,
    CodeFunc_EQ,
    CodeFunc_NEQ,
    CodeFunc_S,
    CodeFunc_L,
    CodeFunc_SE,
    CodeFunc_LE,
    CodeFunc_IF,
    CodeFunc_PushIS,
    CodeFunc_PushLIX,
    CodeFunc_PushLFX,
    CodeFunc_PopLIX,
    CodeFunc_PopLFX,
    CodeFunc_PushSTR,
    NULL,
};
