// Arduino
#include <Arduino.h>

// Libraries
#include <map>                  // Required for handling headers elegantly

// Your Header Files
#include "wifi_handler.h"       // To handle wifi connection
#include "notion_handler.h"

bool auto_mode = true;
int target = 0;
String page_id = "";

void setup() {
    Serial.begin(9600);
    delay(10000);                // Give serial monitor time to open
    connect_to_wifi();
    unsigned long t = millis();
    if(get_latest_command(&auto_mode, &target, &page_id)) {
        Serial.println("Get Latest Command Success");
    } else {
        Serial.println("Get Latest Command Fail");
    }
    Serial.print("time: ");
    Serial.println(millis() - t);
    Serial.println(auto_mode);
    Serial.println(target);
    Serial.println(page_id);
    // Get Current status
    Serial.println(download_current_status());
}

void loop() {
    // Wait 1 minutes
    delay(60000);
}