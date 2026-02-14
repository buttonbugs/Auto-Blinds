#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER
// Arduino
#include <Arduino.h>

// Libraries
#include <HTTPClient.h>
#include <map>                  // Required for handling headers elegantly

// Your Header Files
#include "secret.h"             // Your secret file

/**
 * A universal function to send HTTP requests.
 * @param method  "GET", "POST", "PATCH", etc.
 * @param url     The full endpoint URL
 * @param headers A map of key-value pairs for HTTP headers
 * @param body    The payload (usually JSON) for POST/PATCH/PUT
 */
void sendHttpRequest(String method, String url, std::map<String, String> headers, String body) {
    if (WiFi.status() != WL_CONNECTED) {
        Serial.println("WiFi not connected!");
        return;
    }

    WiFiClientSecure client;
    client.setInsecure();           // Required for HTTPS (Notion, Google, etc.)

    HTTPClient http;
    
    Serial.printf("\n--- Sending [%s] to %s ---\n", method.c_str(), url.c_str());

    if (http.begin(client, url)) {
        // 1. Loop through the map and add all headers
        for (auto const& x : headers) {
            http.addHeader(x.first, x.second);
        }

        // 2. Send the request with the body
        int httpCode = http.sendRequest(method.c_str(), body);

        // 3. Handle the response
        if (httpCode > 0) {
            Serial.printf("[HTTP] Response Code: %d\n", httpCode);
            String payload = http.getString();
            Serial.println("Response body:");
            Serial.println(payload);
        } else {
            Serial.printf("[HTTP] Failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    }
}



#endif