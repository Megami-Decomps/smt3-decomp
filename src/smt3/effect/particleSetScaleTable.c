#include "common.h"

extern void dds3Particle_SpiralSetScale(void *, f32);
extern void dds3Particle_SmokeSetScale(void *, f32);
extern void dds3Particle_SparkSetScale(void *, f32);
extern void dds3Particle_RadiateSetScale(void *, f32);
extern void dds3Particle_SphereSpiralSetScale(void *, f32);
extern void dds3Particle_SphereSmokeSetScale(void *, f32);
extern void dds3Particle_SphereRingSetScale(void *, f32);
extern void dds3Particle_Spark2SetScale(void *, f32);
extern void dds3Particle_SphereAtomSetScale(void *, f32);
extern void effScaleSingleParticleTemplate(void *, f32);
extern void dds3Particle_SparkOffsetSetScale(void *, f32);
extern void dds3Particle_SmokeOffsetSetScale(void *, f32);
extern void dds3Particle_SphereSpiral2SetScale(void *, f32);

void (*dds3ParticleSetScaleTable[13])(void *, f32) = {
    dds3Particle_SpiralSetScale,
    dds3Particle_SmokeSetScale,
    dds3Particle_SparkSetScale,
    dds3Particle_RadiateSetScale,
    dds3Particle_SphereSpiralSetScale,
    dds3Particle_SphereSmokeSetScale,
    dds3Particle_SphereRingSetScale,
    dds3Particle_Spark2SetScale,
    dds3Particle_SphereAtomSetScale,
    effScaleSingleParticleTemplate,
    dds3Particle_SparkOffsetSetScale,
    dds3Particle_SmokeOffsetSetScale,
    dds3Particle_SphereSpiral2SetScale,
};
