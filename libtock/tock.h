#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (subscribe_cb)(int, int, int,void*);

int tock_enqueue(subscribe_cb cb, int arg0, int arg1, int arg2, void* ud);

void yield(void);
inline void yield_for(bool *cond) {
  while (!*cond) {
    yield();
  }
}

__attribute__ ((warn_unused_result))
inline int command(uint32_t driver, uint32_t command, int data, int arg2);

__attribute__ ((warn_unused_result))
inline int subscribe(uint32_t driver, uint32_t subscribe,
              subscribe_cb cb, void* userdata);

__attribute__ ((warn_unused_result))
inline int allow(uint32_t driver, uint32_t allow, void* ptr, size_t size);

// op_type can be:
// 0: brk, arg1 is pointer to new memory break
// 1: sbrk, arg1 is increment to increase/decrease memory break
inline void* memop(uint32_t op_type, int arg1);


#if defined(__thumb__)

inline int subscribe(uint32_t driver, uint32_t subscribe,
              subscribe_cb cb, void* userdata) {
  register uint32_t r0 __asm__ ("r0") = driver;
  register uint32_t r1 __asm__ ("r1") = subscribe;
  register void*    r2 __asm__ ("r2") = cb;
  register void*    r3 __asm__ ("r3") = userdata;
  register int ret __asm__ ("r0");
  __asm__ volatile (
    "svc 1"
    : "=r" (ret)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory");
  return ret;
}


inline int command(uint32_t driver, uint32_t command, int data, int arg2) {
  register uint32_t r0 __asm__ ("r0") = driver;
  register uint32_t r1 __asm__ ("r1") = command;
  register uint32_t r2 __asm__ ("r2") = data;
  register uint32_t r3 __asm__ ("r3") = arg2;
  register int ret __asm__ ("r0");
  __asm__ volatile (
    "svc 2"
    : "=r" (ret)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  return ret;
}

inline int allow(uint32_t driver, uint32_t allow, void* ptr, size_t size) {
  register uint32_t r0 __asm__ ("r0") = driver;
  register uint32_t r1 __asm__ ("r1") = allow;
  register void*    r2 __asm__ ("r2") = ptr;
  register size_t r3 __asm__ ("r3")   = size;
  register int ret __asm__ ("r0");
  __asm__ volatile (
    "svc 3"
    : "=r" (ret)
    : "r" (r0), "r" (r1), "r" (r2), "r" (r3)
    : "memory"
    );
  return ret;
}

inline void* memop(uint32_t op_type, int arg1) {
  register uint32_t r0 __asm__ ("r0") = op_type;
  register int r1 __asm__ ("r1")      = arg1;
  register void*   ret __asm__ ("r0");
  __asm__ volatile (
    "svc 4"
    : "=r" (ret)
    : "r" (r0), "r" (r1)
    : "memory"
    );
  return ret;
}

#elif defined(__riscv)

// Implementation of the syscalls for generic RISC-V platforms.
//
// For RISC-V, the arguments are passed through registers a0-a4. Generally,
// the syscall number is put in a0, and the required arguments are specified in
// a1-a4. Nothing specifically syscall related is pushed to the process stack.

inline int subscribe(uint32_t driver, uint32_t subscribe,
              subscribe_cb cb, void* userdata) {
  register uint32_t a1  __asm__ ("a1") = driver;
  register uint32_t a2  __asm__ ("a2") = subscribe;
  register void*    a3  __asm__ ("a3") = cb;
  register void*    a4  __asm__ ("a4") = userdata;
  register int ret __asm__ ("a0");
  __asm__ volatile (
    "li    a0, 1\n"
    "ecall\n"
    : "=r" (ret)
    : "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  return ret;
}

inline int command(uint32_t driver, uint32_t command, int data, int arg2) {
  register uint32_t a1  __asm__ ("a1") = driver;
  register uint32_t a2  __asm__ ("a2") = command;
  register uint32_t a3  __asm__ ("a3") = data;
  register uint32_t a4  __asm__ ("a4") = arg2;
  register int ret __asm__ ("a0");
  __asm__ volatile (
    "li    a0, 2\n"
    "ecall\n"
    : "=r" (ret)
    : "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  return ret;
}

inline int allow(uint32_t driver, uint32_t allow, void* ptr, size_t size) {
  register uint32_t a1  __asm__ ("a1") = driver;
  register uint32_t a2  __asm__ ("a2") = allow;
  register void*    a3  __asm__ ("a3") = ptr;
  register size_t a4  __asm__ ("a4")   = size;
  register int ret __asm__ ("a0");
  __asm__ volatile (
    "li    a0, 3\n"
    "ecall\n"
    : "=r" (ret)
    : "r" (a1), "r" (a2), "r" (a3), "r" (a4)
    : "memory");
  return ret;
}

inline void* memop(uint32_t op_type, int arg1) {
  register uint32_t a1  __asm__ ("a1") = op_type;
  register uint32_t a2  __asm__ ("a2") = arg1;
  register void*    ret __asm__ ("a0");
  __asm__ volatile (
    "li    a0, 4\n"
    "ecall\n"
    : "=r" (ret)
    : "r" (a1), "r" (a2)
    : "memory");
  return ret;
}

#endif

// Wrappers around memop to support app introspection
void* tock_app_memory_begins_at(void);
void* tock_app_memory_ends_at(void);
void* tock_app_flash_begins_at(void);
void* tock_app_flash_ends_at(void);
void* tock_app_grant_begins_at(void);
int tock_app_number_writeable_flash_regions(void);
void* tock_app_writeable_flash_region_begins_at(int region_index);
void* tock_app_writeable_flash_region_ends_at(int region_index);


// Checks to see if the given driver number exists on this platform.
bool driver_exists(uint32_t driver);

#define TOCK_SUCCESS       0
#define TOCK_FAIL         -1
#define TOCK_EBUSY        -2
#define TOCK_EALREADY     -3
#define TOCK_EOFF         -4
#define TOCK_ERESERVE     -5
#define TOCK_EINVAL       -6
#define TOCK_ESIZE        -7
#define TOCK_ECANCEL      -8
#define TOCK_ENOMEM       -9
#define TOCK_ENOSUPPORT   -10
#define TOCK_ENODEVICE    -11
#define TOCK_EUNINSTALLED -12
#define TOCK_ENOACK       -13

// Pass this to the subscribe syscall as a function pointer to deactivate the callback.
#define TOCK_DEACTIVATE_CALLBACK    0

// Pass this to the allow syscall as pointer to revoke the "allow"-syscall.
#define TOCK_REVOKE_ALLOW           0

const char* tock_strerror(int tock_errno);
void tock_expect(int expected, int actual, const char* file, unsigned line);
#define TOCK_EXPECT(_e, _a) tock_expect((_e), (_a), __FILE__, __LINE__)

#ifdef __cplusplus
}
#endif
