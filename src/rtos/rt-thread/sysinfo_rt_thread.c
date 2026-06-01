#include "embfetch_rtos_platform.h"

#if defined(EMBFETCH_RTTHREAD)

#include "rtos_sysinfo.h"
#include "logo.h"
#include <rtthread.h>
#include <rtdevice.h>
#include "rtconfig.h"
#include <stdio.h>
#include <string.h>
//#include "board.h"            // define BOARD_NAME and SOC_NAME then uncomment
//#include "kernel_version.h"   // define RT_VERSION, RT_SUBVERSION, RT_REVISION then uncomment

// Hardcoded BOARD_NAME and SOC_NAME if not defined in board.h
#ifndef BOARD_NAME
    #define BOARD_NAME "Unknown"
#endif

#ifndef SOC_NAME
    #define SOC_NAME "Unknown"
#endif

// Hardcoded kernel version
#ifndef RT_VERSION
    #define RT_VERSION 5
    #define RT_SUBVERSION 0
    #define RT_REVISION 0
#endif

// Static board info fetching
static const sysinfo_static_t board_info = {
    .username       = "root",
    .hostname       = BOARD_NAME,
    .os_name        = "RT-Thread",
    .mcu            = SOC_NAME,
    .build_date     = __DATE__ " " __TIME__,
};

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

// Hardware info fetching
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst) {
    // RAM
    rt_uint32_t total, used, max_used;
    rt_memory_info(&total, &used, &max_used);
#if defined(RT_USING_MEMHEAP) || defined(RT_USING_HEAP)
    if (total > 0)
        format_size(dst->ram, sizeof(dst->ram), total);
    else
        snprintf(dst->ram, sizeof(dst->ram), "Unknown");
#else
    snprintf(dst->ram, sizeof(dst->ram), "Unknown");
#endif

    // Flash 
    // Try to find MTD/flash device, fall back to board define
#if defined(RT_USING_MTD_NOR) || defined(RT_USING_SFUD)
    rt_device_t flash_dev = rt_device_find("norflash0");
    if (flash_dev != RT_NULL) {
        #ifdef BOARD_FLASH_SIZE_KB
            format_size(dst->flash, sizeof(dst->flash), BOARD_FLASH_SIZE_KB * 1024);
        #else
            snprintf(dst->flash, sizeof(dst->flash), "Unknown");
        #endif
    } else {
        snprintf(dst->flash, sizeof(dst->flash), "Unknown");
    }
#elif defined(BOARD_FLASH_SIZE_KB)
    format_size(dst->flash, sizeof(dst->flash), BOARD_FLASH_SIZE_KB * 1024);
#else
    snprintf(dst->flash, sizeof(dst->flash), "Unknown");
#endif
}

// Dynamic info fetching
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    // Kernel version
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "RT-Thread %ld.%ld.%ld",
             RT_VERSION, RT_SUBVERSION, RT_REVISION);
    
    // Uptime
    rt_tick_t ticks    = rt_tick_get();
    uint64_t  uptime_s = (uint64_t)ticks / RT_TICK_PER_SECOND;
    dst->uptime_h = (uint32_t)(uptime_s / 3600);
    dst->uptime_m = (uint32_t)((uptime_s % 3600) / 60);
    dst->uptime_s = (uint32_t)(uptime_s % 60);

    // Memory heap
#if defined(RT_USING_HEAP)
    rt_uint32_t total, used, max_used;
    rt_memory_info(&total, &used, &max_used);
    format_size(dst->heap_used, sizeof(dst->heap_used), used);
    format_size(dst->heap_free, sizeof(dst->heap_free), total - used);
#else
    snprintf(dst->heap_used, sizeof(dst->heap_used), "Unknown");
    snprintf(dst->heap_free, sizeof(dst->heap_free), "Unknown");
#endif
}

// Print logo and info
void sysinfo_print(sysinfo_putline_fn putline, void *ctx) {
    sysinfo_dynamic_t dyn;
    sysinfo_fetch(&dyn);
    sysinfo_hwinfo_t hw;
    sysinfo_hwinfo_fetch(&hw);

    // All data lines
    char header[64], separator[32];
    char os_line[64], kernel_line[64], mcu_line[64], build_line[64], 
         flash_line[64], ram_line[64], uptime_line[64], heap_line[64];

    snprintf(header,      sizeof(header), "%s@%s", board_info.username, board_info.hostname);
    snprintf(separator,   sizeof(separator), "----------------");
    snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %luh %lum %lus", dyn.uptime_h, dyn.uptime_m, dyn.uptime_s);
    snprintf(build_line,  sizeof(build_line),  "Build:   %s", board_info.build_date);
    snprintf(mcu_line,    sizeof(mcu_line),    "MCU:     %s", board_info.mcu);
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

// Wrapper for shell output
void rt_putline(void *ctx, const char *line) {
    rt_kprintf("%s\n", line);
}
void sysinfo_print_shell(void) {
    sysinfo_print(rt_putline, RT_NULL);
}
void embfetch(void) {
    sysinfo_print_shell();
}

MSH_CMD_EXPORT(embfetch, Print system info);

#endif //sysinfo_rt-thread.c