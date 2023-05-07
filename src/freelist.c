#include "freelist.h"

#include <stdlib.h>

typedef struct {
  u64 size;
  void *next;
} fl_fblock;
typedef struct {
  u64 size;
  u64 max_allocs, allocs;
  char *data;
  fl_fblock *first_free;
} freelist_t;

/**
 * @brief Create a freelist
 *
 * @param[in] size Sizeof list.
 * @param[in] max_allocations Max. number of allocations. Can be 0.
 *
 * @return 0 if creation failed or 1 if it was successful.
 */
u8 freelist_create(u64 size, u64 max_allocations, freelist *const out_list) {
  if (size == 0)
    return 0;

  // create the handle
  freelist_t *list = malloc(sizeof(freelist_t)); // TODO: make only one alloc
  if (list == NULL)
    return 0;
  // set default parameters
  list->size = size + sizeof(u64) * max_allocations;
  list->max_allocs = max_allocations;
  list->allocs = 0;
  list->data = malloc(list->size);
  if (list->data == NULL)
    return 0;
  fl_fblock *fb = (fl_fblock *)list->data;
  fb->size = list->size;
  fb->next = NULL;
  list->first_free = fb;

  (*out_list) = list;
  return 1;
}

void *freelist_allocate(freelist handle, u64 in_size) {
  freelist_t *list = (freelist_t *)handle;
  u64 size = in_size + sizeof(u64);
  if (!list || size > list->size || list->first_free == NULL ||
      list->max_allocs != 0 && list->allocs >= list->max_allocs)
    return NULL;

  // first-fit algor
  {
    fl_fblock *prev_block = NULL;
    fl_fblock *block = list->first_free;
    while (block != NULL) {
      if (block->size >= size) {

        fl_fblock *new_next_block = NULL;
        if (block->size == size) {
          // redirect previous blocks
          new_next_block = block->next;
        } else {
          // create new block from the remaining space
          new_next_block = (fl_fblock *)((char *)block + size);
          new_next_block->size = block->size - size;
          new_next_block->next = block->next;
        }

        // replaces first_free ptr if it was used for allocation
        if (block == list->first_free)
          list->first_free = new_next_block;

        // redirects the previous block
        if (prev_block != NULL)
          prev_block->next = new_next_block;
        // if the block is allocated form first_free
        u64 *size_header = (u64 *)block;
        *size_header = size;
        // increment alloc count
        list->allocs++;
        return (void *)(size_header + 1);
      }
      block = block->next;
    }
  }

  // failed to find suitable block
  return NULL;
}

void _freelist_combine_next(fl_fblock *fb) {
  if (fb == NULL)
    return;
  fl_fblock *fb2 = fb->next;
  if (fb2 != NULL && (fl_fblock *)((char *)fb + fb->size) == fb2) {
    fb->next = fb2->next;
    fb->size += fb2->size;
  }
}

void freelist_free(freelist handle, void **alloc_ref) {
  freelist_t *list = (freelist_t *)handle;
  if (alloc_ref == NULL)
    return;
  void *allocation = *alloc_ref;
  if (list == NULL || allocation == NULL ||
      llabs(((i64)list->data - (i64)allocation)) > list->size)
    return;
  // delete the users pointer to allocation
  *alloc_ref = NULL;
  // decrement alloc count
  list->allocs--;

  fl_fblock *fb = (fl_fblock *)(((u64 *)allocation) - 1);
  fb->size = *(u64 *)fb;
  if ((u64)list->first_free > (u64)fb) {
    fb->next = list->first_free;
    list->first_free = fb;
    _freelist_combine_next(fb);
    return;
  }
  fl_fblock *b_prev = list->first_free, *b_next = NULL;
  while ((u64)b_prev->next < (u64)fb) {
    b_prev = b_prev->next;
  }
  b_next = b_prev->next;

  // register free block
  fb->next = b_next;
  b_prev->next = fb;
  _freelist_combine_next(fb);
  _freelist_combine_next(b_prev);
  return;
}

void freelist_delete(freelist handle) {
  freelist_t *list = (freelist_t *)handle;
  if (!handle || !list->data)
    return;
  free(list->data);
  free(handle);
}
