#include "embfetch_rtos_platform.h"

#if defined(EMBFETCH_FREERTOS) || defined(EMBFETCH_ESPIDF)

#if defined(EMBFETCH_FREERTOS)
#include "FreeRTOS.h"
#elif defined(EMBFETCH_ESPIDF)
#include "freertos/FreeRTOS.h"
#endif

#include "rtos_sysinfo.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "task.h"

// Extern — implemented by platform-specific file (sysinfo_freertos.c or sysinfo_esp-idf.c)
// board_info is non-const to allow runtime mutation on platforms like ESP-IDF
// where mcu and hostname are resolved via chip_info_init()
extern sysinfo_static_t board_info;
extern void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst);
extern void sysinfo_fetch(sysinfo_dynamic_t *dst);

// Byte formatting
void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024 * 1024) {
        snprintf(dst, len, "%uMB", (unsigned int)(bytes >> 20));
    } else if (bytes >= 1024) {
        snprintf(dst, len, "%uKB", (unsigned int)(bytes >> 10));
    } else {
        snprintf(dst, len, "%uB", (unsigned int)bytes);
    }
}

// Print logo and info — no platform ifdefs, NULL checks handle all cases
void sysinfo_print(sysinfo_putline_fn putline, void *ctx) {
    sysinfo_dynamic_t dyn;
    sysinfo_fetch(&dyn);
    sysinfo_hwinfo_t hw;
    sysinfo_hwinfo_fetch(&hw);

    char header[64], separator[32];
    char os_line[64], kernel_line[64], mcu_line[64], build_line[64],
         flash_line[64], ram_line[64], uptime_line[64], heap_line[64];

    snprintf(header,      sizeof(header),      "%s@%s",
             board_info.username ? board_info.username : "root",
             board_info.hostname ? board_info.hostname : "Unknown");
    snprintf(separator,   sizeof(separator),   "----------------");
    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
    snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %" PRIu32 "h %" PRIu32 "m %" PRIu32 "s",
             dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
    snprintf(build_line,  sizeof(build_line),  "Build:   %s", board_info.build_date);
    snprintf(mcu_line,    sizeof(mcu_line),    "MCU:     %s",
             board_info.mcu ? board_info.mcu : "Unknown");
    snprintf(ram_line,    sizeof(ram_line),    "Memory:  %s", hw.ram);
    snprintf(heap_line,   sizeof(heap_line),   "Heap:    %s used / %s free", dyn.heap_used, dyn.heap_free);
    snprintf(flash_line,  sizeof(flash_line),  "Flash:   %s", hw.flash);

    const char *info[] = {
        header, separator, os_line, kernel_line,
        uptime_line, build_line, mcu_line, ram_line,
        heap_line, flash_line
    };

    int logo_count = 0;
    while (logo[logo_count] != NULL) logo_count++;
    int info_count = sizeof(info) / sizeof(info[0]);
    int total_lines = (logo_count > info_count) ? logo_count : info_count;

    for (int i = 0; i < total_lines; i++) {
        const char *logo_part = (i < logo_count) ? logo[i] : "";
        const char *info_part = (i < info_count) ? info[i] : "";

        char line[256];
        if (info_part[0] != '\0')
            snprintf(line, sizeof(line), "%s" LOGO_INFO_COLUMN "%s", logo_part, info_part);
        else
            snprintf(line, sizeof(line), "%s", logo_part);
        putline(ctx, line);
    }
}
#endif // sysinfo_freertos_common.c