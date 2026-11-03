#ifndef HOST64_EEADDR_H
#define HOST64_EEADDR_H

/* The EE address-space transforms, identity on a host with one flat address
 * space. Force-included ahead of include/eeregs.h, whose own #ifndef guards
 * then skip the EE bodies. */
#define EE_PHYSICAL(addr) (addr)
#define EE_UNCACHED(addr) (addr)
#define EE_UNCACHED_RING(addr) (addr)

#endif /* HOST64_EEADDR_H */
