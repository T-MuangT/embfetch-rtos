#ifndef RTOS_SYSINFO_H
#define RTOS_SYSINFO_H

#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

typedef void (*sysinfo_putline_fn)(void *ctx, const char *line);

// Static board info.
// All fields are present on all platforms.
// Fields that are not applicable on a given platform should be set to NULL.
// - username: NULL on NuttX (fetched dynamically via sysinfo_fetch())
// - mcu, hostname: NULL on ESP-IDF (resolved at runtime via chip_info_init())
typedef struct {
    const char *os_name;
    const char *build_date;
    const char *username;       // NULL on NuttX
    const char *mcu;            // NULL on ESP-IDF, resolved via chip_info_init()
    const char *hostname;       // NULL on ESP-IDF, resolved via chip_info_init()
} sysinfo_static_t;

// Hardware info.
typedef struct {
    char flash[16];
    char ram[16];
} sysinfo_hwinfo_t;

// Dynamic runtime info.
// username: populated on NuttX only, NULL or empty on other platforms.
typedef struct {
    char     kernel_version[32];
    uint32_t uptime_h;
    uint32_t uptime_m;
    uint32_t uptime_s;
    char     heap_used[16];
    char     heap_free[16];
    char     username[32];      // populated on NuttX, empty string on other platforms
} sysinfo_dynamic_t;

// Public API
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst);
void sysinfo_fetch(sysinfo_dynamic_t *dst);
void sysinfo_print(sysinfo_putline_fn putline, void *ctx);

// Output wrappers — implemented in sysinfo_rtos_common.c
void sysinfo_print_file(FILE *f);
void sysinfo_print_puts(void (*puts_fn)(const char *));

#endif // RTOS_SYSINFO_H