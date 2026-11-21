#include "pipe.h"

/* Mutable loader state. pipeFileLoad fills these three records together. */
tagPipeLinkInfo linkHeader00 __attribute__((section(".data"))) = { 0 };
tagPipePosTable pipePosData __attribute__((section(".data"))) = { 0 };
tagPipeTable pipeObjectTable __attribute__((section(".data"))) = { 0 };
