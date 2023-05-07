#include <stdio.h>

#include "freelist.h"

typedef struct {
  u64 size, mall, all;
  char *data;
  char *first_free;
} l_intern;

int main() {
  freelist list = NULL;
  freelist_create(1024, 1, &list);

  void *alloc = freelist_allocate(list, 20);
  if (alloc == NULL)
    return 1;

  char *list_data = ((l_intern *)list)->data;
  printf("Allocation OK %zu\n", (u64)((u64)alloc - (u64)list_data));

  freelist_free(list, &alloc);

  alloc = freelist_allocate(list, 1024);
  if (alloc == NULL)
    return 1;
  printf("Allocation OK %zu\n", (u64)((u64)alloc - (u64)list_data));

  freelist_delete(list);
  return 0;
}
