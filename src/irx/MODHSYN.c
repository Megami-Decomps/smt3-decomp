#include "MODHSYN.h"

extern u8 hsynLibraryTable[];

/* MODHSYN.IRX, Sony's SPU2 synthesiser: it owns the voices and the envelopes,
 * and a .SMG's bank is loaded into it. See docs/irx-modules.md.
 *
 * The chunks still in assembly are the ones no C has been written for yet,
 * plus the import stubs and the import and export tables between them. */

/* FUNC modhsyn_e30 */
int modhsyn_e30()
{
    s32 state;
    s32 res;

    intrman_CpuSuspendIntr(&state);
    res = loadcore_ReleaseLibraryEntries(hsynLibraryTable);
    intrman_CpuResumeIntr(state);
    return res != 0;
}
/* ENDF modhsyn_e30 */
INCLUDE_ASM("MODHSYN", D_00000044);
INCLUDE_ASM("MODHSYN", func_00000050);
/* FUNC modhsyn_e7 */
int modhsyn_e7(s16 core, u8 *iopAddr, u32 spuAddr, u32 size)
{
    u32 end;
    int done;

    end = (size >> 6) << 6;
    if ((size & 0x3F) != 0) {
        end += 0x40;
    }
    hsynSdVoiceTransStatus(core, 1);
    done = hsynSdVoiceTrans(core, 0, iopAddr, spuAddr, size);
    hsynSdVoiceTransStatus(core, 1);
    return (done == end) ? 0 : -1;
}
/* ENDF modhsyn_e7 */
INCLUDE_ASM("MODHSYN", D_000002D4);
/* FUNC hsynLoadBank */
/* Not a loader: a 4+4 character chunk tag comparator, with both tags passed
 * as words and both strings read backwards. */
int hsynLoadBank(u32 tag0, u32 tag1, const char *magic, const char *name)
{
    char *p0;
    char *p1;
    u32 i;

    p0 = (char *)&tag0;
    p1 = (char *)&tag1;
    for (i = 0; i < 4; i++) {
        if (p0[i] != magic[3 - i] || p1[i] != name[3 - i]) {
            return 0;
        }
    }
    return 1;
}
/* ENDF hsynLoadBank */
INCLUDE_ASM("MODHSYN", modhsyn_sceHSyn_Load);
/* FUNC modhsyn_e28 */
int modhsyn_e28(sceHSynWork_t *work, int slotNo, u32 bankIdx)
{
    sceHSynTbl_t *tbl;
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynBank_t *bank;
    u32 unit;
    int i;
    int ofs;

    if (work == NULL || bankIdx >= 0x10) {
        return -1;
    }
    tbl = work->tbl;
    unit = slotNo * 2 + 1;
    if (unit >= tbl->nEntryWord) {
        return -1;
    }
    slot = (sceHSynSlot_t *)((void **)tbl->entry)[unit * 2 + 1];
    if (slot == NULL) {
        return -1;
    }
    set = (sceHSynChanSet_t *)&slot->unk14;
    if (slot->path == 1) {
        bankIdx = 0;
    }
    bank = &set->bank[bankIdx];
    bank->head = NULL;
    bank->prog = NULL;
    bank->sset = NULL;
    bank->smpl = NULL;
    bank->vagi = NULL;
    bank->setb = NULL;
    bank->spuAddr = 0;
    for (i = 0, ofs = 4; i < 16; i++, ofs += 0x38) {
        hsynResetState((sceHSynChannel_t *)((char *)set + ofs), slot);
    }
    return 0;
}
/* ENDF modhsyn_e28 */
INCLUDE_ASM("MODHSYN", D_00000714);
/* FUNC hsynResetState */
void hsynResetState(sceHSynChannel_t *chan, sceHSynSlot_t *slot)
{
    sceHSynVoice_t *v;

    v = (sceHSynVoice_t *)((char *)chan - 8);
    chan->onPrev = v;
    chan->onNext = v;
    v = (sceHSynVoice_t *)((char *)chan + 8);
    chan->offPrev = v;
    chan->offNext = v;
    chan->relPrev = (sceHSynVoice_t *)chan;
    chan->relNext = (sceHSynVoice_t *)chan;
    chan->prog = NULL;
    chan->slot = slot;
    chan->bank = NULL;
    chan->unk32 = 0;
    chan->unk26 = 0;
    chan->bendRange = 0;
    chan->unk28 = -1;
    chan->bankIdx = 0;
    chan->vol = 0x80;
    chan->pan = 0x80;
    chan->unk2D = 0;
    chan->unk2F = 0xFF;
    chan->unk30 = -1;
    chan->unk2E = 0;
    chan->unk31 = 0;
    chan->priority = 0xFF;
    chan->unk37 = 0xFF;
    chan->unk36 = 0xFF;
    chan->unk35 = 0xFF;
}
/* ENDF hsynResetState */
/* FUNC hsynVoiceParamReset */
void hsynVoiceParamReset(sceHSynChanSet_t *set, sceHSynChannel_t *chan, u8 prog)
{
    sceSmgProgEnt_t *ent;
    sceHSynBank_t *bank;
    sceSmgProg_t *p;

    ent = NULL;
    bank = NULL;
    if (chan->bankIdx < 16) {
        bank = &set->bank[chan->bankIdx];
        p = bank->prog;
        if (p != NULL && p->nEntry >= prog && p->ofs[prog] != -1) {
            ent = (sceSmgProgEnt_t *)((char *)p + p->ofs[prog]);
            if (ent->nKey == 0) {
                ent = NULL;
            }
        }
    }
    chan->prog = ent;
    if (ent != NULL) {
        chan->bank = bank;
    } else {
        chan->bank = NULL;
    }
}
/* ENDF hsynVoiceParamReset */
#ifdef NON_MATCHING
/* Two instructions swapped: gcc schedules the head pointer ahead of the
 * saved bend value. */
/* FUNC hsynVoiceSetPitch */
void hsynVoiceSetPitch(sceHSynChannel_t *chan, u8 hi, u8 lo)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    s16 bend;
    int i;
    sceHSynVoice_t **head;

    bend = (hi << 7) + lo - 0x2000;
    if (chan->unk26 == bend) {
        return;
    }
    chan->unk26 = bend;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            hsynSdSetParamPitch(v, bend);
        }
    }
}
/* ENDF hsynVoiceSetPitch */
#else
INCLUDE_ASM("MODHSYN", hsynVoiceSetPitch);
#endif
/* FUNC hsynVoiceEnvelopeApply */
void hsynVoiceEnvelopeApply(sceHSynChannel_t *chan, int copyRelease)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;
    sceHSynVoice_t **head;

    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            if (copyRelease != 0) {
                v->unk54 = chan->pan;
            }
            hsynSdSetParamEnvelope(v);
        }
    }
}
/* ENDF hsynVoiceEnvelopeApply */
/* FUNC hsynVoiceSetAttack */
void hsynVoiceSetAttack(sceHSynChannel_t *chan, u8 val)
{
    u8 n;

    n = (val << 7) / 127;
    if (chan->vol != n) {
        chan->vol = n;
        hsynVoiceEnvelopeApply(chan, 0);
    }
}
/* ENDF hsynVoiceSetAttack */
/* FUNC hsynVoiceSetRelease */
void hsynVoiceSetRelease(sceHSynChannel_t *chan, u8 val)
{
    u8 n;

    n = (val << 7) / 127;
    if (chan->pan != n) {
        chan->pan = n;
        hsynVoiceEnvelopeApply(chan, 1);
    }
}
/* ENDF hsynVoiceSetRelease */
/* FUNC hsynVoiceSetVolumeLeft */
void hsynVoiceSetVolumeLeft(sceHSynChannel_t *chan, u8 val)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;
    sceHSynVoice_t **head;
    u16 n;

    n = (val << 7) / 127;
    if (chan->unk2D == n) {
        return;
    }
    chan->unk2D = n;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            hsynSdSetParamVolumeLeft(v, n);
        }
    }
}
/* ENDF hsynVoiceSetVolumeLeft */
/* FUNC hsynVoiceSetVolumeRight */
void hsynVoiceSetVolumeRight(sceHSynChannel_t *chan, u8 val)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;
    sceHSynVoice_t **head;
    u16 n;

    n = (val << 7) / 127;
    if (chan->unk2E == n) {
        return;
    }
    chan->unk2E = n;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            hsynSdSetParamVolumeRight(v, n);
        }
    }
}
/* ENDF hsynVoiceSetVolumeRight */
/* FUNC hsynVoiceFlagsGet */
int hsynVoiceFlagsGet(sceHSynChannel_t *chan, u8 semitones)
{
    chan->bendRange = semitones * 20;
}
/* ENDF hsynVoiceFlagsGet */
/* FUNC hsynVoiceFlagsSet */
void hsynVoiceFlagsSet(sceHSynChannel_t *chan, u8 on)
{
    if (on != 0) {
        if (chan->priority == 0xFF) {
            chan->priority = 0x80;
            chan->unk34 = 0x80;
        }
    } else {
        chan->priority = 0xFF;
    }
}
/* ENDF hsynVoiceFlagsSet */
/* FUNC hsynVoiceIndexGet */
void hsynVoiceIndexGet(sceHSynChannel_t *chan, u8 val)
{
    chan->unk34 = val;
}
/* ENDF hsynVoiceIndexGet */
#ifdef NON_MATCHING
/* Two instructions swapped, as in hsynVoiceSetPitch: gcc schedules the head
 * pointer ahead of the saved value the loop passes on. */
/* FUNC func_00000C58 */
void func_00000C58(sceHSynChannel_t *chan, u8 val)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;
    sceHSynVoice_t **head;
    s8 n;

    n = val - 0x40;
    if (n < -0x3F) {
        n = -0x3F;
    }
    if (chan->unk31 == n) {
        return;
    }
    chan->unk31 = n;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0, head = heads; i < 3; i++, head++) {
        end = *head;
        for (v = end->chNext; v != end; v = v->chNext) {
            func_00004AE4(v, n);
        }
    }
}
/* ENDF func_00000C58 */
#else
INCLUDE_ASM("MODHSYN", func_00000C58);
#endif
INCLUDE_ASM("MODHSYN", func_00000D24);
INCLUDE_ASM("MODHSYN", func_00000E40);
/* FUNC hsynVoiceSetPitchDirect */
void hsynVoiceSetPitchDirect(sceHSynChannel_t *chan, u8 ch, u32 tag, u8 hi, u8 lo)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    s16 bend;
    int i;

    bend = (hi << 7) + lo - 0x2000;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chNext; v != end; v = v->chNext) {
            if (tag != 0x7F && v->tag != tag) {
                continue;
            }
            if ((ch & 0x80) == 0 && v->ch != ch) {
                continue;
            }
            hsynSdSetParamPitch(v, bend);
        }
    }
}
/* ENDF hsynVoiceSetPitchDirect */
INCLUDE_ASM("MODHSYN", D_0000106C);
/* FUNC hsynVoiceApplyAll */
void hsynVoiceApplyAll(sceHSynChannel_t *chan)
{
    hsynVoiceAllocByPriority(chan, 0);
    hsynVoiceSetPitch(chan, 0x40, 0);
    hsynVoiceSetVolumeLeft(chan, 0);
    hsynVoiceSetVolumeRight(chan, 0);
    hsynVoiceSetAttack(chan, 0x7F);
    func_00000C58(chan, 0x40);
    hsynVoiceSetRelease(chan, 0x7F);
    hsynVoiceFlagsSet(chan, 0);
}
/* ENDF hsynVoiceApplyAll */
INCLUDE_ASM("MODHSYN", hsynVoiceEventDispatch);
INCLUDE_ASM("MODHSYN", hsynVoiceUpdatePass);
/* FUNC hsynVoiceRefreshHook */
void hsynVoiceRefreshHook(void)
{
}
/* ENDF hsynVoiceRefreshHook */
INCLUDE_ASM("MODHSYN", hsynVoiceDirectPass);
INCLUDE_ASM("MODHSYN", hsynVoiceRefresh);
#ifdef NON_MATCHING
/* Two instructions short: gcc bases the entry loads on entry+0xC
 * instead of keeping the +8 segment pointer live. */
/* FUNC hsynServiceTick */
void hsynServiceTick(sceHSynEntry_t *entry, int nEntryWord)
{
    sceHSynSeq_t *seq;
    sceHSynSlot_t *slot;
    u32 *p;
    u32 *seg;
    u8 *data;
    u8 *end;
    int i;
    int w;
    int n;

    i = 0;
    p = (u32 *)entry;
    for (w = 1; w < nEntryWord; w += 2) {
        seg = p + 2;
        seq = (sceHSynSeq_t *)p[1];
        slot = (sceHSynSlot_t *)seg[1];
        if (seq != NULL && slot != NULL) {
            data = seq->data;
            n = seq->len + 8;
            end = (u8 *)seq + n;
            if (data < end) {
                seq->len = 0;
                switch (slot->flags & 1) {
                case 0:
                    hsynVoiceRefresh((sceHSynSlot_t *)seg[1], i, data, end);
                    break;
                case 1:
                    func_0000273C((sceHSynSlot_t *)seg[1], i, data, end);
                    break;
                }
            }
        }
        p += 4;
        i++;
    }
}
/* ENDF hsynServiceTick */
#else
INCLUDE_ASM("MODHSYN", hsynServiceTick);
#endif
INCLUDE_ASM("MODHSYN", func_000017EC);
INCLUDE_ASM("MODHSYN", modhsyn_e8);
/* FUNC modhsyn_e26 */
int modhsyn_e26(u32 *mask)
{
    mask[0] = hsynVoiceMask[0];
    mask[1] = hsynVoiceMask[1];
    return 0;
}
/* ENDF modhsyn_e26 */
#ifdef NON_MATCHING
/* Three registers apart: retail keeps the unit index in the dead slotNo
 * register, gcc reuses the one holding slotNo * 2. */
/* FUNC hsynParamSlotResolve */
int hsynParamSlotResolve(sceHSynWork_t *work, int slotNo, sceHSynSlot_t **pslot, sceHSynChanSet_t **pset)
{
    sceHSynTbl_t *tbl;
    sceHSynSlot_t *slot;
    u32 unit;

    if (work != NULL) {
        tbl = work->tbl;
        /* nEntryWord counts eight-byte units, so a slot owns units 2n and
         * 2n+1 and its sceHSynSlot_t pointer is the odd one's second word. */
        unit = slotNo * 2 + 1;
        if (unit < tbl->nEntryWord) {
            slot = (sceHSynSlot_t *)((void **)tbl->entry)[unit * 2 + 1];
            *pslot = slot;
            *pset = (sceHSynChanSet_t *)&slot->unk14;
            return 1;
        }
    }
    return 0;
}
/* ENDF hsynParamSlotResolve */
#else
INCLUDE_ASM("MODHSYN", hsynParamSlotResolve);
#endif
/* FUNC modhsyn_e10 */
int modhsyn_e10(sceHSynWork_t *work, int slotNo, s16 val)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    set->unk00 = val;
    return 0;
}
/* ENDF modhsyn_e10 */
/* FUNC modhsyn_e11 */
int modhsyn_e11(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return 0;
    }
    return set->unk00;
}
/* ENDF modhsyn_e11 */
/* FUNC modhsyn_e12 */
int modhsyn_e12(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    int i;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        hsynVoiceAllocByChannel(&set->chan[i]);
    }
    return 0;
}
/* ENDF modhsyn_e12 */
/* FUNC modhsyn_e13 */
int modhsyn_e13(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    int i;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        hsynVoiceStop(&set->chan[i]);
    }
    return 0;
}
/* ENDF modhsyn_e13 */
/* FUNC modhsyn_e14 */
int modhsyn_e14(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    int i;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    for (i = 0; i < 16; i++) {
        hsynVoiceApplyAll(&set->chan[i]);
    }
    return 0;
}
/* ENDF modhsyn_e14 */
/* FUNC modhsyn_e9 */
int modhsyn_e9(sceHSynCore_t *c)
{
    sceSdEffectAttr_t attr;
    int on;

    attr.core = c->core;
    attr.depth_L = c->depthL;
    attr.depth_R = c->depthR;
    attr.delay = c->delay;
    attr.feedback = c->feedback;
    attr.mode = hsynChannelMap[c->mode & 0xF];
    on = attr.mode != 0;
    if ((c->mode & 0x100) != 0) {
        attr.mode |= 0x100;
    }
    hsynSdSetEffectAttr(c->core, &attr);
    hsynSdSetCoreAttr((u16)c->core | 2, on);
    hsynSdSetParam((u16)c->core | 0xB80, c->volL);
    hsynSdSetParam((u16)c->core | 0xC80, c->volR);
    return 0;
}
/* ENDF modhsyn_e9 */
/* FUNC modhsyn_e15 */
int modhsyn_e15(sceHSynStat_t *stat)
{
    hsynStat = stat;
    return 0;
}
/* ENDF modhsyn_e15 */
/* FUNC hsynEffectAttrRead */
void hsynEffectAttrRead(u8 sel, u8 mode)
{
    sceSdEffectAttr_t attr;
    int core;
    int t;

    if (mode >= 10) {
        return;
    }
    if (sel != 0 && sel != 0x10) {
        return;
    }
    t = sel & 0x10;
    core = t != 0;
    hsynSdGetEffectAttr(core, &attr);
    attr.mode = mode | 0x100;
    hsynSdSetEffectAttr(core, &attr);
}
/* ENDF hsynEffectAttrRead */
/* FUNC hsynEffectAttrWrite */
void hsynEffectAttrWrite(u8 sel, u8 val)
{
    sceSdEffectAttr_t attr;
    int core;
    int v;
    int t;

    if ((sel & 0xE0) != 0) {
        return;
    }
    t = sel & 0x10;
    core = t != 0;
    hsynSdGetEffectAttr(core, &attr);
    switch (sel & 0xF) {
    case 0:
        v = (val & 0xFF) << 8;
        attr.depth_R = v;
        attr.depth_L = v;
        break;
    case 1:
        v = -((val & 0xFF) << 8);
        attr.depth_R = v;
        attr.depth_L = v;
        break;
    case 2:
        attr.delay = val & 0xFF;
        break;
    case 3:
        attr.feedback = val & 0xFF;
        break;
    }
    hsynSdSetEffectAttr(core, &attr);
}
/* ENDF hsynEffectAttrWrite */
/* FUNC modhsyn_e17 */
int modhsyn_e17(sceHSynWork_t *work, int slotNo, u8 *out)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    u8 flags;
    int on;
    int i;
    int ofs;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    for (i = 0, ofs = 4; i < 16; i++, ofs += 0x38) {
        chan = (sceHSynChannel_t *)((char *)set + ofs);
        on = (char *)chan->onNext != (char *)chan - 8;
        flags = on;
        if ((char *)chan->relNext != (char *)chan) {
            flags = on | 2;
        }

        if ((char *)chan->offNext != (char *)chan + 8) {
            flags |= 4;
        }
        out[i] = flags;
    }
    return 0;
}
/* ENDF modhsyn_e17 */
/* FUNC modhsyn_e18 */
int modhsyn_e18(int mode)
{
    switch (mode) {
    case 0:
        hsynEnabled = 1;
        break;
    case 1:
        hsynEnabled = 0;
        break;
    default:
        return -1;
    }
    return 0;
}
/* ENDF modhsyn_e18 */
/* FUNC modhsyn_e27 */
int modhsyn_e27(void)
{
    return (hsynEnabled ^ 1) != 0;
}
/* ENDF modhsyn_e27 */
/* FUNC hsynCmdReset */
int hsynCmdReset(u8 **pp)
{
    u32 acc;
    u8 b;

    acc = 0;
    do {
        b = *(*pp)++;
        acc = (acc << 7) + (b & 0x7F);
    } while (b & 0x80);
    return acc;
}
/* ENDF hsynCmdReset */
INCLUDE_ASM("MODHSYN", hsynCmdProgramChange);
INCLUDE_ASM("MODHSYN", func_000021B8);
INCLUDE_ASM("MODHSYN", hsynCmdChannelPressure);
INCLUDE_ASM("MODHSYN", hsynCmdNoteOff);
INCLUDE_ASM("MODHSYN", hsynCmdNoteOn);
INCLUDE_ASM("MODHSYN", hsynCmdVoiceAlloc);
INCLUDE_ASM("MODHSYN", func_0000273C);
/* FUNC modhsyn_e19 */
int modhsyn_e19(u8 val)
{
    if (val >= 0x31) {
        return -1;
    }
    hsynModDisable = val;
    return 0;
}
/* ENDF modhsyn_e19 */
/* FUNC modhsyn_e20 */
int modhsyn_e20(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if ((slot->flags & 1) == 0) {
        return -1;
    }
    hsynVoiceAllocExport20(&set->chan[0]);
    return 0;
}
/* ENDF modhsyn_e20 */
/* FUNC modhsyn_e21 */
int modhsyn_e21(sceHSynWork_t *work, int slotNo)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if ((slot->flags & 1) == 0) {
        return -1;
    }
    hsynVoiceReleaseExport21(&set->chan[0]);
    return 0;
}
/* ENDF modhsyn_e21 */
/* FUNC modhsyn_e22 */
int modhsyn_e22(sceHSynWork_t *work, int slotNo, u32 tag, u8 *out, s8 max)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int n;
    int i;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if ((slot->flags & 1) == 0) {
        return -1;
    }
    n = 0;
    chan = set->chan;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = v->chPrev) {
            if (v->tag == tag) {
                out[n] = v->index + v->core * 24;
                n++;
                if (n >= max) {
                    return n;
                }
            }
        }
    }
    return n;
}
/* ENDF modhsyn_e22 */
/* FUNC modhsyn_e25 */
int modhsyn_e25(sceHSynWork_t *work, int slotNo, u32 *out, int max)
{
    sceHSynSlot_t *slot;
    sceHSynChanSet_t *set;
    sceHSynChannel_t *chan;
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int n;
    int i;
    int j;

    if (hsynParamSlotResolve(work, slotNo, &slot, &set) == 0) {
        return -1;
    }
    if ((slot->flags & 1) == 0) {
        return -1;
    }
    n = 0;
    chan = set->chan;
    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = v->chPrev) {
            for (j = 0; j < n; j++) {
                if (out[j] == v->tag) {
                    break;
                }
            }
            if (j < n) {
                continue;
            }
            out[n] = v->tag;
            n++;
            if (n >= max) {
                return n;
            }
        }
    }
    return n;
}
/* ENDF modhsyn_e25 */
INCLUDE_ASM("MODHSYN", modhsyn_e23);
INCLUDE_ASM("MODHSYN", modhsyn_e24);
INCLUDE_ASM("MODHSYN", hsynInitCores);
/* FUNC hsynCoreIndexGet */
void hsynCoreIndexGet(sceHSynVoice_t *v, sceHSynVoice_t *at)
{
    sceHSynVoice_t *p;

    v->prev = at;
    p = at->next;
    v->next = p;
    at->next = v;
    p->prev = v;
}
/* ENDF hsynCoreIndexGet */
/* FUNC hsynCoreIndexSet */
void hsynCoreIndexSet(sceHSynVoice_t *v, sceHSynVoice_t *at)
{
    sceHSynVoice_t *p;

    v->next = at;
    p = at->prev;
    v->prev = p;
    at->prev = v;
    p->next = v;
}
/* ENDF hsynCoreIndexSet */
/* FUNC hsynVoiceMaskGet */
void hsynVoiceMaskGet(sceHSynVoice_t *v)
{
    v->next->prev = v->prev;
    v->prev->next = v->next;
}
/* ENDF hsynVoiceMaskGet */
/* FUNC hsynVoiceSlotGet */
void hsynVoiceSlotGet(sceHSynVoice_t *v, sceHSynVoice_t **head)
{
    sceHSynVoice_t *n;

    /* head points at the chNext/chPrev pair of the list's sentinel. */
    v->chPrev = (sceHSynVoice_t *)((u8 *)head - 8);
    n = head[0];
    v->chNext = n;
    head[0] = v;
    n->chPrev = v;
}
/* ENDF hsynVoiceSlotGet */
INCLUDE_ASM("MODHSYN", D_00003734);
/* FUNC hsynVoiceSlotSet */
void hsynVoiceSlotSet(sceHSynVoice_t *v)
{
    v->chNext->chPrev = v->chPrev;
    v->chPrev->chNext = v->chNext;
}
/* ENDF hsynVoiceSlotSet */
INCLUDE_ASM("MODHSYN", hsynServiceVoices);
/* FUNC hsynEnvRateLookup */
s16 hsynEnvRateLookup(s16 delta, s32 base, s32 bias)
{
    s32 v;

    v = base + bias + delta;
    if (v > 0x7FFF) {
        v = 0x7FFF;
    } else if (v < -0x8000) {
        v = -0x8000;
    }
    return v;
}
/* ENDF hsynEnvRateLookup */
/* FUNC hsynEnvLevelLookup */
s8 hsynEnvLevelLookup(s8 delta, s32 base, s32 bias)
{
    s32 v;

    v = base + bias + delta;
    if (v >= 0x80) {
        v = 0x7F;
    } else if (v < -0x80) {
        v = -0x80;
    }
    return v;
}
/* ENDF hsynEnvLevelLookup */
#ifdef NON_MATCHING
/* Seven registers apart: the key delta and the mask land in the other
 * order. */
/* FUNC hsynAdjustFieldByKey */
u16 hsynAdjustFieldByKey(u16 field, s8 scale, int keyDelta, u16 mask, int shift)
{
    int delta;
    int v;

    delta = keyDelta * scale / 12;
    if (delta != 0) {
        v = ((field >> shift) & mask) + delta;
        if (v < 0) {
            v = 0;
        } else if (v > mask) {
            v = mask;
        }
        field = (field & ~(mask << shift)) | (v << shift);
    }
    return field;
}
/* ENDF hsynAdjustFieldByKey */
#else
INCLUDE_ASM("MODHSYN", hsynAdjustFieldByKey);
#endif
/* FUNC hsynBuildAdsr1 */
u16 hsynBuildAdsr1(int note, sceSmgSmplEnt_t *tone)
{
    u16 v;

    v = tone->adsr1;
    if (tone->arScale != 0) {
        v = hsynAdjustFieldByKey(v, tone->arScale, note - tone->arKey, 0x7F, 8);
    }
    if (tone->drScale != 0) {
        v = hsynAdjustFieldByKey(v, tone->drScale, note - tone->drKey, 0xF, 4);
    }
    if (tone->slScale != 0) {
        v = hsynAdjustFieldByKey(v, tone->slScale, note - tone->slKey, 0xF, 0);
    }
    return v;
}
/* ENDF hsynBuildAdsr1 */
/* FUNC hsynBuildAdsr2 */
u16 hsynBuildAdsr2(int note, sceSmgSmplEnt_t *tone)
{
    u16 v;

    v = tone->adsr2;
    if (tone->srScale != 0) {
        v = hsynAdjustFieldByKey(v, tone->srScale, note - tone->srKey, 0x7F, 6);
    }
    if (tone->rrScale != 0) {
        v = hsynAdjustFieldByKey(v, tone->rrScale, note - tone->rrKey, 0x1F, 0);
    }
    return v;
}
/* ENDF hsynBuildAdsr2 */
INCLUDE_ASM("MODHSYN", hsynVoiceUpdatePathA);
INCLUDE_ASM("MODHSYN", hsynVoiceKeyOnPathA);
INCLUDE_ASM("MODHSYN", hsynVoicePitchCommit);
/* FUNC hsynSdSetParamPitch */
void hsynSdSetParamPitch(sceHSynVoice_t *v, s16 bend)
{
    sceSmgProgKey_t *key;
    u16 depth;

    key = v->key;
    if (key == NULL) {
        return;
    }
    if (bend >= 0) {
        depth = key->unk08;
    } else {
        depth = key->unk06;
    }
    v->unk34 = depth * bend / 8192;
}
/* ENDF hsynSdSetParamPitch */
INCLUDE_ASM("MODHSYN", func_00004614);
/* FUNC hsynSdSetParamEnvelope */
void hsynSdSetParamEnvelope(sceHSynVoice_t *voice)
{
    u32 v;

    v = voice->chan->vol * voice->unk54;
    v = v * voice->unk16;
    voice->unk18 = v >> 14;
}
/* ENDF hsynSdSetParamEnvelope */
/* FUNC hsynSdSetParamVolume */
s32 hsynSdSetParamVolume(sceHSynVoice_t *v, s8 scale, u8 a, u8 b)
{
    sceHSynSlot_t *slot;

    slot = v->chan->slot;
    return hsynVolumeCurveLookup(v->vel, a, b, slot->unk10, slot->unk0C) * scale
        / 65536;
}
/* ENDF hsynSdSetParamVolume */
/* FUNC hsynSdSetParamVolumeLeft */
void hsynSdSetParamVolumeLeft(sceHSynVoice_t *v, u8 scale)
{
    int t;

    t = (scale * v->prog->unk22) / 128;
    t += v->unk51;
    if (t >= 0x80) {
        t = 0x7F;
    } else if (t < -0x80) {
        t = -0x80;
    }
    v->unk4F = t;
    t = (scale * v->prog->unk23) / 128;
    t += v->unk52;
    if (t >= 0x80) {
        t = 0x7F;
    } else if (t < -0x80) {
        t = -0x80;
    }
    v->unk50 = t;
}
/* ENDF hsynSdSetParamVolumeLeft */
/* FUNC hsynSdSetParamVolumeRight */
void hsynSdSetParamVolumeRight(sceHSynVoice_t *v, u8 scale)
{
    int t;

    t = (scale * v->prog->unk1C) / 128;
    t += v->unk24;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk20 = t;
    t = (scale * v->prog->unk1E) / 128;
    t += v->unk26;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk22 = t;
}
/* ENDF hsynSdSetParamVolumeRight */
#ifdef NON_MATCHING
/* Two instructions short: gcc enters the second wrap loop at its
 * bottom test where retail emits a guard with an unfilled delay slot. */
/* FUNC hsynSdSetParamAdsr */
void hsynSdSetParamAdsr(sceHSynVoice_t *v, s32 bias)
{
    int p;
    int k;
    int t;
    int lo;
    int hi;
    int x;

    p = v->prog->unk07;
    k = v->key->unk11;
    t = v->tone->unk0D;
    if (v->unk4E != 0) {
        lo = p - 0x80;
        hi = k + t;
        hi += bias;
        x = lo + hi;
        while (x >= 0x80) {
            x -= 0xFF;
        }
        while (x < -0x80) {
            x += 0xFF;
        }
        v->unk1E = x;
        return;
    }
    if (p < 0) {
        p = -p;
    }
    if (k < 0) {
        k = -k;
    }
    if (t < 0) {
        t = -t;
    }
    lo = p - 0x80;
    hi = k + t;
    hi += bias;
    x = lo + hi;
    if (x < 0) {
        x = 0;
    } else if (x >= 0x80) {
        x = 0x7F;
    }
    v->unk53 = 1;
    v->unk1E = x;
}
/* ENDF hsynSdSetParamAdsr */
#else
INCLUDE_ASM("MODHSYN", hsynSdSetParamAdsr);
#endif
INCLUDE_ASM("MODHSYN", func_00004AE4);
INCLUDE_ASM("MODHSYN", hsynSdSetParamBend);
INCLUDE_ASM("MODHSYN", func_00004D90);
INCLUDE_ASM("MODHSYN", hsynVoiceKeyOff);
/* FUNC hsynVoiceStateClear */
int hsynVoiceStateClear(sceHSynVoice_t *v)
{
    int x;
    u32 q;
    int t;

    x = (v->chan->priority - v->ch) << 7;
    v->unk38 = x;
    if (x < 0) {
        x = -x;
        v->flags |= 2;
    } else {
        v->flags |= 4;
    }
    q = (x * hsynTickScale) / v->chan->bendRange;
    t = (int)q / 256;
    v->unk2A = t;
    return t;
}
/* ENDF hsynVoiceStateClear */
INCLUDE_ASM("MODHSYN", hsynVoiceUpdatePathB);
INCLUDE_ASM("MODHSYN", hsynVoiceKeyOnPathB);
/* FUNC hsynEnvAttackStep */
void hsynEnvAttackStep(sceHSynVoice_t *v, u8 scale)
{
    sceSmgProgEnt_t *p;
    int t;

    p = v->prog;
    t = (scale * p->unk20) / 128;
    t += v->unk51;
    if (t >= 0x80) {
        t = 0x7F;
    } else if (t < -0x80) {
        t = -0x80;
    }
    v->unk4F = t;
    t = (scale * p->unk21) / 128;
    t += v->unk52;
    if (t >= 0x80) {
        t = 0x7F;
    } else if (t < -0x80) {
        t = -0x80;
    }
    v->unk50 = t;
}
/* ENDF hsynEnvAttackStep */
/* FUNC hsynEnvDecayStep */
void hsynEnvDecayStep(sceHSynVoice_t *v, u8 scale)
{
    sceSmgProgEnt_t *p;
    int t;

    p = v->prog;
    t = (scale * p->unk1C) / 128;
    t += v->unk24;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk20 = t;
    t = (scale * p->unk1E) / 128;
    t += v->unk26;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk22 = t;
}
/* ENDF hsynEnvDecayStep */
/* FUNC func_00005AA0 */
void func_00005AA0(sceHSynVoice_t *v, u16 scale)
{
    sceSmgProgEnt_t *p;
    int t;

    p = v->prog;
    t = (scale * p->unk1C) / 128;
    t += v->unk24;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk20 = t;
    t = (scale * p->unk1E) / 128;
    t += v->unk26;
    if (t > 0x7FFF) {
        t = 0x7FFF;
    } else if (t < -0x8000) {
        t = -0x8000;
    }
    v->unk22 = t;
}
/* ENDF func_00005AA0 */
#ifdef NON_MATCHING
/* Two instructions short: the same second-wrap-loop shape as
 * hsynSdSetParamAdsr. */
/* FUNC func_00005B4C */
/* The +4 byte of the program entry is read signed here and unsigned as a key
 * count in hsynVoiceParamReset, so the cast stands in for a field the struct
 * cannot hold both ways. */
void func_00005B4C(sceHSynVoice_t *v)
{
    int x;

    x = *(s8 *)&v->prog->nKey;
    if (v->unk4E != 0) {
        while (x >= 0x80) {
            x -= 0xFF;
        }
        while (x < -0x80) {
            x += 0xFF;
        }
        v->unk1E = x;
        return;
    }
    if (x < 0) {
        x = -x;
    }
    if (x < 0) {
        x = 0;
    } else if (x >= 0x80) {
        x = 0x7F;
    }
    v->unk53 = 1;
    v->unk1E = x;
}
/* ENDF func_00005B4C */
#else
INCLUDE_ASM("MODHSYN", func_00005B4C);
#endif
INCLUDE_ASM("MODHSYN", D_00005BDC);
/* FUNC hsynSpu2SettleDelay */
int hsynSpu2SettleDelay()
{
    u32 clk[2];
    u32 sec;
    int usec;
    int t;

    thbase_GetSystemTime(clk);
    thbase_SysClock2USec(clk, &sec, (u32 *)&usec);
    if (sec == hsynSettleTimeHi) {
        usec -= hsynSettleTimeLo;
    } else {
        sec--;
        if (sec == hsynSettleTimeHi) {
            return 1;
        }
        t = usec + 1000000;
        usec = t - hsynSettleTimeLo;
    }
    return usec >= 0x15;
}
/* ENDF hsynSpu2SettleDelay */
INCLUDE_ASM("MODHSYN", hsynVoiceAllocScan);
INCLUDE_ASM("MODHSYN", hsynVoiceMaskUpdate);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocByPriority);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocByChannel);
/* FUNC hsynVoiceStop */
void hsynVoiceStop(sceHSynChannel_t *chan)
{
    sceHSynVoice_t *heads[3];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)chan;
    heads[2] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 3; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = end->chPrev) {
            hsynKeyOffPending[v->core] |= 1 << v->index;
            hsynSdSetParam((v->core | (v->index << 1)) | 0x400, 0xC021);
            hsynVoiceRelease(v);
        }
    }
}
/* ENDF hsynVoiceStop */
INCLUDE_ASM("MODHSYN", hsynVoiceRelease);
INCLUDE_ASM("MODHSYN", hsynVoiceStealScan);
INCLUDE_ASM("MODHSYN", hsynVoiceEngineStep);
/* FUNC hsynVoiceMaskClear */
sceHSynVoice_t *hsynVoiceMaskClear(u8 allocHint)
{
    sceHSynVoice_t *v;
    int idx;
    int n;

    switch (allocHint & 0x30) {
    case 0x10:
        idx = 0;
        break;
    case 0x20:
        idx = 1;
        break;
    default:
        idx = hsynFreeCount[0] < hsynFreeCount[1];
        break;
    }
    n = hsynFreeCount[idx];
    if (n == 0) {
        return NULL;
    }
    hsynFreeCount[idx] = n - 1;
    v = hsynFreeList[idx][1];
    hsynVoiceMaskGet(v);
    return v;
}
/* ENDF hsynVoiceMaskClear */
INCLUDE_ASM("MODHSYN", hsynVoiceEnvelopeAdvance);
INCLUDE_ASM("MODHSYN", hsynVoiceVolumeAdvance);
INCLUDE_ASM("MODHSYN", hsynVoiceRetrigger);
/* FUNC hsynLfoApply */
sceHSynVoice_t *hsynLfoApply(sceHSynVoice_t *v, int keyOff)
{
    sceHSynVoice_t *next;

    next = v->chNext;
    if ((v->flags & 8) != 0) {
        hsynVoiceSlotSet(v);
        hsynVoiceMaskSet(v);
        return next;
    }
    if (keyOff == 1) {
        hsynKeyOffPending[v->core] |= keyOff << v->index;
    }
    hsynSdSetParam((v->core | (v->index << 1)) | 0x400, 0xC021);
    hsynVoiceReleaseOne(v);
    return next;
}
/* ENDF hsynLfoApply */
INCLUDE_ASM("MODHSYN", hsynVoiceAllocForCmd);
/* FUNC hsynVoiceMaskSet */
int hsynVoiceMaskSet(sceHSynVoice_t *v)
{
    return hsynVoiceMaskUpdate(v);
}
/* ENDF hsynVoiceMaskSet */
INCLUDE_ASM("MODHSYN", hsynVoiceAllocExport20);
INCLUDE_ASM("MODHSYN", hsynVoiceAllocPathB);
/* FUNC hsynVoiceAllocPathBEntry */
int hsynVoiceAllocPathBEntry(int a, int b)
{
    return hsynVoiceAllocPathB(a + 0x18, b);
}
/* ENDF hsynVoiceAllocPathBEntry */
INCLUDE_ASM("MODHSYN", hsynVoiceAllocPathC);
/* FUNC hsynVoiceAllocPathCEntry */
int hsynVoiceAllocPathCEntry(int a, int b, int c, int d)
{
    return hsynVoiceAllocPathC(a + 0x18, b, c, d);
}
/* ENDF hsynVoiceAllocPathCEntry */
/* FUNC hsynVoiceReleaseExport21 */
void hsynVoiceReleaseExport21(sceHSynChannel_t *chan)
{
    sceHSynVoice_t *heads[2];
    sceHSynVoice_t *end;
    sceHSynVoice_t *v;
    int i;

    heads[0] = (sceHSynVoice_t *)((char *)chan - 8);
    heads[1] = (sceHSynVoice_t *)((char *)chan + 8);
    for (i = 0; i < 2; i++) {
        end = heads[i];
        for (v = end->chPrev; v != end; v = end->chPrev) {
            hsynKeyOffPending[v->core] |= 1 << v->index;
            hsynSdSetParam((v->core | (v->index << 1)) | 0x400, 0xC021);
            hsynVoiceReleaseOne(v);
        }
    }
}
/* ENDF hsynVoiceReleaseExport21 */
/* FUNC hsynVoiceReleaseOne */
int hsynVoiceReleaseOne(sceHSynVoice_t *v)
{
    return hsynVoiceRelease(v);
}
/* ENDF hsynVoiceReleaseOne */
INCLUDE_ASM("MODHSYN", hsynVoiceReleaseGroup);
INCLUDE_ASM("MODHSYN", func_000078D4);
INCLUDE_ASM("MODHSYN", func_00007F54);
INCLUDE_ASM("MODHSYN", func_00008350);
/* FUNC hsynVoiceMaskClearOne */
sceHSynVoice_t *hsynVoiceMaskClearOne(u8 allocHint)
{
    return hsynVoiceMaskClear(allocHint);
}
/* ENDF hsynVoiceMaskClearOne */
INCLUDE_ASM("MODHSYN", hsynModulationApply);
/* FUNC hsynModulationStep */
int hsynModulationStep(u16 a, u8 b)
{
    u8 limit;
    int on;
    int done;

    on = 1;
    done = 0;
    limit = hsynModDisable;
    if (limit == 0) {
        on = 0;
    } else if (hsynRelCount >= limit) {
        done = 1;
    }
    return hsynModulationApply(a, b, on, done);
}
/* ENDF hsynModulationStep */
INCLUDE_ASM("MODHSYN", hsynPanApply);
INCLUDE_ASM("MODHSYN", hsynLfoStep);
INCLUDE_ASM("MODHSYN", D_00008914);
/* FUNC hsynUnrefParamGetA */
int hsynUnrefParamGetA(u8 a, u8 b)
{
    return ((a - b) << 16) / 126;
}
/* ENDF hsynUnrefParamGetA */
/* FUNC hsynUnrefParamGetB */
int hsynUnrefParamGetB(u8 a, u8 b)
{
    return ((b - a) << 16) / 126;
}
/* ENDF hsynUnrefParamGetB */
/* FUNC hsynUnrefParamSetA */
int hsynUnrefParamSetA(u8 a, u8 b)
{
    int x;
    int y;

    x = ((a - 1) << 15) / 126;
    x = x * x;
    y = ((b - 1) << 15) / 126;
    y = y * y;
    return (x - y) / 16384;
}
/* ENDF hsynUnrefParamSetA */
/* FUNC hsynUnrefParamSetB */
int hsynUnrefParamSetB(u8 a, u8 b)
{
    int x;
    int y;

    x = ((a - 1) << 15) / 126;
    x = 0x10000 - x * x / 16384;
    y = ((b - 1) << 15) / 126;
    y = 0x10000 - y * y / 16384;
    return x - y;
}
/* ENDF hsynUnrefParamSetB */
/* FUNC hsynUnrefParamSetC */
int hsynUnrefParamSetC(u8 a, u8 b)
{
    int x;
    int y;

    x = 0x10000 - ((a - 1) << 15) / 126;
    x = 0x10000 - x * x / 16384;
    y = 0x10000 - ((b - 1) << 15) / 126;
    y = 0x10000 - y * y / 16384;
    return x - y;
}
/* ENDF hsynUnrefParamSetC */
/* FUNC hsynUnrefParamSetD */
int hsynUnrefParamSetD(u8 a, u8 b)
{
    int x;
    int y;

    x = 0x10000 - ((a - 1) << 15) / 126;
    x = x * x / 16384;
    y = 0x10000 - ((b - 1) << 15) / 126;
    y = y * y / 16384;
    return x - y;
}
/* ENDF hsynUnrefParamSetD */
/* FUNC func_00008BB0 */
int func_00008BB0(u8 from, u8 to)
{
    int n;

    if (from == to) {
        return 0;
    }
    if (to < from) {
        n = 0x7F - to;
    } else {
        n = to - 1;
    }
    return ((from - to) << 16) / n;
}
/* ENDF func_00008BB0 */
/* FUNC func_00008C1C */
int func_00008C1C(u8 from, u8 to)
{
    int n;

    if (from == to) {
        return 0;
    }
    if (to < from) {
        n = 0x7F - to;
    } else {
        n = to - 1;
    }
    return ((to - from) << 16) / n;
}
/* ENDF func_00008C1C */
/* FUNC func_00008C88 */
int func_00008C88(u8 from, u8 to)
{
    int d;
    int q;

    if (from == to) {
        return 0;
    }
    d = (from - to) << 15;
    if (to < from) {
        q = d / (0x7F - to);
        return (q * q) / 16384;
    }
    q = d / (to - 1);
    return -((q * q) / 16384);
}
/* ENDF func_00008C88 */
#ifdef NON_MATCHING
/* Same size: the two `0x8000 - q` subtractions land in q's register
 * rather than the constant's. */
/* FUNC func_00008D54 */
int func_00008D54(u8 from, u8 to)
{
    int d;
    int t;
    int q;

    if (from == to) {
        return 0;
    }
    d = (from - to) << 15;
    if (to < from) {
        t = 0x8000 - d / (0x7F - to);
        return 0x10000 - (t * t) / 16384;
    }
    t = d / (to - 1) + 0x8000;
    return (t * t) / 16384 - 0x10000;
}
/* ENDF func_00008D54 */
#else
INCLUDE_ASM("MODHSYN", func_00008D54);
#endif
#ifdef NON_MATCHING
/* Same size and instructions: gcc coalesces `curve` into the dying `sel`
 * register where retail gives it one of its own. */
/* FUNC hsynVolumeCurveLookup */
int hsynVolumeCurveLookup(u8 a, u8 b, u8 sel, u8 *tbl, int nTbl)
{
    u32 curve;
    int idx;

    idx = sel >> 4;
    if (a == 0) {
        a = 1;
    }
    idx--;
    if (idx != -1 && idx < nTbl) {
        tbl += idx * 126;
        a = tbl[a - 1];
        b = tbl[b - 1];
    }
    curve = sel & 0xF;
    if (curve >= 11) {
        curve = 0;
    }
    return hsynCurveEval2[curve](a, b);
}
/* ENDF hsynVolumeCurveLookup */
#else
INCLUDE_ASM("MODHSYN", hsynVolumeCurveLookup);
#endif
/* FUNC hsynUnrefStubA */
u8 hsynUnrefStubA(s32 v)
{
    return v;
}
/* ENDF hsynUnrefStubA */
/* FUNC hsynUnrefStubB */
u8 hsynUnrefStubB(u8 v)
{
    return 0x80 - v;
}
/* ENDF hsynUnrefStubB */
/* FUNC hsynTableEntryFill */
int hsynTableEntryFill(u8 x)
{
    u32 v;
    int r;

    r = 1;
    v = x;
    v = v * v / 127;
    if (v != 0) {
        r = v & 0xFF;
    }
    return r;
}
/* ENDF hsynTableEntryFill */
/* FUNC hsynUnrefTableSetA */
u8 hsynUnrefTableSetA(u8 x)
{
    u32 r;

    r = hsynTableEntryFill(x);
    return 0x80 - r;
}
/* ENDF hsynUnrefTableSetA */
/* FUNC hsynUnrefTableSetB */
u8 hsynUnrefTableSetB(u8 x)
{
    int c;

    c = 0x80;
    return c - hsynTableEntryFill(c - x);
}
/* ENDF hsynUnrefTableSetB */
/* FUNC hsynUnrefTableSetC */
u8 hsynUnrefTableSetC(u8 x)
{
    return hsynTableEntryFill(0x80 - x);
}
/* ENDF hsynUnrefTableSetC */
#ifdef NON_MATCHING
/* Five registers apart: gcc keeps the curve index in the sel register where
 * retail uses a fresh one. */
/* FUNC hsynVolumeScale */
u8 hsynVolumeScale(u8 val, u8 sel, u8 *tbl, int n)
{
    int idx;
    u32 curve;

    idx = sel >> 4;
    if (val == 0) {
        val = 1;
    }
    /* Separate from the shift: folded into one expression gcc rewrites the
     * -1 test as a test of sel >> 4 against zero. */
    idx--;
    if (idx != -1 && idx < n) {
        tbl += idx * 126;
        val = tbl[val - 1];
    }
    curve = sel & 0xF;
    if (curve >= 11) {
        curve = 0;
    }
    return hsynCurveEval1[curve](val);
}
/* ENDF hsynVolumeScale */
#else
INCLUDE_ASM("MODHSYN", hsynVolumeScale);
#endif
/* FUNC hsynSeqByteRead */
int hsynSeqByteRead(sceHSynEnv_t *env, int wrapped)
{
    return (s16)((env->phase - 0x8000) >> 1);
}
/* ENDF hsynSeqByteRead */
/* FUNC hsynSeqWordRead */
int hsynSeqWordRead(sceHSynEnv_t *env, int wrapped)
{
    u32 t;

    t = 0xFFFF - env->phase;
    return (s16)((t - 0x8000) >> 1);
}
/* ENDF hsynSeqWordRead */
/* FUNC hsynSeqVarLenRead */
int hsynSeqVarLenRead(sceHSynEnv_t *env, int wrapped)
{
    int v;

    v = env->phase;
    if (v > 0xBFFF) {
        v -= 0x10000;
    } else if (v >= 0x4001) {
        v = 0x8000 - v;
    }
    return (s16)v;
}
/* ENDF hsynSeqVarLenRead */
/* FUNC hsynSeqSkip */
int hsynSeqSkip(sceHSynEnv_t *env, int wrapped)
{
    int v;

    v = -0x4000;
    if ((int)env->phase > 0x7FFF) {
        v = 0x4000;
    }
    return v;
}
/* ENDF hsynSeqSkip */
/* FUNC hsynSeqSeek */
int hsynSeqSeek(sceHSynEnv_t *env, int wrapped)
{
    u32 n;
    u32 prod;
    u32 i;
    s16 *t;
    int a;
    int b;

    n = env->nTbl;
    prod = env->phase * n;
    t = env->tbl;
    i = prod >> 16;
    a = t[i];
    i++;
    if (i >= n) {
        i = 0;
    }
    b = t[i];
    a += ((b - a) * (int)(prod & 0xFFFF)) >> 16;
    return (s16)(a / 2);
}
/* ENDF hsynSeqSeek */
/* FUNC hsynSeqEmit */
int hsynSeqEmit(sceHSynEnv_t *env, int wrapped)
{
    int v;

    if (wrapped != 0) {
        v = (s16)hsynSeqEmitTail() >> 2;
    } else {
        v = env->level;
    }
    return v;
}
/* ENDF hsynSeqEmit */
INCLUDE_ASM("MODHSYN", hsynEffectDispatch);
/* FUNC hsynSeqStateReset */
void hsynSeqStateReset(sceHSynEnv_t *lfo)
{
    if (lfo->unk02 != 0) {
        lfo->unk00 = 1;
        lfo->unk04 = lfo->unk02;
    } else if (lfo->unk0C != 0) {
        lfo->unk00 = 2;
        lfo->unk08 = 0;
    } else {
        lfo->unk00 = 3;
    }
}
/* ENDF hsynSeqStateReset */
INCLUDE_ASM("MODHSYN", hsynSeqInterpretA);
/* FUNC hsynSeqStart */
void hsynSeqStart(sceHSynEnv_t *env)
{
    if (env->unk01 == 0) {
        return;
    }
    switch (env->unk00) {
    case 0:
        hsynSeqStateReset(env);
        break;
    case 1:
        env->unk00 = 6;
        break;
    case 2:
    case 3:
        if (env->unk02 != 0) {
            env->unk00 = 4;
            env->unk04 = env->unk02;
        } else if (env->unk0C != 0) {
            env->unk00 = 5;
            env->unk08 = 0x10000;
        } else {
            env->unk00 = 6;
        }
        break;
    default:
        env->unk00 = 6;
        break;
    }
}
/* ENDF hsynSeqStart */
INCLUDE_ASM("MODHSYN", hsynSeqInterpretB);
INCLUDE_ASM("MODHSYN", D_00009AC4);
/* FUNC hsynSeqEmitTail */
int hsynSeqEmitTail()
{
    int seed;

    seed = hsynRandSeed * 0x41C64E6D + 0x3039;
    hsynRandSeed = seed;
    return seed >> 16;
}
/* ENDF hsynSeqEmitTail */
INCLUDE_ASM("MODHSYN", D_00009AFC);
#ifdef NON_MATCHING
/* One nop long: gas will not move the second half of the `sw w, sym` macro
 * into the branch delay slot, where the retail assembler put it. */
/* FUNC modhsyn_e16 */
int modhsyn_e16(sceHSynSdLog_t *w)
{
    hsynSdWorkArea = w;
    if (w != NULL) {
        w->index = 0;
        w->unk04 = 0;
    }
    return 0;
}
/* ENDF modhsyn_e16 */
#else
INCLUDE_ASM("MODHSYN", modhsyn_e16);
#endif
/* FUNC hsynSdLockAcquire */
/* The work area is a ring of 28-byte call records; +0 holds the opcode with
 * bit 31 set while the call is outstanding, +4 its result. */
sceHSynSdLogEnt_t *hsynSdLockAcquire()
{
    sceHSynSdLog_t *w;
    u32 i;
    sceHSynSdLogEnt_t *e;

    w = hsynSdWorkArea;
    if (w == NULL) {
        return NULL;
    }
    i = w->index;
    e = &w->entry[i];
    w->index = i + 1;
    if (i + 1 >= w->size) {
        w->index = 0;
    }
    return e;
}
/* ENDF hsynSdLockAcquire */
/* FUNC hsynSdLockRelease */
void hsynSdLockRelease(sceHSynSdLogEnt_t *e, int op, u32 a0, u32 a1, u32 a2, u32 a3, u32 a4)
{
    if (e != NULL) {
        e->op = op | 0x80000000;
        e->arg[0] = a0;
        e->arg[1] = a1;
        e->arg[2] = a2;
        e->arg[3] = a3;
        e->arg[4] = a4;
    }
}
/* ENDF hsynSdLockRelease */
/* FUNC hsynSdLockWait */
void hsynSdLockWait(sceHSynSdLogEnt_t *e, int result)
{
    if (e != NULL) {
        e->result = result;
        e->op &= 0x7FFFFFFF;
    }
}
/* ENDF hsynSdLockWait */
/* FUNC hsynSdInit */
int hsynSdInit(u32 flag)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 0, flag, 0, 0, 0, 0);
    ret = libsd_sceSdInit(flag);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdInit */
/* FUNC hsynSdSetParam */
int hsynSdSetParam(u16 entry, u16 value)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 1, entry, value, 0, 0, 0);
    libsd_sceSdSetParam(entry, value);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdSetParam */
/* FUNC hsynSdGetParam */
int hsynSdGetParam(u16 entry)
{
    sceHSynSdLogEnt_t *e;
    u16 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 2, entry, 0, 0, 0, 0);
    ret = libsd_sceSdGetParam(entry);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdGetParam */
/* FUNC hsynSdSetSwitch */
int hsynSdSetSwitch(u16 entry, u32 value)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 3, entry, value, 0, 0, 0);
    libsd_sceSdSetSwitch(entry, value);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdSetSwitch */
/* FUNC hsynSdGetSwitch */
u32 hsynSdGetSwitch(u16 entry)
{
    sceHSynSdLogEnt_t *e;
    u32 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 4, entry, 0, 0, 0, 0);
    ret = libsd_sceSdGetSwitch(entry);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdGetSwitch */
/* FUNC hsynSdSetAddr */
int hsynSdSetAddr(u16 entry, u32 value)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 5, entry, value, 0, 0, 0);
    libsd_sceSdSetAddr(entry, value);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdSetAddr */
/* FUNC hsynSdGetAddr */
u32 hsynSdGetAddr(u16 entry)
{
    sceHSynSdLogEnt_t *e;
    u32 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 6, entry, 0, 0, 0, 0);
    ret = libsd_sceSdGetAddr(entry);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdGetAddr */
/* FUNC hsynSdSetCoreAttr */
int hsynSdSetCoreAttr(u16 entry, u16 value)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 7, entry, value, 0, 0, 0);
    libsd_sceSdSetCoreAttr(entry, value);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdSetCoreAttr */
/* FUNC hsynSdNote2Pitch */
int hsynSdNote2Pitch(u16 center, u16 fine, u16 note, s16 bend)
{
    sceHSynSdLogEnt_t *e;
    u16 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 9, center, fine, note, bend, 0);
    ret = libsd_sceSdNote2Pitch(center, fine, note, bend);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdNote2Pitch */
/* FUNC hsynSdPitch2Note */
int hsynSdPitch2Note(u16 center, u16 fine, u16 pitch)
{
    sceHSynSdLogEnt_t *e;
    u16 ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 10, center, fine, pitch, 0, 0);
    ret = libsd_sceSdPitch2Note(center, fine, pitch);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdPitch2Note */
/* FUNC hsynSdProcBatch */
int hsynSdProcBatch(u32 *cmds, u32 *rets, u32 count)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 11, (u32)cmds, (u32)rets, count, 0, 0);
    ret = libsd_sceSdProcBatch(cmds, rets, count);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdProcBatch */
/* FUNC hsynSdProcBatchEx */
int hsynSdProcBatchEx(u32 *cmds, u32 *rets, u32 count, u32 flags)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 12, (u32)cmds, (u32)rets, count, flags, 0);
    ret = libsd_sceSdProcBatchEx(cmds, rets, count, flags);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdProcBatchEx */
/* FUNC hsynSdVoiceTrans */
int hsynSdVoiceTrans(s16 chan, u16 mode, u8 *iopAddr, u32 spuAddr, u32 size)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 13, chan, mode, (u32)iopAddr, spuAddr, size);
    ret = libsd_sceSdVoiceTrans(chan, mode, iopAddr, spuAddr, size);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdVoiceTrans */
/* FUNC hsynSdBlockTrans */
int hsynSdBlockTrans(s16 chan, u16 mode, u8 *iopAddr, u32 size)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 14, chan, mode, (u32)iopAddr, size, 0);
    ret = libsd_sceSdBlockTrans(chan, mode, iopAddr, size);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdBlockTrans */
/* FUNC hsynSdVoiceTransStatus */
int hsynSdVoiceTransStatus(s16 chan, s16 flag)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 15, chan, flag, 0, 0, 0);
    ret = libsd_sceSdVoiceTransStatus(chan, flag);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdVoiceTransStatus */
/* FUNC hsynSdBlockTransStatus */
int hsynSdBlockTransStatus(s16 chan, s16 flag)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 16, chan, flag, 0, 0, 0);
    ret = libsd_sceSdBlockTransStatus(chan, flag);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdBlockTransStatus */
/* FUNC hsynSdSetEffectAttr */
int hsynSdSetEffectAttr(int core, sceSdEffectAttr_t *attr)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 19, core, (u32)attr, 0, 0, 0);
    ret = libsd_sceSdSetEffectAttr(core, attr);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdSetEffectAttr */
/* FUNC hsynSdGetEffectAttr */
void hsynSdGetEffectAttr(int core, sceSdEffectAttr_t *attr)
{
    sceHSynSdLogEnt_t *e;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 20, core, (u32)attr, 0, 0, 0);
    libsd_sceSdGetEffectAttr(core, attr);
    hsynSdLockWait(e, 0);
}
/* ENDF hsynSdGetEffectAttr */
/* FUNC hsynSdClearEffectWorkArea */
int hsynSdClearEffectWorkArea(int core, int channel, int effectMode)
{
    sceHSynSdLogEnt_t *e;
    int ret;

    e = hsynSdLockAcquire();
    hsynSdLockRelease(e, 21, core, channel, effectMode, 0, 0);
    ret = libsd_sceSdClearEffectWorkArea(core, channel, effectMode);
    hsynSdLockWait(e, ret);
    return ret;
}
/* ENDF hsynSdClearEffectWorkArea */
INCLUDE_ASM("MODHSYN", D_0000A5E4);
#ifdef NON_MATCHING
/* One instruction short: retail copies the counter before incrementing it,
 * gcc folds the copy into the add. */
/* FUNC hsynEffectDepthSet */
int hsynEffectDepthSet(sceHSynLfo_t *lfo)
{
    u32 n;

    if (lfo->unk00 == 1) {
        n = lfo->unk10;
        if (n < lfo->unk0C) {
            lfo->unk10 = n + 1;
            lfo->unk02 = lfo->unk04 + (lfo->unk08 * n >> 8);
        } else {
            lfo->unk00 = 2;
            lfo->unk02 = lfo->unk04 + lfo->unk06;
            lfo->unk10 = lfo->unk10 + 1;
        }
    }
    return 0;
}
/* ENDF hsynEffectDepthSet */
#else
INCLUDE_ASM("MODHSYN", hsynEffectDepthSet);
#endif
/* FUNC hsynCoreStateGet */
int hsynCoreStateGet(sceHSynVoice_t *v)
{
    v->lfoA.unk00 = 0;
    v->lfoA.unk04 = 0;
    v->lfoA.unk02 = 0;
    v->lfoA.unk06 = 0;
    v->lfoA.unk08 = 0;
    v->lfoA.unk0C = 0;
    v->lfoA.unk10 = 0;
    return 0;
}
/* ENDF hsynCoreStateGet */
/* FUNC hsynPitchBendApply */
int hsynPitchBendApply(sceHSynVoice_t *v, int rate, u16 depth, int dir)
{
    int period;
    int step;
    int d;
    u16 e;

    period = (rate << 8) / hsynTickScale;
    d = (depth << 7) / 100;
    period--;
    e = d;
    if (period <= 0) {
        period = 1;
    }
    step = (e << 8) / period;
    if (dir == 0xF) {
        step = -step;
        e = -d;
    }
    v->lfoA.unk08 = step;
    v->lfoA.unk04 = v->lfoA.unk02;
    v->lfoA.unk10 = 1;
    v->lfoA.unk00 = 1;
    v->lfoA.unk06 = e;
    v->lfoA.unk0C = period;
    return 0;
}
/* ENDF hsynPitchBendApply */
INCLUDE_ASM("MODHSYN", D_0000A754);
#ifdef NON_MATCHING
/* One instruction short: retail copies the counter before incrementing it,
 * gcc folds the copy into the add. */
/* FUNC hsynEffectFeedbackSet */
int hsynEffectFeedbackSet(sceHSynVoice_t *voice)
{
    sceHSynLfo_t *lfo;
    u32 n;

    lfo = &voice->lfoB;
    if (lfo->unk00 == 1) {
        n = lfo->unk10;
        if (n < lfo->unk0C) {
            lfo->unk10 = n + 1;
            lfo->unk02 = lfo->unk04 + (lfo->unk08 * n >> 16);
        } else {
            lfo->unk02 = lfo->unk04 + lfo->unk06;
            lfo->unk00 = 2;
            lfo->unk10 = lfo->unk10 + 1;
        }
    }
    return 0;
}
/* ENDF hsynEffectFeedbackSet */
#else
INCLUDE_ASM("MODHSYN", hsynEffectFeedbackSet);
#endif
/* FUNC hsynCoreStateSet */
int hsynCoreStateSet(sceHSynVoice_t *v)
{
    v->lfoB.unk00 = 0;
    v->lfoB.unk04 = 0;
    v->lfoB.unk02 = 0;
    v->lfoB.unk06 = 0;
    v->lfoB.unk08 = 0;
    v->lfoB.unk0C = 0;
    v->lfoB.unk10 = 0;
    return 0;
}
/* ENDF hsynCoreStateSet */
#ifdef NON_MATCHING
/* One nop long: gcc hoists the lfoB phase load above the volume
 * product's mflo, where retail schedules it after the shift. */
/* FUNC func_0000A810 */
int func_0000A810(sceHSynVoice_t *v, int rate, u8 vol)
{
    sceHSynChannel_t *chan;
    sceHSynSlot_t *slot;
    int period;
    u32 prod;
    int level;
    int target;
    int step;

    period = (rate << 8) / hsynTickScale;
    chan = v->chan;
    slot = chan->slot;
    period--;
    if (period <= 0) {
        period = 1;
    }
    prod = vol * hsynVolumeScale(vol, v->unk4A, slot->unk10, slot->unk0C);
    prod = prod * (chan->vol * chan->pan);
    level = prod >> 14;
    level = (u16)level;
    target = v->unk18 + *(s16 *)&v->lfoB.unk02;
    target = level - target;
    step = (target << 16) / period;
    v->lfoB.unk0C = period;
    v->lfoB.unk04 = v->lfoB.unk02;
    v->lfoB.unk10 = 1;
    v->lfoB.unk00 = 1;
    v->lfoB.unk06 = target;
    v->lfoB.unk08 = step;
    return 0;
}
/* ENDF func_0000A810 */
#else
INCLUDE_ASM("MODHSYN", func_0000A810);
#endif
INCLUDE_ASM("MODHSYN", D_0000A92C);
/* FUNC func_0000A930 */
void func_0000A930(u32 *out, int wrap, s8 pan)
{
    int x;
    int en;

    en = hsynEnabled;
    out[1] = 0x80;
    out[0] = 0x80;
    if (en != 0) {
        return;
    }
    x = pan;
    if (wrap != 0) {
        if (x < -0x80) {
            x += 0xFF;
        } else if (x >= 0x80) {
            x -= 0xFF;
        }
        if (x < 0) {
            x = -x;
        }
    } else {
        if (x >= 0x80) {
            x = 0x7F;
        }
        if (x < -0x80) {
            x = -0x80;
        }
    }
    if (x == 0) {
        out[1] = 0;
        return;
    }
    if (x >= 0x7F) {
        out[0] = 0;
        return;
    }
    if (x < 0x40) {
        out[1] = (x << 1) & 0xFF;
        return;
    }
    if (x > 0x40) {
        out[0] = ((0x80 - x) << 1) & 0xFF;
    }
}
/* ENDF func_0000A930 */
INCLUDE_ASM("MODHSYN", func_0000A9FC);
/* FUNC hsynChannelStateGet */
int hsynChannelStateGet(sceHSynVoice_t *v)
{
    *(u32 *)&v->param[0] = 0;
    *(u32 *)&v->param[4] = -1;
    *(u32 *)&v->param[8] = 0;
    *(u32 *)&v->param[12] = 0;
    *(u32 *)&v->param[16] = 0;
    *(u32 *)&v->param[20] = 0;
    *(u32 *)&v->param[24] = 0;
    *(u32 *)&v->param[28] = 0;
    *(u32 *)&v->param[32] = 0;
    *(u32 *)&v->param[36] = 0;
    *(u32 *)&v->param[40] = 0;
    *(u32 *)&v->param[44] = 0x80;
    *(u32 *)&v->param[48] = 0x80;
    *(u32 *)&v->param[52] = 1;
    return 0;
}
/* ENDF hsynChannelStateGet */
INCLUDE_ASM("MODHSYN", func_0000AB90);
INCLUDE_ASM("MODHSYN", hsynLibraryTable);
/* FUNC hsynRetnStub */
void hsynRetnStub(void)
{
}
/* ENDF hsynRetnStub */
INCLUDE_ASM("MODHSYN", D_0000B094);
INCLUDE_ASM("MODHSYN", libsd_sceSdInit);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetParam);
INCLUDE_ASM("MODHSYN", libsd_sceSdGetParam);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetSwitch);
INCLUDE_ASM("MODHSYN", libsd_sceSdGetSwitch);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetAddr);
INCLUDE_ASM("MODHSYN", libsd_sceSdGetAddr);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetCoreAttr);
INCLUDE_ASM("MODHSYN", libsd_sceSdNote2Pitch);
INCLUDE_ASM("MODHSYN", libsd_sceSdPitch2Note);
INCLUDE_ASM("MODHSYN", libsd_sceSdProcBatch);
INCLUDE_ASM("MODHSYN", libsd_sceSdProcBatchEx);
INCLUDE_ASM("MODHSYN", libsd_sceSdVoiceTrans);
INCLUDE_ASM("MODHSYN", libsd_sceSdBlockTrans);
INCLUDE_ASM("MODHSYN", libsd_sceSdVoiceTransStatus);
INCLUDE_ASM("MODHSYN", libsd_sceSdBlockTransStatus);
INCLUDE_ASM("MODHSYN", libsd_sceSdSetEffectAttr);
INCLUDE_ASM("MODHSYN", libsd_sceSdGetEffectAttr);
INCLUDE_ASM("MODHSYN", libsd_sceSdClearEffectWorkArea);
INCLUDE_ASM("MODHSYN", D_0000B14C);
INCLUDE_ASM("MODHSYN", intrman_CpuSuspendIntr);
INCLUDE_ASM("MODHSYN", intrman_CpuResumeIntr);
INCLUDE_ASM("MODHSYN", D_0000B178);
INCLUDE_ASM("MODHSYN", loadcore_ReleaseLibraryEntries);
INCLUDE_ASM("MODHSYN", D_0000B19C);
INCLUDE_ASM("MODHSYN", thbase_DelayThread);
INCLUDE_ASM("MODHSYN", D_0000B1C0);
INCLUDE_ASM("MODHSYN", thbase_GetSystemTime);
INCLUDE_ASM("MODHSYN", thbase_SysClock2USec);
INCLUDE_ASM("MODHSYN", D_0000B1EC);
