#include "fpu.h"
#include "common.h"
#include "ps2asm.h"
#include "basic.h"
#include "motion.h"
#include "pipe.h"

extern void (*D_00551828)(void);

extern f32 sdfSin(f32 aRadius);

extern f32 sdfCos(f32 aRadius);

extern void *memset(void *a, s32 b, u32 c);

extern void *memcpy(void *a, const void *b, u32 c);

extern void *sdfMemGetPtr(void *aHandle);

extern void sdfMemFreeDirect(void *aHandle);

f32 vecLength(f32 *vector);

f32 vecNormalize(f32 *vector);

f32 func_002A7DF8(f32 base, f32 exponent);

typedef struct tagPipeLinkInfo {
    sdfMemHandle_t *aHandle;
    tagPipeLinkData *head;
    s32 maxNum;
    s32 nextId;
} tagPipeLinkInfo;

extern void vecRotate2D(f32 *vector, f32 angle);

extern void pipeFreeLink(tagPipeLinkInfo *pInfo);

extern s32 pipeInitLink(tagPipeLinkInfo *pInfo);

extern f32 vecDot(f32 *a, f32 *b);

extern f32 __ieee754_acosf(f32 x);

extern void *sdfMemAllocDirect(s32 aSize);


extern s32 pipeLinkGetNum(u8 *data, s32 size);
extern s32 pipeCreateLink(tagPipeLinkInfo *info, s32 count);
extern tagPipeLinkData *pipeMakeNodeLink(tagPipeLinkInfo *info);
extern s32 pipeConnectLink(tagPipeLinkData *previous, tagPipeLinkData *next);

#ifdef NON_MATCHING
s32 LoadLinkData(tagPipeLinkInfo *info, pipeLinkSource_t *source, s32 mode)
{
    u8 *data = source->data;
    s32 count = pipeLinkGetNum(data, source->count);
    tagPipeLinkData *previous;
    tagPipeLinkData *last = NULL;
    s8 direction;
    s8 reverse;
    s32 turn;
    s32 i;
    u8 code;

    pipeCreateLink(info, count);
    previous = pipeMakeNodeLink(info);
    code = *data++;
    if (code >= 11) {
        direction = (s8)(code - 10);
        turn = 1;
    } else {
        direction = code == 0 ? 6 : (s8)code;
        turn = 0;
    }
    previous->partsID = (turn << 6) | direction | 8;
    previous->id = 1;
    reverse = 7 - direction;
    info->head = previous;
    for (i = 1; i < count; i++) {
        last = pipeMakeNodeLink(info);
        code = *data++;
        if (code >= 11) {
            direction = (s8)(code - 10);
            turn = 1;
        } else {
            direction = code == 0 ? 6 : (s8)code;
            turn = 0;
        }
        last->partsID = (turn << 6) | ((u32)(s32)reverse << 3) | direction;
        reverse = 7 - direction;
        pipeConnectLink(previous, last);
        previous = last;
    }
    last->id = 0xFFFF;
    return 1;
}
#else
INCLUDE_ASM(const s32, "pipe/mnMyLink", LoadLinkData);
#endif

s32 pipeLinkGetNum(u8 *lPtr, s32 size)
{
    s32 num = 1;

    while (num < size && *lPtr != 0) {
        lPtr++;
        num++;
    }
    return num;
}

s32 pipeCreateLink(tagPipeLinkInfo *pInfo, s32 nodeNum)
{
    if (pInfo == NULL) {
        return 0;
    }
    pInfo->aHandle = sdfMemAllocDirect(nodeNum * 16);
    pInfo->maxNum = nodeNum;
    pipeInitLink(pInfo);
    return 1;
}

s32 pipeInitLink(tagPipeLinkInfo *pInfo)
{
    u16 *pNode;
    s32 i;

    if (pInfo == NULL) {
        return 0;
    }
    pNode = sdfMemGetPtr(pInfo->aHandle);
    i = pInfo->maxNum;
    if (i > 0) {
        do {
            i--;
            *pNode = 0;
            pNode += 8;
        } while (i != 0);
    }
    pInfo->head = NULL;
    pInfo->nextId = 1;
    return 0;
}


/* NON_MATCHING: 7 of 32 words, exact size; two registers are swapped. */
#ifdef NON_MATCHING
tagPipeLinkData *pipeMakeNodeLink(tagPipeLinkInfo *pInfo)
{
    tagPipeLinkData *pNode = sdfMemGetPtr(pInfo->aHandle);
    s32 i;

    for (i = 0; i < pInfo->maxNum; i++) {
        if (pNode->id == 0) {
            s32 *pNextId = &pInfo->nextId;
            s32 id = *pNextId;

            pNode->next = NULL;
            pNode->id = id;
            *pNextId = id + 1;
            pNode->prev = NULL;
            return pNode;
        }
        pNode++;
    }
    return NULL;
}
#else
INCLUDE_ASM(const s32, "pipe/mnMyLink", pipeMakeNodeLink);
#endif


s32 pipeConnectLink(tagPipeLinkData *pPrev, tagPipeLinkData *pNext)
{
    if (pPrev == NULL || pNext == NULL) {
        return 0;
    }
    pPrev->next = pNext;
    pNext->prev = pPrev;
    return 1;
}

s32 pipeDellLink(tagPipeLinkData *pData)
{
    tagPipeLinkData *prev;
    tagPipeLinkData *next;

    if (pData == NULL) {
        return 0;
    }
    prev = pData->prev;
    next = pData->next;
    if (prev != NULL) {
        prev->next = next;
    }
    if (next != NULL) {
        next->prev = prev;
    }
    return 1;
}

void pipeFreeLink(tagPipeLinkInfo *pInfo)
{
    if (pInfo != NULL) {
        if (pInfo->aHandle != NULL) {
            pipeInitLink(pInfo);
            sdfMemFreeDirect(pInfo->aHandle);
            pInfo->aHandle = NULL;
        }
    }
}

void pipeCameraLerp(f32 *vector, f32 angle) {
    f32 rotated[4];
    f32 cosine = sdfCos(angle);
    f32 sine = sdfSin(angle);

    rotated[1] = vector[1] * cosine + vector[2] * sine;
    sine = sdfSin(angle);
    cosine = sdfCos(angle);
    rotated[2] = vector[1] * -sine + vector[2] * cosine;
    vector[1] = rotated[1];
    vector[2] = rotated[2];
}

void vec_2A6B80(f32 *vector, f32 angle) {
    f32 rotated[4];
    f32 cosine = sdfCos(angle);
    f32 sine = sdfSin(angle);

    rotated[0] = vector[0] * cosine - vector[2] * sine;
    sine = sdfSin(angle);
    cosine = sdfCos(angle);
    rotated[2] = vector[0] * sine + vector[2] * cosine;
    vector[0] = rotated[0];
    vector[2] = rotated[2];
}

void vecRotate2D(f32 *vector, f32 angle) {
    f32 rotated[4];
    f32 cosine = sdfCos(angle);
    f32 sine = sdfSin(angle);

    rotated[0] = vector[0] * cosine + vector[1] * sine;
    sine = sdfSin(angle);
    cosine = sdfCos(angle);
    rotated[1] = vector[0] * -sine + vector[1] * cosine;
    vector[0] = rotated[0];
    vector[1] = rotated[1];
}

void vec_2A6CA8(float *vector, float *axis, float angle) {
    sdfVector4_t normalizedAxis;
    sdfVector4_t scratch;
    sdfVector4_t inputVector;
    float rotationMatrix[9];

    memset(&scratch, 0, sizeof(scratch));
    scratch.x = axis[0];
    scratch.y = axis[1];
    scratch.z = axis[2];
    normalizedAxis = scratch;
    memset(&inputVector, 0, sizeof(inputVector));
    inputVector.x = vector[0];
    inputVector.y = vector[1];
    inputVector.z = vector[2];
    scratch = inputVector;
    vecNormalize(&normalizedAxis.x);
    rotationMatrix[0] = normalizedAxis.x * normalizedAxis.x * (1.0f - sdfCos(angle)) + sdfCos(angle);
    rotationMatrix[1] = normalizedAxis.x * normalizedAxis.y * (1.0f - sdfCos(angle)) - normalizedAxis.z * sdfSin(angle);
    rotationMatrix[2] = normalizedAxis.x * normalizedAxis.z * (1.0f - sdfCos(angle)) + normalizedAxis.y * sdfSin(angle);
    rotationMatrix[3] = normalizedAxis.y * normalizedAxis.x * (1.0f - sdfCos(angle)) + normalizedAxis.z * sdfSin(angle);
    rotationMatrix[4] = normalizedAxis.y * normalizedAxis.y * (1.0f - sdfCos(angle)) + sdfCos(angle);
    rotationMatrix[5] = normalizedAxis.y * normalizedAxis.z * (1.0f - sdfCos(angle)) - normalizedAxis.x * sdfSin(angle);
    rotationMatrix[6] = normalizedAxis.z * normalizedAxis.x * (1.0f - sdfCos(angle)) - normalizedAxis.y * sdfSin(angle);
    rotationMatrix[7] = normalizedAxis.z * normalizedAxis.y * (1.0f - sdfCos(angle)) + normalizedAxis.x * sdfSin(angle);
    rotationMatrix[8] = normalizedAxis.z * normalizedAxis.z * (1.0f - sdfCos(angle)) + sdfCos(angle);
    vector[0] = scratch.x * rotationMatrix[0] + scratch.y * rotationMatrix[3] + scratch.z * rotationMatrix[6];
    vector[1] = scratch.x * rotationMatrix[1] + scratch.y * rotationMatrix[4] + scratch.z * rotationMatrix[7];
    vector[2] = scratch.x * rotationMatrix[2] + scratch.y * rotationMatrix[5] + scratch.z * rotationMatrix[8];
}

void vec_2A6FF0(f32 *a, f32 *b)
{
    a[0] += b[0];
    a[1] += b[1];
    a[2] += b[2];
}

void vec_2A7028(f32 *a, f32 *b)
{
    a[0] -= b[0];
    a[1] -= b[1];
    a[2] -= b[2];
}

void vec_2A7060(f32 *v, f32 x, f32 y, f32 z)
{
    v[0] += x;
    v[1] += y;
    v[2] += z;
}

void vecSet(f32 *v, f32 x, f32 y, f32 z)
{
    v[0] = x;
    v[1] = y;
    v[2] = z;
}

void vecScale(f32 *v, f32 scale)
{
    v[0] *= scale;
    v[1] *= scale;
    v[2] *= scale;
}

float vecNormalize(float *vector) {
    float length = vecLength(vector);

    vector[0] = vector[0] / length;
    vector[1] = vector[1] / length;
    vector[2] = vector[2] / length;
    return length;
}

f32 vecLength(f32 *v)
{
    f32 len;

    len = sdfSqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
    return len;
}

float vecDot(float *left, float *right) {
    sdfVector4_t normalizedLeft;
    sdfVector4_t normalizedRight;

    normalizedLeft = *(sdfVector4_t *)left;
    normalizedRight = *(sdfVector4_t *)right;
    vecNormalize(&normalizedLeft.x);
    vecNormalize(&normalizedRight.x);
    return normalizedLeft.x * normalizedRight.x + normalizedLeft.y * normalizedRight.y + normalizedLeft.z * normalizedRight.z;
}

f32 vecAngleBetween(f32 *a, f32 *b)
{
    return __ieee754_acosf(vecDot(a, b));
}

void vecTranslate(float *output, float *left, float *right) {
    sdfVector4_t normalizedLeft;
    sdfVector4_t normalizedRight;

    normalizedLeft = *(sdfVector4_t *)left;
    normalizedRight = *(sdfVector4_t *)right;
    vecNormalize(&normalizedLeft.x);
    vecNormalize(&normalizedRight.x);
    output[0] = normalizedLeft.y * normalizedRight.z - normalizedLeft.z * normalizedRight.y;
    output[1] = normalizedLeft.z * normalizedRight.x - normalizedLeft.x * normalizedRight.z;
    output[2] = normalizedLeft.x * normalizedRight.y - normalizedLeft.y * normalizedRight.x;
}

void mtxRotateX(sdfMatrix4_t *matrix, f32 angle) {
    sdfMatrix4_t rotated;

    rotated.m[0] = matrix->m[0];
    rotated.m[1] = matrix->m[1];
    rotated.m[2] = matrix->m[2];
    rotated.m[3] = matrix->m[3];
    rotated.m[4] = matrix->m[4] * sdfCos(angle) + matrix->m[8] * sdfSin(angle);
    rotated.m[5] = matrix->m[5] * sdfCos(angle) + matrix->m[9] * sdfSin(angle);
    rotated.m[6] = matrix->m[6] * sdfCos(angle) + matrix->m[10] * sdfSin(angle);
    rotated.m[7] = matrix->m[7] * sdfCos(angle) + matrix->m[11] * sdfSin(angle);
    rotated.m[8] = matrix->m[4] * -sdfSin(angle) + matrix->m[8] * sdfCos(angle);
    rotated.m[9] = matrix->m[5] * -sdfSin(angle) + matrix->m[9] * sdfCos(angle);
    rotated.m[10] = matrix->m[6] * -sdfSin(angle) + matrix->m[10] * sdfCos(angle);
    rotated.m[11] = matrix->m[7] * -sdfSin(angle) + matrix->m[11] * sdfCos(angle);
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

void mtxRotateY(sdfMatrix4_t *matrix, f32 angle) {
    sdfMatrix4_t rotated;

    rotated.m[0] = matrix->m[0] * sdfCos(angle) + matrix->m[8] * -sdfSin(angle);
    rotated.m[1] = matrix->m[1] * sdfCos(angle) + matrix->m[9] * -sdfSin(angle);
    rotated.m[2] = matrix->m[2] * sdfCos(angle) + matrix->m[10] * -sdfSin(angle);
    rotated.m[3] = matrix->m[3] * sdfCos(angle) + matrix->m[11] * -sdfSin(angle);
    rotated.m[4] = matrix->m[4];
    rotated.m[5] = matrix->m[5];
    rotated.m[6] = matrix->m[6];
    rotated.m[7] = matrix->m[7];
    rotated.m[8] = matrix->m[0] * sdfSin(angle) + matrix->m[8] * sdfCos(angle);
    rotated.m[9] = matrix->m[1] * sdfSin(angle) + matrix->m[9] * sdfCos(angle);
    rotated.m[10] = matrix->m[2] * sdfSin(angle) + matrix->m[10] * sdfCos(angle);
    rotated.m[11] = matrix->m[3] * sdfSin(angle) + matrix->m[11] * sdfCos(angle);
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

void mtxRotateZ(sdfMatrix4_t *matrix, f32 angle) {
    sdfMatrix4_t rotated;

    rotated.m[0] = matrix->m[0] * sdfCos(angle) + matrix->m[4] * sdfSin(angle);
    rotated.m[1] = matrix->m[1] * sdfCos(angle) + matrix->m[5] * sdfSin(angle);
    rotated.m[2] = matrix->m[2] * sdfCos(angle) + matrix->m[6] * sdfSin(angle);
    rotated.m[3] = matrix->m[3] * sdfCos(angle) + matrix->m[7] * sdfSin(angle);
    rotated.m[4] = matrix->m[0] * -sdfSin(angle) + matrix->m[4] * sdfCos(angle);
    rotated.m[5] = matrix->m[1] * -sdfSin(angle) + matrix->m[5] * sdfCos(angle);
    rotated.m[6] = matrix->m[2] * -sdfSin(angle) + matrix->m[6] * sdfCos(angle);
    rotated.m[7] = matrix->m[3] * -sdfSin(angle) + matrix->m[7] * sdfCos(angle);
    rotated.m[8] = matrix->m[8];
    rotated.m[9] = matrix->m[9];
    rotated.m[10] = matrix->m[10];
    rotated.m[11] = matrix->m[11];
    rotated.m[12] = matrix->m[12];
    rotated.m[13] = matrix->m[13];
    rotated.m[14] = matrix->m[14];
    rotated.m[15] = matrix->m[15];
    *matrix = rotated;
}

void mtxStore(sdfMatrix4_t *pDst, sdfMatrix4_t *pSrc) {
    sdfMatrix4_t sourceCopy = *pSrc;

    pDst->m[0] = sourceCopy.m[0];
    pDst->m[1] = sourceCopy.m[4];
    pDst->m[2] = sourceCopy.m[8];
    pDst->m[3] = sourceCopy.m[12];
    pDst->m[4] = sourceCopy.m[1];
    pDst->m[5] = sourceCopy.m[5];
    pDst->m[6] = sourceCopy.m[9];
    pDst->m[7] = sourceCopy.m[13];
    pDst->m[8] = sourceCopy.m[2];
    pDst->m[9] = sourceCopy.m[6];
    pDst->m[10] = sourceCopy.m[10];
    pDst->m[11] = sourceCopy.m[14];
    pDst->m[12] = sourceCopy.m[3];
    pDst->m[13] = sourceCopy.m[7];
    pDst->m[14] = sourceCopy.m[11];
    pDst->m[15] = sourceCopy.m[15];
}

f32 *mtx_2A7B10(f32 *direction, f32 *matrix) {
    f32 transformed[4];
    f32 x = direction[0];
    f32 y = direction[1];
    f32 z = direction[2];

    transformed[0] = x * matrix[0] + y * matrix[4] + z * matrix[8];
    transformed[1] = x * matrix[1] + y * matrix[5] + z * matrix[9];
    transformed[2] = x * matrix[2] + y * matrix[6] + z * matrix[10];
    memcpy(direction, transformed, 16);
    return direction;
}

void *mtx_2A7BB8(sdfVector4_t *controlPoints, s32 degree, s32 sampleCount) {
    void *outputBlock;
    void *coefficientBlock;
    f32 *coefficients;
    sdfVector4_t *output;
    sdfVector4_t *sample;
    s32 i;
    s32 j;

    coefficientBlock = sdfMemAllocDirect(degree + 1);
    coefficients = (f32 *)sdfMemGetPtr(coefficientBlock);
    outputBlock = sdfMemAllocDirect(sampleCount + 2);
    output = (sdfVector4_t *)sdfMemGetPtr(outputBlock);

    for (i = 0; i <= degree; i++) {
        coefficients[i] = 1.0f;
        for (j = 1; j <= i; j++) {
            coefficients[i] = (coefficients[i] * (f32)(degree - j + 1)) / (f32)j;
        }
    }

    i = 0;
    output[0].x = controlPoints[0].x;
    output[0].y = controlPoints[0].y;
    output[0].z = controlPoints[0].z;

    if (sampleCount >= 0) {
        sample = output;
        do {
            f32 t = (f32)i / (f32)sampleCount;
            f32 x = 0.0f;
            f32 y = 0.0f;
            f32 z = 0.0f;

            j = 0;
            if (degree >= 0) {
                sdfVector4_t *point = controlPoints;
                f32 *coefficient = coefficients;
                do {
                    f32 powers = func_002A7DF8(t, (f32)j);
                    f32 weight;

                    powers *= func_002A7DF8(1.0f - t, (f32)(degree - j));
                    weight = *coefficient * powers;
                    x += point->x * weight;
                    y += point->y * weight;
                    z += point->z * weight;
                    point++;
                    coefficient++;
                    j++;
                } while (j <= degree);
            }

            sample->x = x;
            sample->y = y;
            sample->z = z;
            sample++;
            i++;
        } while (i <= sampleCount);
    }

    sdfMemFreeDirect(coefficientBlock);
    return outputBlock;
}

f32 func_002A7DF8(f32 value, f32 count)
{
    f32 result = 1.0f;
    s32 i;

    for (i = 1; i <= count; i++) {
        result *= value;
    }
    return result;
}

void vecDistance(f32 quaternion[4], f32 matrix[4][4]) {
    f32 trace;
    f32 scale;
    s32 i;
    u8 j;
    u8 k;
    f32 xx = matrix[0][0];
    f32 yy = matrix[1][1];
    f32 zz = matrix[2][2];

    trace = xx + yy + zz + 1.0f;
    if (trace >= 1.0f) {
        scale = fsqrtf(trace) * 2.0f;
        quaternion[3] = scale * 0.25f;
        quaternion[0] = (matrix[1][2] - matrix[2][1]) / scale;
        quaternion[1] = (matrix[2][0] - matrix[0][2]) / scale;
        quaternion[2] = (matrix[0][1] - matrix[1][0]) / scale;
    } else {
        i = xx > yy ? 0 : 1;
        if (zz > matrix[i][i]) {
            i = 2;
        }
        j = (i + 1) % 3;
        k = (j + 1) % 3;
        scale = fsqrtf(matrix[i][i] - matrix[j][j] - matrix[k][k] + 1.0f) * 2.0f;
        if (scale != 0.0f) {
            quaternion[i] = scale * 0.25f;
            quaternion[j] = (matrix[i][j] + matrix[j][i]) / scale;
            quaternion[k] = (matrix[i][k] + matrix[k][i]) / scale;
            quaternion[3] = (matrix[j][k] - matrix[k][j]) / scale;
        } else {
            quaternion[i] = 1.0f;
            quaternion[j] = 0.0f;
            quaternion[k] = 0.0f;
            quaternion[3] = 0.0f;
        }
    }
}
