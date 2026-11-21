#ifndef PARTICLE_DISPATCH_H
#define PARTICLE_DISPATCH_H

typedef struct parDispatch_t {
    void *(*func)(void *);
    void (*update)(void *);
    void (*destroy)(void *);
} parDispatch_t;

extern parDispatch_t dds3ParticleFuncTable[13];

#endif
