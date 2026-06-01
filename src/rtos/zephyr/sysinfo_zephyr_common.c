#include "embfetch_rtos_platform.h"

#if defined(EMBFETCH_ZEPHYR) || defined(EMBFETCH_NRF_CONNECT)

#include "rtos_sysinfo.h"
#include <zephyr/kernel.h>
#include <zephyr/sys/util.h>
#include <zephyr/sys/mem_stats.h>
#include <zephyr/sys/sys_heap.h>
#include <zephyr/shell/shell.h>
#include <inttypes.h>

// Extern — implemented by platform-specific file (sysinfo_zephyr.c or sysinfo_nrf-connect.c)
// board_info is const — Zephyr subfamily board info is always compile-time constant
extern const sysinfo_static_t board_info;
extern void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst);

// Byte formatting
static void format_size(char *dst, size_t len, size_t bytes) {
    if (bytes >= 1024 * 1024) {
        snprintf(dst, len, "%uMB", (unsigned int)(bytes >> 20));
    } else if (bytes >= 1024) {
        snprintf(dst, len, "%uKB", (unsigned int)(bytes >> 10));
    } else {
        snprintf(dst, len, "%uB", (unsigned int)bytes);
    }
}

// Dynamic info — kernel version, uptime, heap
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    uint32_t ver = sys_kernel_version_get();
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "Zephyr %d.%d.%d",
             SYS_KERNEL_VER_MAJOR(ver),
             SYS_KERNEL_VER_MINOR(ver),
             SYS_KERNEL_VER_PATCHLEVEL(ver));

    int64_t uptime_ms = k_uptime_get();
    int64_t uptime_s  = uptime_ms / 1000;
    dst->uptime_h = (uint32_t)(uptime_s / 3600);
    dst->uptime_m = (uint32_t)((uptime_s % 3600) / 60);
    dst->uptime_s = (uint32_t)(uptime_s % 60);

#if defined(CONFIG_SYS_HEAP_RUNTIME_STATS)
    struct sys_memory_stats stats;
    extern struct sys_heap _system_heap;
    sys_heap_runtime_stats_get(&_system_heap, &stats);
    format_size(dst->heap_used, sizeof(dst->heap_used), stats.allocated_bytes);
    format_size(dst->heap_free, sizeof(dst->heap_free), stats.free_bytes);
#else
    snprintf(dst->heap_used, sizeof(dst->heap_used), "Unknown");
    snprintf(dst->heap_free, sizeof(dst->heap_free), "Unknown");
#endif

    // username not used on Zephyr
    dst->username[0] = '\0';
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

// Shell output wrapper
static void zephyr_shell_putline(void *ctx, const char *line) {
    shell_print((const struct shell *)ctx, "%s", line);
}

void sysinfo_print_shell(const struct shell *sh) {
    sysinfo_print(zephyr_shell_putline, (void *)sh);
}

#endif // sysinfo_zephyr_common.c