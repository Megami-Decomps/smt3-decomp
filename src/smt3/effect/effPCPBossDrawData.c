#include "common.h"

#include "effPCPBossDrawData.h"

/* The VU0 readers use quadword loads for these vectors. */
f32 D_0031FF70[4] __attribute__((aligned(16))) = { 0.0f, 1.0f, 0.0f, 0.0f };
f32 effPCPBossLookDown[4] __attribute__((aligned(16))) = { 0.0f, 1.0f, 0.0f, 0.0f };
f32 effPCPBossKAGUDrawParam[4] __attribute__((aligned(16))) = { 0.0f, -200.0f, -800.0f, 0.0f };
f32 effPCPBossKAGU2DrawParam[4] __attribute__((aligned(16))) = { 0.0f, -900.0f, -800.0f, 0.0f };

effPCPDANTE_UV_tag effPCPDANTE_Bingo_UV = { 1, 1, 90, 62 };
effPCPDANTE_UV_tag effPCPDANTE_AreYouReady_UV = { 93, 1, 162, 62 };
effPCPDANTE_UV_tag effPCPDANTE_ShowTime_UV = { 1, 1, 185, 62 };
