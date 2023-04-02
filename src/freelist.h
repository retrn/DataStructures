#ifndef _FREELIST_H
#define _FREELIST_H

#include "types.h"

typedef struct freelist {
  u64 size;
} freelist;

u8 freelist_create(u64 size, freelist **fl);
u64 freelis_allocate(u64 size, freelist *freelist);

#endif
