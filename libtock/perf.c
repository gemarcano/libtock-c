#include "perf.h"

int perf_count(void) {
  return command(DRIVER_PERF, 0, 0, 0);
}

unsigned perf_cycles(void) {
  return (unsigned)command(DRIVER_PERF, 1, 0, 0);
}

unsigned perf_instructions_retired(void) {
  return (unsigned)command(DRIVER_PERF, 2, 0, 0);
}
