#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_PERF 0x00090004

int perf_count(void);
unsigned perf_cycles(void);
unsigned perf_instructions_retired(void);

#ifdef __cplusplus
}
#endif
