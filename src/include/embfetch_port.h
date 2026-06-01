#include "embfetch_config.h"

#ifndef EMBFETCH_PORT_H
#define EMBFETCH_PORT_H

#if defined(EMBFETCH_ZEPHYR) || defined(EMBFETCH_NRF_CONNECT)
#include <zephyr/shell/shell.h>
void sysinfo_print_shell(const struct shell *sh);
#elif defined(EMBFETCH_FREERTOS)
void sysinfo_print_uart(void (*uart_puts_fn)(const char *));
#elif defined(EMBFETCH_ESP_IDF)
void sysinfo_print_espidf(void);
#elif defined(EMBFETCH_KEIL_RTX4) || defined(EMBFETCH_KEIL_RTX5)
void sysinfo_print_rtx(void (*uart_puts_fn)(const char *));
#elif defined(EMBFETCH_THREADX)
#include <tx_api.h>
void sysinfo_print_threadx(void);
#elif defined(EMBFETCH_RT_THREAD)
// Handled in sysinfo_rt_thread.c to avoid dependency on rtthread.h in this common header
#elif defined(EMBFETCH_NUTTX)
// Handled in sysinfo_nuttx.c to avoid dependency on NuttX headers in this common header
#else
#error "No platform defined"

#endif
#endif // EMBFETCH_PORT_H