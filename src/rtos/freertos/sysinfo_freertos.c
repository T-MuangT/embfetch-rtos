#include "embfetch_config.h"

#if defined (EMBFETCH_FREERTOS)

#include "rtos_sysinfo.h"
#include "logo.h"
#include <stdio.h>
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"
#include "uart.h"
//#include "board.h"    // define BOARD_NAME and MCU_NAME in your board header then uncomment

// Forward declaration of format_size from common
void format_size(char *dst, size_t len, size_t bytes);

// Static board info — hostname and mcu default to "Unknown" if not defined by BSP
sysinfo_static_t board_info = {
    .username   = "root",
    .os_name    = "FreeRTOS",
    .build_date = __DATE__ " " __TIME__,
#if defined(BOARD_NAME) && defined(MCU_NAME)
    .hostname   = BOARD_NAME,
    .mcu        = MCU_NAME,
#else
    .hostname   = "Unknown",
    .mcu        = "Unknown",
#endif
};

// Hardware info — sizes from FreeRTOSConfig.h or BSP defines
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst) {
#if defined(SYSINFO_RAM_SIZE)
    format_size(dst->ram, sizeof(dst->ram), SYSINFO_RAM_SIZE);
#else
    snprintf(dst->ram, sizeof(dst->ram), "Unknown");
#endif

#if defined(SYSINFO_FLASH_SIZE)
    format_size(dst->flash, sizeof(dst->flash), SYSINFO_FLASH_SIZE);
#else
    snprintf(dst->flash, sizeof(dst->flash), "Unknown");
#endif
}

// Dynamic info — uptime via tick count, heap via FreeRTOS native API
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "FreeRTOS %d.%d.%d",
             tskKERNEL_VERSION_MAJOR,
             tskKERNEL_VERSION_MINOR,
             tskKERNEL_VERSION_BUILD);

    // NOTE: TickType_t is uint32_t by default — wraps after ~49 days at 1000Hz.
    // For long-running uptime, maintain a separate overflow counter in a timer callback.
    TickType_t ticks    = xTaskGetTickCount();
    uint64_t   uptime_s = (uint64_t)ticks / configTICK_RATE_HZ;
    dst->uptime_h = (uint32_t)(uptime_s / 3600);
    dst->uptime_m = (uint32_t)((uptime_s % 3600) / 60);
    dst->uptime_s = (uint32_t)(uptime_s % 60);

    // FreeRTOS only exposes free heap natively; used = total - free.
    size_t heap_free  = xPortGetFreeHeapSize();
    size_t heap_total = configTOTAL_HEAP_SIZE;
    size_t heap_used  = (heap_total > heap_free) ? (heap_total - heap_free) : 0;

    format_size(dst->heap_used, sizeof(dst->heap_used), heap_used);
    format_size(dst->heap_free, sizeof(dst->heap_free), heap_free);

    // username not used on FreeRTOS
    dst->username[0] = '\0';
}

// UART output wrapper
static void uart_putline(void *ctx, const char *line) {
    (void)ctx;
    uart_puts(line);
    uart_puts("\r\n");
}

void sysinfo_print_uart(void) {
    sysinfo_print(uart_putline, NULL);
}
#endif // sysinfo_freertos.c