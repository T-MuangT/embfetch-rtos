#include "embfetch_config.h"

#if defined(EMBFETCH_ESP_IDF)

#include "rtos_sysinfo.h"
#include "freertos/FreeRTOS.h"
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include "esp_heap_caps.h"
#include "esp_timer.h"
#include "esp_idf_version.h"
#include "logo.h"

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
#include "esp_flash.h"
#else
#include "esp_spi_flash.h"  // Legacy header for IDF < 4.0
#endif

// Forward declaration of format_size from common
void format_size(char *dst, size_t len, size_t bytes);

// board_info is non-const — mcu and hostname are set at runtime by chip_info_init()
// This eliminates the need for separate mcu_name/hostname globals and
// removes all ESP_PLATFORM ifdefs from sysinfo_freertos_common.c
sysinfo_static_t board_info = {
    .username   = "root",
    .os_name    = "ESP-IDF",
    .build_date = __DATE__ " " __TIME__,
    .mcu        = NULL,   // set by chip_info_init()
    .hostname   = NULL,   // set by chip_info_init()
};

// Resolve chip identity at runtime and store directly into board_info
static void chip_info_init(void) {
    esp_chip_info_t chip;
    esp_chip_info(&chip);

    switch (chip.model) {
        case CHIP_ESP32:    board_info.mcu = "ESP32";     board_info.hostname = "esp32";    break;
        case CHIP_ESP32S2:  board_info.mcu = "ESP32-S2";  board_info.hostname = "esp32s2";  break;
        case CHIP_ESP32S3:  board_info.mcu = "ESP32-S3";  board_info.hostname = "esp32s3";  break;
        case CHIP_ESP32C2:  board_info.mcu = "ESP32-C2";  board_info.hostname = "esp32c2";  break;
        case CHIP_ESP32C3:  board_info.mcu = "ESP32-C3";  board_info.hostname = "esp32c3";  break;
        case CHIP_ESP32C5:  board_info.mcu = "ESP32-C5";  board_info.hostname = "esp32c5";  break;
        case CHIP_ESP32C6:  board_info.mcu = "ESP32-C6";  board_info.hostname = "esp32c6";  break;
        case CHIP_ESP32C61: board_info.mcu = "ESP32-C61"; board_info.hostname = "esp32c61"; break;
        case CHIP_ESP32H2:  board_info.mcu = "ESP32-H2";  board_info.hostname = "esp32h2";  break;
        case CHIP_ESP32H21: board_info.mcu = "ESP32-H21"; board_info.hostname = "esp32h21"; break;
        case CHIP_ESP32H4:  board_info.mcu = "ESP32-H4";  board_info.hostname = "esp32h4";  break;
        case CHIP_ESP32P4:  board_info.mcu = "ESP32-P4";  board_info.hostname = "esp32p4";  break;
        default:            board_info.mcu = "Unknown";   board_info.hostname = "unknown";  break;
    }
}

// Hardware info — flash and RAM via ESP-IDF APIs
void sysinfo_hwinfo_fetch(sysinfo_hwinfo_t *dst) {
    size_t ram = heap_caps_get_total_size(MALLOC_CAP_INTERNAL);
    if (ram > 0)
        format_size(dst->ram, sizeof(dst->ram), ram);
    else
        snprintf(dst->ram, sizeof(dst->ram), "Unknown");

#if ESP_IDF_VERSION >= ESP_IDF_VERSION_VAL(4, 0, 0)
    uint32_t flash_size = 0;
    if (esp_flash_get_size(NULL, &flash_size) != ESP_OK || flash_size == 0)
        snprintf(dst->flash, sizeof(dst->flash), "Unknown");
    else
        format_size(dst->flash, sizeof(dst->flash), flash_size);
#else
    uint32_t flash_size = spi_flash_get_chip_size();
    if (flash_size > 0)
        format_size(dst->flash, sizeof(dst->flash), flash_size);
    else
        snprintf(dst->flash, sizeof(dst->flash), "Unknown");
#endif
}

// Dynamic info — uptime via esp_timer, heap via heap_caps
void sysinfo_fetch(sysinfo_dynamic_t *dst) {
    snprintf(dst->kernel_version, sizeof(dst->kernel_version),
             "FreeRTOS %d.%d.%d",
             tskKERNEL_VERSION_MAJOR,
             tskKERNEL_VERSION_MINOR,
             tskKERNEL_VERSION_BUILD);

    // esp_timer does not wrap back unlike xTaskGetTickCount()
    int64_t  uptime_us = esp_timer_get_time();
    uint64_t uptime_s  = (uint64_t)(uptime_us / 1000000);
    dst->uptime_h = (uint32_t)(uptime_s / 3600);
    dst->uptime_m = (uint32_t)((uptime_s % 3600) / 60);
    dst->uptime_s = (uint32_t)(uptime_s % 60);

    size_t heap_free  = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
    size_t heap_total = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
    size_t heap_used  = heap_total - heap_free;

    format_size(dst->heap_used, sizeof(dst->heap_used), heap_used);
    format_size(dst->heap_free, sizeof(dst->heap_free), heap_free);

    // username not used on ESP-IDF
    dst->username[0] = '\0';
}

// UART output wrapper — ESP-IDF uses printf directly
static void uart_putline(void *ctx, const char *line) {
    (void)ctx;
    printf("%s\r\n", line);
}

void sysinfo_print_espidf(void) {
    chip_info_init();
    sysinfo_print(uart_putline, NULL);
}
#endif // sysinfo_esp-idf.c