// Arduino
#include <Arduino.h>

// Libraries
// #include <WiFi.h>
#include <HTTPClient.h>
#include <esp_wpa2.h>           // Required for Enterprise WiFi
#include <map>                  // Required for handling headers elegantly

// Your Header Files
#include "secret.h"             // Your secret file
#include "wifi_handler.h"       // To handle wifi connection
#include "url_list.h"           // Place to store URLs
#include "request_handler.h"    // To handle HTTP requests

void setup() {
    Serial.begin(9600);
    delay(10000);                // Give serial monitor time to open
    connect_to_wifi();
}

void loop() {
    // 2. Define your Headers (Notion example)
    std::map<String, String> notionHeaders;
    notionHeaders["Authorization"] = "Bearer " + String(NOTION_TOKEN);
    notionHeaders["Notion-Version"] = "2025-09-03";
    notionHeaders["Content-Type"] = "application/json";

    // 3. Define your Body (Create a page in a database)
    // Using raw strings (R"(...)") makes JSON much easier to read in C++
    String notionBody = R"({
        "page_size": 100
    })";

    // 4. Fire the request!
    sendHttpRequest("POST", String(notion_query_data_sourse_url_p1) + DATA_SOURCE_ID + notion_query_data_sourse_url_p2, notionHeaders, notionBody);

    // Wait 1 minutes
    delay(60000);
}