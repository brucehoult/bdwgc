#include <stdio.h>
#include "gc.h"

#define COUNT 1000

int failed = 0;

void test(char *p, int i, const char *name, void *want_fn){
  size_t sz;
  GC_alloc_func fn;

  fn = GC_alloc_fn_and_size(p, &sz);
  if (fn != want_fn){
    fprintf(stderr, "Wrong alloc func returned for %s\n", name);
    failed = 1;
  }
  if (sz < i){
    fprintf(stderr, "Didn't expect an object smaller than asked for!"
            " %d vs %d (%s)\n", (int)sz, i, name);
    failed = 1;
  }
}

int main(void) {
  int i;

  GC_INIT();

  for (i = 0; i < COUNT; i++) {
    char *p1 = GC_malloc(i);
    char *p2 = GC_malloc_atomic(i);
    char *p3 = GC_malloc_uncollectable(i);
#   ifdef ATOMIC_UNCOLLECTABLE
    char *p4 = GC_malloc_atomic_uncollectable(i);
#   endif

    test(p1, i, "GC_malloc", GC_malloc);
    test(p2, i, "GC_malloc_atomic", GC_malloc_atomic);
    test(p3, i, "GC_malloc_uncollectable", GC_malloc_uncollectable);
#   ifdef ATOMIC_UNCOLLECTABLE
    test(p4, i, "GC_malloc_atomic_uncollectable", GC_malloc_atomic_uncollectable);
#   endif
  }

  return failed;
}
