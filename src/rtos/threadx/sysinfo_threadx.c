#include "embfetch_rtos_platform.h"

#if defined(EMBFETCH_THREADX)

#include "rtos_sysinfo.h"
#include "logo.h"
#include <tx_api.h>
#include "uart.h"
//#include "board.h"      // define BOARD_NAME and MCU_NAME in your board header then uncomment

// Static board info fetching.
static const sysinfo_static_t board_info = {
    .username   = "root",
    .os_name    = "ThreadX",
    .build_date = __DATE__ " " __TIME__,
#if defined(BOARD_NAME) && defined(MCU_NAME)
    .hostname = BOARD_NAME,
    .mcu      = MCU_NAME,
#else
    .hostname = "Unknown",
    .mcu      = "Unknown",
#endif
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
#if defined(BOARD_RAM_SIZE) && BOARD_RAM_SIZE > 0
    format_size(dst->ram, sizeof(dst->ram), BOARD_RAM_SIZE);
#else
    snprintf(dst->ram, sizeof(dst->ram), "Unknown");
#endif

    // Flash
#if defined(BOARD_FLASH_SIZE) && BOARD_FLASH_SIZE > 0
    format_size(dst->flash, sizeof(dst->flash), BOARD_FLASH_SIZE);
#else
    snprintf(dst->flash, sizeof(dst->flash), "Unknown");
#endif
}

// Dynamic info fetching
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    // Uptime and kernel version
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "ThreadX %d.%d", THREADX_MAJOR_VERSION, THREADX_MINOR_VERSION);
    ULONG ticks = tx_time_get();
    ULONG freq   = TX_TIMER_TICKS_PER_SECOND;
    ULONG uptime_s = ticks / freq;
    dst->uptime_h = (uint32_t)(uptime_s / 3600);
    dst->uptime_m = (uint32_t)((uptime_s % 3600) / 60);
    dst->uptime_s = (uint32_t)(uptime_s % 60);

    // Memory heap
    ULONG available, fragments;
    extern TX_BYTE_POOL app_byte_pool;
    tx_byte_pool_info_get(&app_byte_pool,
                      TX_NULL,
                      &available,
                      &fragments,
                      TX_NULL,
                      TX_NULL,
                      TX_NULL);
    ULONG total = app_byte_pool.tx_byte_pool_size;
    format_size(dst->heap_free, sizeof(dst->heap_free), (size_t)available);
    format_size(dst->heap_used, sizeof(dst->heap_used), (size_t)(total - available));
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
    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
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
static void threadx_putline(void *ctx, const char *line) {
    uart_puts(line);
    uart_puts("\r\n");
}
void sysinfo_print_threadx(void) {
    sysinfo_print(threadx_putline, NULL);
}
#endif //sysinfo_threadx.c