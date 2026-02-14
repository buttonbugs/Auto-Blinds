// Header Guard
#ifndef WIFI_HANDLER
#define WIFI_HANDLER

// Arduino
#include <Arduino.h>

// Libraries
#include <WiFi.h>
#include <esp_wpa2.h>       // Required for Enterprise WiFi

// Your Secret
#include "secret.h"         // Your secret file

void connect_to_wifi() {
    Serial.print("Connecting to ");
    Serial.println(SECRET_SSID);

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);

    // Using the defines from secret.h
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)SECRET_USER, strlen(SECRET_USER));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)SECRET_USER, strlen(SECRET_USER));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)SECRET_PASS, strlen(SECRET_PASS));
    esp_wifi_sta_wpa2_ent_enable();

    WiFi.begin(SECRET_SSID);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(WiFi.status());
    }

    Serial.println("\nConnected! IP: " + WiFi.localIP().toString());
    Serial.print("Time: ");
    Serial.print(millis()/1000.0);
    Serial.println("s");
}

#endif