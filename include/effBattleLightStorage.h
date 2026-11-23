#ifndef EFF_BATTLE_LIGHT_STORAGE_H
#define EFF_BATTLE_LIGHT_STORAGE_H

#include "sdf_types.h"

typedef struct
{
    u8 type;
    u8 unknown01;
    u8 unknown02;
    u8 unknown03;
    u32 unknown04;
    u32 diffuse;
    u32 ambient;
    u32 color_anim;
    u32 unknown14;
    u32 unknown18;
    u32 unknown1C;
    u32 unknown20;
} effBTL_SkillLight_t;

typedef struct
{
    u32 unknown00;
    u32 unknown04;
    u32 unknown08;
    u32 unknown0C;
    u32 unknown10;
    u32 unknown14;
    u32 unknown18;
} effBTLModelColorParam_t;

/* One light the battle effects place, 32 of them indexed by the formation id. */
typedef struct
{
    effBTL_SkillLight_t param;
    u32 light_frame;
    effBTLModelColorParam_t model;
    u32 model_frame;
} effBattleLight_effBTLPointLight_tag;

/* The unit position and model id a light starts from. */
typedef struct
{
    sdfVector4_t position;
    u32 unknown10;
    u32 unknown14;
    u8 reserve18[0x8];
} effBattleLight_effBTLPointLight_Base_tag;

extern effBattleLight_effBTLPointLight_tag effBTLPointLight[32];

extern effBattleLight_effBTLPointLight_Base_tag effBTLPointLightBase[32];

SIZE_CHECK(effBattlePointLightSize, effBattleLight_effBTLPointLight_tag, 0x48);
SIZE_CHECK(effBattlePointLightBaseSize, effBattleLight_effBTLPointLight_Base_tag, 0x20);

#endif
