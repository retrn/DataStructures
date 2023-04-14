#include <stdio.h>

#include "freelist.h"

int main() {
  freelist list = NULL;
  freelist_create(1024, &list);

  return 0;
}
