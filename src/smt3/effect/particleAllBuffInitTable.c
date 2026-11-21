#include "common.h"

extern void dds3Particle_SpiralAllBuffInit(void *);
extern void dds3Particle_SmokeAllBuffInit(void *);
extern void dds3Particle_SparkAllBuffInit(void *);
extern void dds3Particle_RadiateAllBuffInit(void *);
extern void dds3Particle_SphereSpiralAllBuffInit(void *);
extern void dds3Particle_SphereSmokeAllBuffInit(void *);
extern void dds3Particle_SphereRingAllBuffInit(void *);
extern void dds3Particle_Spark2AllBuffInit(void *);
extern void dds3Particle_SphereAtomAllBuffInit(void *);
extern void dds3Particle_SpiralRandomize(void *);
extern void dds3Particle_SparkOffsetAllBuffInit(void *);
extern void dds3Particle_SmokeOffsetAllBuffInit(void *);
extern void dds3Particle_SphereSpiral2AllBuffInit(void *);

void (*dds3ParticleAllBuffInitTable[13])(void *) = {
    dds3Particle_SpiralAllBuffInit,
    dds3Particle_SmokeAllBuffInit,
    dds3Particle_SparkAllBuffInit,
    dds3Particle_RadiateAllBuffInit,
    dds3Particle_SphereSpiralAllBuffInit,
    dds3Particle_SphereSmokeAllBuffInit,
    dds3Particle_SphereRingAllBuffInit,
    dds3Particle_Spark2AllBuffInit,
    dds3Particle_SphereAtomAllBuffInit,
    dds3Particle_SpiralRandomize,
    dds3Particle_SparkOffsetAllBuffInit,
    dds3Particle_SmokeOffsetAllBuffInit,
    dds3Particle_SphereSpiral2AllBuffInit,
};
