#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "rtos_sysinfo.h"
#include "logo.h"

#if defined(ESP_PLATFORM)
#include <inttypes.h>
#endif

// Extern, implemented by platform-specific C file

extern const sysinfo_static_t board_info;
extern void format_size(char *dst, size_t len, size_t bytes);
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

// Print logo and info
void sysinfo_print(sysinfo_putline_fn putline, void *ctx) {
    sysinfo_dynamic_t dyn;
    sysinfo_fetch(&dyn);
    sysinfo_hwinfo_t hw;
    sysinfo_hwinfo_fetch(&hw);

    char header[64], separator[32];
    char os_line[64], kernel_line[64], mcu_line[64], build_line[64],
         flash_line[64], ram_line[64], uptime_line[64], heap_line[64];
#if defined(ESP_PLATFORM)
	// hostname and mcu are runtime-resolved in sysinfo_esp-idf.c
	extern const char *mcu_name;
	extern const char *hostname;
	snprintf(header,   sizeof(header),   "%s@%s", board_info.username, hostname);
	snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %" PRIu32 "h %" PRIu32 "m %" PRIu32 "s",
	         dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
	snprintf(mcu_line, sizeof(mcu_line), "MCU:     %s", mcu_name);
#else
	snprintf(header,   sizeof(header),   "%s@%s", board_info.username, board_info.hostname);
	snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %uh %um %us", dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
	snprintf(mcu_line, sizeof(mcu_line), "MCU:     %s", board_info.mcu);
#endif

    snprintf(separator,   sizeof(separator),   "----------------");
    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
    snprintf(build_line,  sizeof(build_line),  "Build:   %s", board_info.build_date);
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
} // sysinfo_freertos_common.c