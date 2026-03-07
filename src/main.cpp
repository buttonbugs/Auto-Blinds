// Arduino
#include <Arduino.h>

// Libraries
#include <map>                  // Required for handling headers elegantly

// Your Header Files
#include "config.h"
#include "wifi_handler.h"       // To handle wifi connection
#include "notion_handler.h"
#include "motor_controller.h"
#include "sun_calculation.h"

bool auto_mode = true;
float angle = 0;                    // Current angle of the blinds in degrees
float target = 0;
double sun_u = 0;
double sun_v = 0;
double sun_w = 0;

String page_id = "";                // The Notion page_id of the lastest command

void setup() {
    Serial.begin(9600);
    delay(200);                   // Give serial monitor time to open

    // Connect to WiFi
    connect_to_wifi();
    init_time();

    // Get Current status
    Serial.println("Get Current status");
    angle = download_current_status();

    // Get latest Command
    if(get_latest_command(&auto_mode, &angle, &target, &page_id)) {
        Serial.println("Get Latest Command Success");
    } else {
        Serial.println("Get Latest Command Fail");
    }

    if (auto_mode) {
        /* calculate auto_mode angle */
    }
    
    init_motor_controller(&angle, &target);

    // sun calcuation test
    delay(500);
    get_sun_position_ENU(&sun_u, &sun_v, &sun_w);
}

void loop() {
    // Get latest Command
    if(get_latest_command(&auto_mode, &angle, &target, &page_id)) {
        // Serial.println("Get Latest Command Success");
    } else {
        // Serial.println("Get Latest Command Fail");
    }

    if (auto_mode) {
        /* calculate auto_mode angle */
    }

    // Serial.println("Update Status");
    get_sun_position_ENU(&sun_u, &sun_v, &sun_w);
    upload_current_status(auto_mode, angle, target, sun_u, sun_v, sun_w);
    Serial.printf("auto_mode: %d, angle: %f, target: %f, sun_u: %.15lf, sun_v: %.15lf, sun_w: %.15lf\n", auto_mode, angle, target, sun_u, sun_v, sun_w);
    
    // Delay
    delay(request_delay_ms);
}