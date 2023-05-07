#ifndef _FREELIST_H
#define _FREELIST_H

#include "types.h"

typedef void *freelist;

u8 freelist_create(u64 size, u64 max_allocations, freelist *const fl);
void *freelist_allocate(freelist handle, u64 size);
void freelist_free(freelist handle, void **allocation);
void freelist_delete(freelist freelist);

#endif
