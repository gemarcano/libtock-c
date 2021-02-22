#pragma once

#include "tock.h"

#ifdef __cplusplus
extern "C" {
#endif

#define DRIVER_PERF 0x00090004

int perf_count(void);
int perf_cycles(void);
int perf_instructions_retired(void);

#ifdef __cplusplus
}
#endif
