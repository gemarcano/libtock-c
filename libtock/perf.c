#include "perf.h"

int perf_count(void) {
  return command(DRIVER_PERF, 0, 0, 0);
}

int perf_cycles(void) {
  return command(DRIVER_PERF, 1, 0, 0);
}
