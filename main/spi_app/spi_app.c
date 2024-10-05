//
// Created by kok on 15.09.24.
//

#include "driver/spi_common.h"

#include "tasks_common.h"
#include "spi_app.h"

#include <esp_log.h>
#include <driver/gpio.h>
#include <driver/spi_master.h>

static const char TAG[] = "spi_app";

static spi_device_handle_t spi_device_handle;

void spi_app_init() {
    // Initialize the SPI Bus
    const spi_bus_config_t bus_config = {
        .mosi_io_num = SPI_APP_MOSI_GPIO,
        .sclk_io_num = SPI_APP_SCLK_GPIO,
        .isr_cpu_id = SPI_APP_ISR_CORE_ID,
        .flags = SPICOMMON_BUSFLAG_MASTER,
        .miso_io_num = -1,
        .data0_io_num = -1,
        .data1_io_num = -1,
        .data2_io_num = -1,
        .data3_io_num = -1,
        .data4_io_num = -1,
        .data5_io_num = -1,
        .data6_io_num = -1,
        .data7_io_num = -1,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1
    };
    spi_bus_initialize(SPI2_HOST, &bus_config, SPI_DMA_DISABLED);

    // Add SPI device
    const spi_device_interface_config_t device_config = {
        .command_bits = 4,
        .address_bits = 4,
        .dummy_bits = 0,
        .mode = 0,
        .clock_source = SPI_CLK_SRC_DEFAULT,
        .clock_speed_hz = SPI_APP_CLK_SPEED_MHZ * 1000000,
        .cs_ena_posttrans = 5,
        .spics_io_num = SPI_APP_CS_GPIO,
        .queue_size = 3
    };
    spi_bus_add_device(SPI2_HOST, &device_config, &spi_device_handle);
}

void spi_app_send_value(const uint16_t value) {
    const uint8_t control = SPI_APP_WRITE_MODE;
    const uint8_t data_byte0 = value & 0xFF;
    const uint8_t data_byte1 = (value >> 8) & 0x03;

    spi_transaction_t transaction = {
        .flags = SPI_TRANS_USE_TXDATA,
        .tx_data = {control, data_byte1, data_byte0},
        .length = 24
    };

    // Select the SPI Slave
    gpio_set_level(SPI_APP_CS_GPIO, 0);
    ESP_LOGI(TAG, "Transmitting SPI control bits: %d, SPI data: %d", control, value);
    const esp_err_t err = spi_device_transmit(spi_device_handle, &transaction);
    if (err != ESP_OK)
        ESP_LOGE(TAG, "Failed to transmit data to SPI device! Error: %s", esp_err_to_name(err));

    vTaskDelay(10 / portTICK_PERIOD_MS);

    // Deselect the SPI Slave
    gpio_set_level(SPI_APP_CS_GPIO, 1);
}
