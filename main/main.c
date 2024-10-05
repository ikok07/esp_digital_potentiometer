
#include <esp_log.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portmacro.h"
#include "pcnt_app/pcnt_app.h"
#include "spi_app/spi_app.h"

// static const char TAG[] = "main";

void app_main() {
    spi_app_init();
    pcnt_app_init();
}
