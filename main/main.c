
#include "spi_app/spi_app.h"
#include "pcnt_app/pcnt_app.h"

// static const char TAG[] = "main";

void app_main() {
    spi_app_init();
    pcnt_app_init();
}
