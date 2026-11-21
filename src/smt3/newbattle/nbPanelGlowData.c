#include "common.h"

/* Four scale values for each of the eleven press-turn animation ticks. */
f32 nbPanelGlowTable[11][4] = {
    { 0x1.000000p+0f, 0x1.266666p+0f, 0x1.07ae14p+0f, 0x1.1cccccp+0f },
    { 0x1.0f5c28p+0f, 0x1.133332p+0f, 0x1.170a3cp+0f, 0x1.099998p+0f },
    { 0x1.1eb850p+0f, 0x1.000000p+0f, 0x1.266666p+0f, 0x1.09fbe6p+0f },
    { 0x1.2e147ap+0f, 0x1.13f7cep+0f, 0x1.35c28ep+0f, 0x1.1df3b6p+0f },
    { 0x1.3d70a2p+0f, 0x1.27ef9cp+0f, 0x1.451eb8p+0f, 0x1.31eb84p+0f },
    { 0x1.4cccccp+0f, 0x1.333332p+0f, 0x1.474bc6p+0f, 0x1.533332p+0f },
    { 0x1.41cac0p+0f, 0x1.733332p+0f, 0x1.3b367ap+0f, 0x1.6b851ep+0f },
    { 0x1.34a232p+0f, 0x1.63d70ap+0f, 0x1.2e0decp+0f, 0x1.5c28f4p+0f },
    { 0x1.2779a6p+0f, 0x1.547ae0p+0f, 0x1.20e560p+0f, 0x1.4cccccp+0f },
    { 0x1.1a5118p+0f, 0x1.451eb8p+0f, 0x1.13bcd2p+0f, 0x1.3d70a2p+0f },
    { 0x1.0d288cp+0f, 0x1.35c28ep+0f, 0x1.069446p+0f, 0x1.2e147ap+0f },
};
