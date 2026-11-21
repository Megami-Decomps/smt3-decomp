#include "particleDispatch.h"

extern void *dds3Particle_SpiralCreate(void *);
extern void dds3Particle_SpiralDraw(void *);
extern void dds3Particle_SpiralDelete(void *);
extern void *dds3Particle_SmokeCreate(void *);
extern void dds3Particle_SmokeDraw(void *);
extern void dds3Particle_SmokeDelete(void *);
extern void *dds3Particle_SparkCreate(void *);
extern void dds3Particle_SparkDraw(void *);
extern void dds3Particle_SparkDelete(void *);
extern void *dds3Particle_RadiateCreate(void *);
extern void dds3Particle_RadiateDraw(void *);
extern void dds3Particle_RadiateDelete(void *);
extern void *dds3Particle_SphereSpiralCreate(void *);
extern void dds3Particle_SphereSpiralDraw(void *);
extern void dds3Particle_SphereSpiralDelete(void *);
extern void *dds3Particle_SphereSmokeCreate(void *);
extern void dds3Particle_SphereSmokeDraw(void *);
extern void dds3Particle_SphereSmokeDelete(void *);
extern void *dds3Particle_SphereRingCreate(void *);
extern void dds3Particle_SphereRingDraw(void *);
extern void dds3Particle_SphereRingDelete(void *);
extern void *dds3Particle_Spark2Create(void *);
extern void dds3Particle_Spark2Draw(void *);
extern void dds3Particle_Spark2Delete(void *);
extern void *dds3Particle_SphereAtomCreate(void *);
extern void dds3Particle_SphereAtomDraw(void *);
extern void dds3Particle_SphereAtomDelete(void *);
extern void *effCloneSingleParticleTemplate(void *);
extern void dds3Particle_SpiralBlendColor(void *);
extern void effFreeSingleParticleTemplate(void *);
extern void *dds3Particle_SparkOffsetCreate(void *);
extern void dds3Particle_SparkOffsetDraw(void *);
extern void dds3Particle_SparkOffsetDelete(void *);
extern void *dds3Particle_SmokeOffsetCreate(void *);
extern void dds3Particle_SmokeOffsetDraw(void *);
extern void dds3Particle_SmokeOffsetDelete(void *);
extern void *dds3Particle_SphereSpiral2Create(void *);
extern void dds3Particle_SphereSpiral2Draw(void *);
extern void dds3Particle_SphereSpiral2Delete(void *);

parDispatch_t dds3ParticleFuncTable[13] = {
    { dds3Particle_SpiralCreate, dds3Particle_SpiralDraw, dds3Particle_SpiralDelete },
    { dds3Particle_SmokeCreate, dds3Particle_SmokeDraw, dds3Particle_SmokeDelete },
    { dds3Particle_SparkCreate, dds3Particle_SparkDraw, dds3Particle_SparkDelete },
    { dds3Particle_RadiateCreate, dds3Particle_RadiateDraw, dds3Particle_RadiateDelete },
    { dds3Particle_SphereSpiralCreate, dds3Particle_SphereSpiralDraw, dds3Particle_SphereSpiralDelete },
    { dds3Particle_SphereSmokeCreate, dds3Particle_SphereSmokeDraw, dds3Particle_SphereSmokeDelete },
    { dds3Particle_SphereRingCreate, dds3Particle_SphereRingDraw, dds3Particle_SphereRingDelete },
    { dds3Particle_Spark2Create, dds3Particle_Spark2Draw, dds3Particle_Spark2Delete },
    { dds3Particle_SphereAtomCreate, dds3Particle_SphereAtomDraw, dds3Particle_SphereAtomDelete },
    { effCloneSingleParticleTemplate, dds3Particle_SpiralBlendColor, effFreeSingleParticleTemplate },
    { dds3Particle_SparkOffsetCreate, dds3Particle_SparkOffsetDraw, dds3Particle_SparkOffsetDelete },
    { dds3Particle_SmokeOffsetCreate, dds3Particle_SmokeOffsetDraw, dds3Particle_SmokeOffsetDelete },
    { dds3Particle_SphereSpiral2Create, dds3Particle_SphereSpiral2Draw, dds3Particle_SphereSpiral2Delete },
};
