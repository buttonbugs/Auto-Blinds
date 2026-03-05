#include <map>                  // Required for handling headers elegantly
#include <Arduino.h>
#include <ArduinoJson.h>        // Parse JSON

#include "request_handler.h"    // To handle HTTP requests
#include "secret.h"             // Your secret file
#include "url_list.h"           // Place to store URLs

/* Http Request Templates */
void fetch_notion_database(String data_source_id, int *response_code, String *response_body) {
    // 2. Define your Headers (Notion example)
    std::map<String, String> notionHeaders;
    notionHeaders["Authorization"] = "Bearer " + String(NOTION_TOKEN);
    notionHeaders["Notion-Version"] = "2025-09-03";
    notionHeaders["Content-Type"] = "application/json";

    // 3. Define your Body (Create a page in a database)
    // Using raw strings (R"(...)") makes JSON much easier to read in C++
    String notionBody = R"({
        "page_size": 100,
        "sorts": [
            {
                "timestamp": "last_edited_time",
                "direction": "descending"
            }
        ]
    })";

    // 4. Fire the request!
    sendHttpRequest("POST", String(notion_query_data_sourse_url_p1) + data_source_id + notion_query_data_sourse_url_p2, notionHeaders, notionBody, response_code, response_body);
}

/* Http Data Processing */
void get_command() {
    int http_code;
    String http_body;
    fetch_notion_database(DATA_SOURCE_ID, &http_code, &http_body);
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, http_body.c_str());

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return;
    }

    JsonArray results = doc["results"].as<JsonArray>();
    for (JsonObject obj : results) {
        // Access members inside each object
        const char* id = obj["id"];
        const char* val = obj["created_time"];
        const char* edt = obj["last_edited_time"];

        Serial.printf("ID: %s, create: %s, edit: %s\n", id, val, edt);
    }
}

void download_current_status() {
    //
}

void upload_current_status() {
    //
}