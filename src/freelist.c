#include "freelist.h"

#include <stdlib.h>

typedef struct {
  u64 size;
  char *data;
} freelist_t;

typedef struct {
  u64 size;
  void *next;
  void *prev;
} block_header;

u8 freelist_create(u64 size, freelist *const out_list) {
  if (size == 0)
    return 0;

  // create the handle
  freelist_t *list = malloc(sizeof(freelist_t));
  if (list == NULL)
    return 0;
  // set default parameters
  list->size = size;
  list->data = malloc(size);
  if (list->data == NULL)
    return 0;
  block_header *header = (block_header *)list->data;
  header->size = list->size - sizeof(block_header);
  header->next = header->prev = list->data;

  (*out_list) = list;
  return 1;
}

void *freelist_allocate(u64 size, freelist handle) {
  freelist_t *list = (freelist_t *)handle;
  if (!list || size > list->size)
    return NULL;

  // first-fit algor
  block_header *header = (block_header *)list->data;
  if (header->next == NULL && header->prev == NULL)
    header = (block_header *)(((char *)header) + header->size);
}

void freelist_delete(freelist handle) {
  freelist_t *list = (freelist_t *)handle;
  if (!handle || !list->data)
    return;
  free(list->data);
  free(handle);
}
