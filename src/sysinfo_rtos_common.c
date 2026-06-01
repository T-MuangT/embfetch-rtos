#include <stdio.h>
#include "rtos_sysinfo.h"

// This file provides common output wrapper functions for embfetch-rtos.
// The actual fetching and sysinfo_print() are implemented in platform-family
// common files (e.g. sysinfo_zephyr_common.c) and platform-specific files
// (e.g. sysinfo_zephyr.c).

static void file_putline(void *ctx, const char *line) {
    fprintf((FILE *)ctx, "%s\n", line);
}

void sysinfo_print_file(FILE *f) {
    sysinfo_print(file_putline, f);
}

// Wrap function pointer in a struct to avoid casting function pointer
// to void * which is undefined behavior in strict C99/C11.
typedef struct {
    void (*fn)(const char *);
} puts_ctx_t;

static void puts_putline(void *ctx, const char *line) {
    puts_ctx_t *p = (puts_ctx_t *)ctx;
    p->fn(line);
}

void sysinfo_print_puts(void (*puts_fn)(const char *)) {
    puts_ctx_t ctx = { puts_fn };
    sysinfo_print(puts_putline, &ctx);
} // sysinfo_rtos_common.c