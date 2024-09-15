//
// Created by kok on 15.09.24.
//

#include "pcnt_app.h"

#include "freertos/FreeRTOS.h"
#include "esp_log.h"
#include "driver/pulse_cnt.h"

#include "tasks_common.h"
#include "spi_app/spi_app.h"

static const char TAG[] = "pcnt_app";
static pcnt_unit_handle_t pcnt_handle;
static pcnt_channel_handle_t pcnt_channel;

/**
 * Install PCNT channel
 */
void pcnt_app_channel_init() {
    const pcnt_chan_config_t chan_config = {
        .edge_gpio_num = PCNT_APP_EDGE_GPIO,
        .level_gpio_num = PCNT_APP_LEVEL_GPIO
    };

    ESP_ERROR_CHECK(pcnt_new_channel(pcnt_handle, &chan_config, &pcnt_channel));

    ESP_ERROR_CHECK(pcnt_channel_set_edge_action(pcnt_channel, PCNT_CHANNEL_EDGE_ACTION_DECREASE, PCNT_CHANNEL_EDGE_ACTION_INCREASE));
    ESP_ERROR_CHECK(pcnt_channel_set_level_action(pcnt_channel, PCNT_CHANNEL_LEVEL_ACTION_KEEP, PCNT_CHANNEL_LEVEL_ACTION_INVERSE));
}

void pcnt_app_task(void *pvParams) {
    while (1) {
        uint16_t value = 0;
        ESP_ERROR_CHECK(pcnt_app_get_value(&value));
        ESP_LOGI(TAG, "PCNT value: %d", value);
        spi_app_send_value(value);
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
}

void pcnt_app_init() {
    const pcnt_unit_config_t pcnt_config = {
        .low_limit = PCNT_APP_MIN_VALUE,
        .high_limit = (PCNT_APP_MAX_VALUE + 1) * 2,
        .intr_priority = PCNT_APP_INTR_PRIORITY
    };

    // Create PCNT handle
    ESP_ERROR_CHECK(pcnt_new_unit(&pcnt_config, &pcnt_handle));

    // Initialize the PCNT Channel
    pcnt_app_channel_init();

    // Enable PCNT
    ESP_ERROR_CHECK(pcnt_unit_enable(pcnt_handle));
    ESP_ERROR_CHECK(pcnt_unit_start(pcnt_handle));

    // Start PCNT task
    xTaskCreatePinnedToCore(
        &pcnt_app_task,
        "pcnt_app_task",
        PCNT_APP_TASK_STACK_SIZE,
        NULL,
        PCNT_APP_TASK_PRIORITY,
        NULL,
        PCNT_APP_TASK_CORE_ID
    );
}

esp_err_t pcnt_app_get_value(int *value) {
    int pulse_count = 0;
    const esp_err_t err = pcnt_unit_get_count(pcnt_handle, &pulse_count);
    if (err != ESP_OK) {
        ESP_LOGE(TAG, "Failed to get PCNT value! Error: %s", esp_err_to_name(err));
        return err;
    }
    *value = pulse_count / 2;
    return ESP_OK;
}
