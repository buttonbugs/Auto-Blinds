// Arduino
#include <Arduino.h>

// Libraries
#include <map>                  // Required for handling headers elegantly

// Your Header Files
#include "wifi_handler.h"       // To handle wifi connection
#include "notion_handler.h"

bool auto_mode = true;
float angle = 0;
int target = 0;
double sun_u = 0;
double sun_v = 0;
double sun_w = 0;

String page_id = "";                // The Notion page_id of the lastest command

void setup() {
    Serial.begin(9600);
    delay(10000);                   // Give serial monitor time to open
    connect_to_wifi();
    if(get_latest_command(&auto_mode, &target, &page_id)) {
        Serial.println("Get Latest Command Success");
    } else {
        Serial.println("Get Latest Command Fail");
    }
    // Get Current status
    Serial.println("Get Current status");
    Serial.println(download_current_status());
    Serial.println("Update Status");
    auto_mode = false;
    upload_current_status(auto_mode, angle, target, sun_u, sun_v, sun_w);
}

void loop() {
    // Wait 1 minutes
    delay(60000);
}