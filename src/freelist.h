#ifndef _FREELIST_H
#define _FREELIST_H

#include "types.h"

typedef void *freelist;

u8 freelist_create(u64 size, freelist *const fl);
void *freelist_allocate(u64 size, freelist freelist);
void freelist_delete(freelist freelist);

#endif
