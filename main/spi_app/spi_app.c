//
// Created by kok on 15.09.24.
//

#include "driver/spi_common.h"

#include "tasks_common.h"
#include "spi_app.h"

#include <esp_log.h>
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
        .command_bits = 0,
        .address_bits = 0,
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
    uint16_t command = (0x00 << 10) | (value & 0x03FF);

    spi_transaction_t transaction = {
        .tx_buffer = &command,
        .rx_buffer = NULL,
        .length = 16
    };
    const esp_err_t err = spi_device_transmit(spi_device_handle, &transaction);
    if (err != ESP_OK)
        ESP_LOGE(TAG, "Failed to transmit data to SPI device! Error: %s", esp_err_to_name(err));
}
