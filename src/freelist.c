#include "freelist.h"

#include <stdlib.h>

u8 freelist_create(u64 size, freelist **fl) {
  if (size == 0)
    return 0;

  // create the handle
  *fl = malloc(sizeof(freelist));
  if ((*fl) == NULL)
    return 0;
  // set default parameters
  (*fl)->size = size;
  return 1;
}

u64 freelist_allocate(u64 size, freelist *fl) {}

void freelist_delete(freelist *fl) {
  if (!fl)
    return;
  free(fl);
}
