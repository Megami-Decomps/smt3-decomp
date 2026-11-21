#ifndef POLY_MANAGER_DATA_H
#define POLY_MANAGER_DATA_H

typedef struct
{
    void *(*func)(void *params);
    void (*callback)(void *work);
    void (*destroy)(void *work);
} dds3SpPolyEntry_t;

#endif
