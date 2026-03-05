// Arduino
#include <Arduino.h>

// Libraries
#include <map>                  // Required for handling headers elegantly

// Your Header Files
#include "wifi_handler.h"       // To handle wifi connection
#include "notion_handler.h"

void setup() {
    Serial.begin(9600);
    delay(10000);                // Give serial monitor time to open
    connect_to_wifi();
    get_command();
}

void loop() {
    // Wait 1 minutes
    delay(60000);
}