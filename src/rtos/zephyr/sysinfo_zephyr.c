#include "embfetch_rtos_platform.h"

#if defined(EMBFETCH_ZEPHYR)

#include "rtos_sysinfo.h"
#include "logo.h"
#include <zephyr/kernel.h>

#if defined(CONFIG_FLASH)
#include <zephyr/drivers/flash.h>
#include <zephyr/devicetree.h>
#endif

// Static board info fetching
const sysinfo_static_t board_info = {
    .username   = "root",
    .hostname   = CONFIG_BOARD,
    .os_name    = "Zephyr",
    .mcu        = CONFIG_SOC,
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

// Hardware info - standard Kconfig SRAM and FLASH symbols
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst) {
    // RAM
#if defined(CONFIG_SRAM_SIZE) && CONFIG_SRAM_SIZE > 0
    format_size(dst->ram, sizeof(dst->ram), CONFIG_SRAM_SIZE * 1024);
#else
    snprintf(dst->ram, sizeof(dst->ram), "Unknown");
#endif

    // Flash
#if defined(CONFIG_FLASH)
    const struct device *flash_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_flash_controller));
    if (device_is_ready(flash_dev)) {
        format_size(dst->flash, sizeof(dst->flash), CONFIG_FLASH_SIZE * 1024);
    } else {
        snprintf(dst->flash, sizeof(dst->flash), "Unknown");
    }
#else
    snprintf(dst->flash, sizeof(dst->flash), "Unknown");
#endif
}
#endif // sysinfo_zephyr.c