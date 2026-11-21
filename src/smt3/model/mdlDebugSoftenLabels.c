#include "mdlDebugData.h"

extern const char softenBiasTitle[16];
extern char softenBiasLabelSoften[8], softenBiasLabelBias[8], softenBiasLabelDivide[8];

mdlDebugLabel_t softenBiasLabels[4] = {
    { 0x8500, 0x79C0, 0, softenBiasTitle },
    { 0x85C0, 0x7A80, 0, softenBiasLabelSoften },
    { 0x85C0, 0x7AE0, 0, softenBiasLabelBias },
    { 0x85C0, 0x7BA0, 0, softenBiasLabelDivide },
};
