#include "rtos_platform.h"

#if defined(EMBFETCH_KEIL_RTX4)

#include "rtos_sysinfo.h"
#include "cmsis_os.h"
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "logo.h"
//#include "board.h"    // define SYSINFO_* in your board header then uncomment

// RTX4-specific defaults
#ifndef SYSINFO_OS_NAME
#  define SYSINFO_OS_NAME    "Keil RTX4"
#endif

#ifndef SYSINFO_HOSTNAME
#  define SYSINFO_HOSTNAME   "rtx4"
#endif

#ifndef SYSINFO_MCU
#  define SYSINFO_MCU        "Unknown"
#endif

#ifndef SYSINFO_KERNEL_VER
#  define SYSINFO_KERNEL_VER "4.0.0"
#endif

// Forward declaration of format_size from common
void format_size(char *dst, size_t len, size_t bytes);

// Static board info
const sysinfo_static_t board_info = {
    .username   = "root",
    .hostname   = SYSINFO_HOSTNAME,
    .os_name    = SYSINFO_OS_NAME,
    .mcu        = SYSINFO_MCU,
    .build_date = __DATE__ " " __TIME__,
};

// Dynamic info — RTX4 uses cmsis_os.h API
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    uint32_t ticks, tick_hz, uptime_s;

    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "RTX4 %s", SYSINFO_KERNEL_VER);

    ticks    = osKernelSysTick();
    tick_hz  = osKernelSysTickFrequency;
    if (tick_hz == 0u) tick_hz = 1u;  // Guard divide-by-zero
    uptime_s      = ticks / tick_hz;
    dst->uptime_h = uptime_s / 3600u;
    dst->uptime_m = (uptime_s % 3600u) / 60u;
    dst->uptime_s =  uptime_s % 60u;

#if defined(SYSINFO_HEAP_USED) && defined(SYSINFO_HEAP_FREE)
    format_size(dst->heap_used, sizeof(dst->heap_used), SYSINFO_HEAP_USED);
    format_size(dst->heap_free, sizeof(dst->heap_free), SYSINFO_HEAP_FREE);
#else
    snprintf(dst->heap_used, sizeof(dst->heap_used), "Unknown");
    snprintf(dst->heap_free, sizeof(dst->heap_free), "Unknown");
#endif

    dst->username[0] = '\0';
}

#endif // sysinfo_keilrtx4.c