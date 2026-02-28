#include "wifi_handler.h"
#include "freertos/FreeRTOS.h"

void app_main() {
    vTaskDelay(5000 / portTICK_PERIOD_MS);
    connect_to_wifi();
    while (1) {
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }
    
}