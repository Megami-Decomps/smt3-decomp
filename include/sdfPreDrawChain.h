#ifndef SDF_PRE_DRAW_CHAIN_H
#define SDF_PRE_DRAW_CHAIN_H

struct sdfPreDrawTag_t;

/* Frame submission links its pre-draw tags through the two endpoints. */
typedef struct sdfPreDrawChain_t
{
    struct sdfPreDrawTag_t *tagHead;
    struct sdfPreDrawTag_t *tail;
} sdfPreDrawChain_t;

#endif
