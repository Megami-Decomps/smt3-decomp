#include "model.h"

struct mdlLoadingTag_t;

/* Per-major heads for the loaded resources and outstanding load requests. */
dds3ModelResrc_t *mdlResrcMajorList[8] __attribute__((section(".data"))) = { 0 };
struct mdlLoadingTag_t *mdlLoadingMajorList[8] __attribute__((section(".data"))) = { 0 };
