//
// Created by kok on 15.09.24.
//

#ifndef PCNT_APP_H
#define PCNT_APP_H

#define PCNT_APP_EDGE_GPIO         32
#define PCNT_APP_LEVEL_GPIO        33

#define PCNT_APP_MIN_VALUE         -1
#define PCNT_APP_MAX_VALUE         1024

#define PCNT_APP_INTR_PRIORITY     0 // driver uses default priority
#include <esp_err.h>

/**
 * Initializes the PCNT application
 */
void pcnt_app_init();

/*
 * Gets the current PCNT value
 */
esp_err_t pcnt_app_get_value(int *value);

#endif //PCNT_APP_H
