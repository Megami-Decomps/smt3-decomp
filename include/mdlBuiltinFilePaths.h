#ifndef MDL_BUILTIN_FILE_PATHS_H
#define MDL_BUILTIN_FILE_PATHS_H

/* Same three-pointer row as mdlManager's file table (texture, model, motion). */
typedef struct {
    char *texFile;
    char *modelFile;
    char *motionFile;
} dds3ModelFileTable_t;

/* mdlManager reads the built-in path text. Only the separate HDD slot owns
 * mutable heap strings. Preserve the native row ABI for these const symbols. */
#define MDL_BUILTIN_PATH(symbol) ((char *)(symbol))

#endif
