#ifndef REQUEST_HANDLER
#define REQUEST_HANDLER

/**
 * A universal function to send HTTP requests.
 * @param method  "GET", "POST", "PATCH", etc.
 * @param url     The full endpoint URL
 * @param headers A map of key-value pairs for HTTP headers
 * @param body    The payload (usually JSON) for POST/PATCH/PUT
 */
void sendHttpRequest(String method, String url, std::map<String, String> headers, String body, int * response_code, String * response_body);

#endif