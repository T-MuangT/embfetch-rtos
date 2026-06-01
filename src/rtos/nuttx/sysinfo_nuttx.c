#include "rtos_platform.h"

#if defined(EMBFETCH_NUTTX)

#include "rtos_sysinfo.h"
#include <nuttx/config.h>
#include <nuttx/arch.h>
#include <sys/boardctl.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <time.h>
#include "logo.h"

// Static board info fetching
static const sysinfo_static_t board_info = {
    .hostname   = CONFIG_ARCH_BOARD,
    .os_name    = "NuttX",
    .mcu        = CONFIG_ARCH_CHIP,
    .build_date = __DATE__ " " __TIME__,
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
#if defined(CONFIG_RAM_SIZE) && CONFIG_RAM_SIZE > 0
    format_size(dst->ram, sizeof(dst->ram), CONFIG_RAM_SIZE);
#else
    snprintf(dst->ram, sizeof(dst->ram), "Unknown");
#endif

    // Flash
#if defined(CONFIG_ARCH_HAVE_FLASH)
    format_size(dst->flash, sizeof(dst->flash), CONFIG_FLASH_SIZE);
#else
    snprintf(dst->flash, sizeof(dst->flash), "Unknown");
#endif
}

// Dynamic info fetching
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    // Kernel version
    struct utsname uts;
    uname(&uts);
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "NuttX %s", uts.release);

    // Uptime
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    uint32_t uptime_s = (uint32_t)ts.tv_sec;
    dst->uptime_h = uptime_s / 3600;
    dst->uptime_m = (uptime_s % 3600) / 60;
    dst->uptime_s = uptime_s % 60;

    // Memory heap
    struct mallinfo mi = mallinfo();
    format_size(dst->heap_used, sizeof(dst->heap_used), (size_t)mi.uordblks);
    format_size(dst->heap_free, sizeof(dst->heap_free), (size_t)mi.fordblks);

    // Runtime username
#if defined(CONFIG_LIBC_GETLOGIN)
    const char *login = getlogin();
#else
    const char *login = getenv("USER");
#endif
    snprintf(dst->username, sizeof(dst->username), "%s", login ? login : "root");
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

    snprintf(header,      sizeof(header), "%s@%s", dyn.username, board_info.hostname);
    snprintf(separator,   sizeof(separator), "----------------");

    snprintf(os_line,     sizeof(os_line),     "OS:      %s", board_info.os_name);
    snprintf(kernel_line, sizeof(kernel_line), "Kernel:  %s", dyn.kernel_version);
    snprintf(uptime_line, sizeof(uptime_line), "Uptime:  %uh %um %us", (unsigned)dyn.uptime_h, (unsigned)dyn.uptime_m, (unsigned)dyn.uptime_s);
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
static void nuttx_putline(void *ctx, const char *line) {
    (void)ctx;
    printf("%s\n", line);
}
void sysinfo_print_nuttx(void) {
    sysinfo_print(nuttx_putline, NULL);
}
#endif //sysinfo_nuttx.c