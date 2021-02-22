/* vim: set sw=2 expandtab tw=80: */

#include <stdio.h>
#include <perf.h>

int main(void) {
  unsigned count = driver_exists(DRIVER_PERF);
  unsigned cycles1 = perf_cycles();
  unsigned cycles2 = perf_cycles();
  printf("Perf available: %u\n", count);
  printf("Perf1: %u, Perf2: %u, diff: %u\n",
    cycles1, cycles2, cycles2 - cycles1);
  return 0;
}
