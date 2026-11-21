#include "effEventSmoke.h"

SIZE_CHECK(fclCombineSmokeTemplateSize, effEVECombParSmokeParam_t, 124);

/* Base billboard-emitter parameters; the fusion scene copies this record
 * before overriding the number and radius for the selected demon. */
effEVECombParSmokeParam_t cmbDataTable = {
    .position = { 0.0f, -0x1.2c00000000000p+7f, 0.0f, 0.0f },
    .number = 150,
    .duration = 150,
    .wait = 20,
    .motionDelaySpread = 15,
    .spawnRadius = 0x1.9000000000000p+4f,
    .color = 0x60808080,
    .drawMode = 1,
    .fade_in = 15,
    .fade_out = 15,
    .repeat = 0,
    .baseScale = 0x1.e000000000000p+3f,
    .scaleRandomness = 0x1.8000000000000p+1f,
    .scaleAmplitudeX = 0x1.4000000000000p+2f,
    .scaleAmplitudeY = 0x1.4000000000000p+2f,
    .scalePhaseStep = 0x1.c9870a0000000p-10f,
    .angularSpeed = 0.0f,
    .verticalSpeed = -0x1.0000000000000p+0f,
    .verticalSpeedRandomness = 0.0f,
    .lateralSpeed = 0x1.0000000000000p+0f,
    .lateralSpeedRandomness = 0x1.8000000000000p+0f,
    .accelerationPercent = 0x1.6666660000000p+1f,
    .swayPhaseStep = 0x1.acee9c0000000p-5f,
    .swayAmplitude = 0x1.4000000000000p+2f,
    .swayAmplitudeStep = 0x1.0000000000000p+0f,
    .swayRandomness = 0x1.0000000000000p+0f,
    .colorStartAge = 30,
    .colorTransitionFrames = 10,
};
