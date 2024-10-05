//
// Created by kok on 15.09.24.
//

#ifndef SPI_APP_H
#define SPI_APP_H

#define SPI_APP_MOSI_GPIO           23
#define SPI_APP_SCLK_GPIO           18
#define SPI_APP_CS_GPIO             5

#define SPI_APP_CLK_SPEED_MHZ       5

#define SPI_APP_WRITE_MODE          0xB0

#include <stdint.h>

/**
* Initializes the SPI Application
*/
void spi_app_init();

/**
 * Sends value to the SPI device
 */
void spi_app_send_value(uint16_t value);

#endif //SPI_APP_H
