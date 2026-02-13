#include <Arduino.h>

// define led according to pin diagram in article
const int led = D10; // there is no LED_BUILTIN available for the XIAO ESP32C3.

void setup() {
    // initialize digital pin led as an output
    Serial.begin(9600);
}

void loop() {
    delay(1000);               // wait for a second
    Serial.println("Serial Test");
}