#include <Arduino.h>        // For trigonometric functions and PI
#include "time.h"

#include "config.h"         // Get earth obliquity
#include "search.h"         // Get local longitude and latitude

void init_time() {
    configTime(0, 0, ntpServer); 
}

void get_current_time() {
    time_t now;
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
        Serial.println("Failed to obtain time");
        return;
    }
    
    // Get Unix Timestamp (UTC)
    time(&now);
    Serial.print("UTC Epoch: ");
    Serial.println(now);

    // Get Formatted UTC Time
    Serial.print("UTC Time: ");
    Serial.println(asctime(&timeinfo));
}