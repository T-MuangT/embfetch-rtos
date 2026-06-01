#include "embfetch_rtos_platform.h"

#if defined(EMBFETCH_KEIL_RTX4) || defined(EMBFETCH_KEIL_RTX5)

#include "rtos_sysinfo.h"
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#if !defined(SYSINFO_NO_MALLINFO) && !defined(__MICROLIB)
#  include <stdlib.h>
#endif

// Extern — implemented by platform-specific file (sysinfo_keilrtx4.c or sysinfo_keilrtx5.c)
extern const sysinfo_static_t board_info;
extern void sysinfo_fetch(sysinfo_dynamic_t *dst);
extern const char *logo[]; // TODO: Refactor this to call header instead

// Common SYSINFO_* defaults — override in board.h or build system
#ifndef SYSINFO_SRAM_KB
#  define SYSINFO_SRAM_KB  0u
#endif

#ifndef SYSINFO_FLASH_KB
#  define SYSINFO_FLASH_KB 0u
#endif

// Byte formatting
void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024u * 1024u) {
        snprintf(dst, len, "%uMB", (unsigned)(bytes >> 20));
    } else if (bytes >= 1024u) {
        snprintf(dst, len, "%uKB", (unsigned)(bytes >> 10));
    } else {
        snprintf(dst, len, "%uB",  (unsigned)bytes);
    }
}

// Hardware info — sizes from SYSINFO_SRAM_KB / SYSINFO_FLASH_KB
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst) {
    if (SYSINFO_SRAM_KB > 0u) {
        format_size(dst->ram, sizeof(dst->ram), (size_t)SYSINFO_SRAM_KB * 1024u);
    } else {
        snprintf(dst->ram, sizeof(dst->ram), "Unknown");
    }

    if (SYSINFO_FLASH_KB > 0u) {
        format_size(dst->flash, sizeof(dst->flash), (size_t)SYSINFO_FLASH_KB * 1024u);
    } else {
        snprintf(dst->flash, sizeof(dst->flash), "Unknown");
    }
}

// Print logo and info — static locals for C99 compliance and stack safety on RTX
void sysinfo_print(sysinfo_putline_fn putline, void *ctx) {
    static sysinfo_dynamic_t dyn;
    static sysinfo_hwinfo_t  hw;
    static char header[64],     separator[32];
    static char os_line[64],    kernel_line[64], mcu_line[64],    build_line[64];
    static char flash_line[64], ram_line[64],    uptime_line[64], heap_line[64];
    static char line[256];
    int logo_count, info_count, total, i;

    sysinfo_fetch(&dyn);
    sysinfo_hwinfo_fetch(&hw);

    snprintf(header,      sizeof(header),      "%s@%s",
             board_info.username ? board_info.username : "root",
             board_info.hostname ? board_info.hostname : "Unknown");
    snprintf(separator,   sizeof(separator),   "----------------");
    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
    snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %uh %um %us",
             dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
    snprintf(build_line,  sizeof(build_line),  "Build:   %s", board_info.build_date);
    snprintf(mcu_line,    sizeof(mcu_line),    "MCU:     %s",
             board_info.mcu ? board_info.mcu : "Unknown");
    snprintf(ram_line,    sizeof(ram_line),    "Memory:  %s", hw.ram);
    snprintf(heap_line,   sizeof(heap_line),   "Heap:    %s used / %s free",
             dyn.heap_used, dyn.heap_free);
    snprintf(flash_line,  sizeof(flash_line),  "Flash:   %s", hw.flash);

    {
        const char *info[] = {
            header,      separator,  os_line,    kernel_line,
            uptime_line, build_line, mcu_line,   ram_line,
            heap_line,   flash_line
        };

        logo_count = 0;
        while (logo[logo_count] != NULL) logo_count++;
        info_count = (int)(sizeof(info) / sizeof(info[0]));
        total      = (logo_count > info_count) ? logo_count : info_count;

        for (i = 0; i < total; i++) {
            const char *logo_part = (i < logo_count) ? logo[i] : "";
            const char *info_part = (i < info_count) ? info[i] : "";

            if (info_part[0] != '\0')
                snprintf(line, sizeof(line), "%s" LOGO_INFO_COLUMN "%s", logo_part, info_part);
            else
                snprintf(line, sizeof(line), "%s", logo_part);
            putline(ctx, line);
        }
    }
}

// UART output wrapper — shared across RTX4 and RTX5
static void (*saved_uart_puts)(const char *) = 0;

static void rtx_puts_putline(void *ctx, const char *line) {
    (void)ctx;
    if (saved_uart_puts) saved_uart_puts(line);
}

void sysinfo_print_rtx(void (*uart_puts)(const char *)) {
    saved_uart_puts = uart_puts;
    sysinfo_print(rtx_puts_putline, 0);
}

#endif // sysinfo_keilrtx_common.c