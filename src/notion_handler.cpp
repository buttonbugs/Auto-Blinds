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

void retrieve_notion_block(String block_id, int *response_code, String *response_body) {
    // 2. Define your Headers (Notion example)
    std::map<String, String> notionHeaders;
    notionHeaders["Authorization"] = "Bearer " + String(NOTION_TOKEN);
    notionHeaders["Notion-Version"] = "2025-09-03";
    notionHeaders["Content-Type"] = "application/json";

    // 3. Define your Body (Create a page in a database)
    // Using raw strings (R"(...)") makes JSON much easier to read in C++
    String notionBody = R"({})";

    // 4. Fire the request!
    sendHttpRequest("GET", String(notion_block_url) + block_id, notionHeaders, notionBody, response_code, response_body);
}

void update_notion_embed_block(String block_id, String url, int *response_code, String *response_body) {
    // 2. Define your Headers (Notion example)
    std::map<String, String> notionHeaders;
    notionHeaders["Authorization"] = "Bearer " + String(NOTION_TOKEN);
    notionHeaders["Notion-Version"] = "2025-09-03";
    notionHeaders["Content-Type"] = "application/json";

    // 3. Define your Body (Create a page in a database)
    // Using raw strings (R"(...)") makes JSON much easier to read in C++
    String notionBody = R"({
        "embed": {
                "url": ")";
    notionBody += url;
    notionBody += R"("
        }
    })";
    // Serial.print("Notion Body:");
    // Serial.println(notionBody);

    // 4. Fire the request!
    sendHttpRequest("PATCH", String(notion_block_url) + block_id, notionHeaders, notionBody, response_code, response_body);
}

/* Http Data Processing */
// Get lastest command from Notion database.
// Return:
// - true: success
// - false: failed or no command
bool get_latest_command(bool * auto_mode, float * angle, float * target, String * page_id) {
    int http_code;
    String http_body;
    fetch_notion_database(DATA_SOURCE_ID, &http_code, &http_body);

    if (http_code == 0) {
        return false;
    }

    // Serial.println(http_body);
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, http_body.c_str());

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return false;
    }
    
    bool auto_mode_tmp = true;
    JsonArray results = doc["results"].as<JsonArray>();
    for (JsonObject obj : results) {
        // Access members inside each object
        const char* command_status = obj["properties"]["Status"]["status"]["name"];
        const char* command_page_id = obj["id"];
        if (String(command_status) == "Ignore") {
            // Serial.println("Status: Ignore");
        } else {
            const char* command_description = obj["properties"]["Description"]["title"][0]["plain_text"];
            if (String(command_description) == "Auto Mode") {
                *auto_mode = true;
            } else {
                *auto_mode = false;
                *target = obj["properties"]["Target"]["number"];
            }
            *page_id = command_page_id;
            // Serial.println(command_description);
            return true;
        }
    }
    *auto_mode = true;
    return true;
}

// Get current status from the embed block URL
float download_current_status() {
    int http_code;
    String http_body;
    retrieve_notion_block(NOTION_PREVIEW_BLOCK_ID, &http_code, &http_body);

    if (http_code == 0) {
        Serial.print("download_current_status() http failed");
        return 0;
    }

    // Serial.println(http_body);
    /* Parse JSON data */
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, http_body.c_str());

    if (error) {
        Serial.print("deserializeJson() failed: ");
        Serial.println(error.f_str());
        return 0;
    }

    const char* block_type = doc["type"];
    Serial.print(block_type);
    if (String(block_type) != "embed") {
        Serial.println("The preview block is not an 'embed'.");
        return 0;
    }
    
    String url = doc[block_type]["url"];
    // Serial.println(url);
    
    /* Parse search parameter from URL */
    // 1. Find where parameters start (location.search in JavaScript)
    int queryIndex = url.indexOf('?');
    if (queryIndex == -1) return 0; // No parameters found
    Serial.print("download_current_status() No parameters found");

    // 2. Extract only the query part
    String queryString = url.substring(queryIndex + 1);

    // 3. Remove everything from '#' onwards (location.hash in JavaScript)
    int hashIndex = queryString.indexOf('#');
    if (hashIndex != -1) {
        queryString = queryString.substring(0, hashIndex);
    }

    // 4. Loop through pairs separated by '&'
    int start = 0;
    while (start < queryString.length()) {
        int end = queryString.indexOf('&', start);
        if (end == -1) end = queryString.length();

        String pair = queryString.substring(start, end);
        int sep = pair.indexOf('=');

        if (sep != -1) {
            String key = pair.substring(0, sep);
            if (key == "angle") {
                return pair.substring(sep + 1).toFloat();
            }
        }
        start = end + 1;
    }
    Serial.print("download_current_status() 'angle' not found in parameters");
    return 0;
}

void upload_current_status(bool auto_mode, float angle, float target, double sun_u, double sun_v, double sun_w) {
    String url = github_preview_p1;
    url += auto_mode ? "true" : "false";
    url += github_preview_p2 + String(angle, 6);
    url += github_preview_p3 + String(target, 6);
    url += github_preview_p4 + String(sun_u, 16);
    url += github_preview_p5 + String(sun_v, 16);
    url += github_preview_p6 + String(sun_w, 16);

    int http_code;
    String http_body;
    update_notion_embed_block(NOTION_PREVIEW_BLOCK_ID, url, &http_code, &http_body);

    if (http_code == 0) {
        Serial.println("update_notion_embed_block() http failed");
    }
    // Serial.println(http_body);
}

void update_command_status(){
    //
}