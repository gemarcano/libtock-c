#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_PERF 0x00090004

inline unsigned perf_cycles(void) {
  return command(DRIVER_PERF, 1, 0, 0);
}

inline unsigned perf_instructions_retired(void) {
  return (unsigned)command(DRIVER_PERF, 2, 0, 0);
}
#ifdef __cplusplus
}
#endif
