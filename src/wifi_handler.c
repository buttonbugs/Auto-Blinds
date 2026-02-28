// Global library
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_eap_client.h"

// Custom library
#include "config.h"
#include "secret.h"

static const char *TAG = "WiFi";
static esp_netif_t *sta_netif = NULL;
static esp_netif_t *ap_netif = NULL;

#ifdef EABLE_ROUTER
#include "lwip/lwip_napt.h"
#include "lwip/dns.h"

void enable_nat() {
    if (ap_netif == NULL) {
        ESP_LOGE("NAT", "AP netif NULL");
        return;
    }

    esp_netif_ip_info_t ip_info;
    esp_netif_get_ip_info(ap_netif, &ip_info);

    if (ip_info.ip.addr == 0) {
        ESP_LOGE("NAT", "AP IP not ready yet");
        return;
    }

    u32_t ap_ip = ip_info.ip.addr;

    ip_napt_enable(ap_ip, 1);

    ESP_LOGI("NAT", "NAT enabled on IP: " IPSTR, IP2STR(&ip_info.ip));
}

void enable_dns_forwarding() {
    // Enable DNS Forwarding
    dns_setserver(0, dns_getserver(0));
    dns_setserver(1, dns_getserver(1));

    ESP_LOGI("DNS", "DNS Forwarded");
}

#endif

void create_netifs() {
    // Create Netifs Before WiFi Init
    sta_netif = esp_netif_create_default_wifi_sta();
    
    #ifdef EABLE_ROUTER
        ap_netif = esp_netif_create_default_wifi_ap();
    #endif
}

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {

            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "WiFi started, connecting...");
                esp_wifi_connect();
                break;

            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "Connected to AP");
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                ESP_LOGI(TAG, "Disconnected. Reconnecting...");
                esp_wifi_connect();
                break;
        }
    }

    if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));

        #ifdef EABLE_ROUTER
            enable_nat();
            // enable_dns_forwarding();
        #endif
    }
}

void wifi_enterprise_init(void) {

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);

    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL);

    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL);

    wifi_config_t wifi_config = {0};
    strcpy((char*)wifi_config.sta.ssid, WIFI_SSID);

    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);

    // ===== New Enterprise API =====
    esp_eap_client_set_identity((uint8_t *)WIFI_USER, strlen(WIFI_USER));
    esp_eap_client_set_username((uint8_t *)WIFI_USER, strlen(WIFI_USER));
    esp_eap_client_set_password((uint8_t *)WIFI_PASS, strlen(WIFI_PASS));

    esp_wifi_sta_enterprise_enable();

    esp_wifi_start();
}


void start_softap(void) {
    #ifdef EABLE_ROUTER

        wifi_config_t ap_config = {
            .ap = {
                .ssid = AP_SSID,
                .password = AP_PASS,
                .ssid_len = 0,
                .channel = 1,
                .max_connection = 4,
                .authmode = WIFI_AUTH_WPA2_PSK
            },
        };

        if (strlen(AP_PASS) == 0) {
            ap_config.ap.authmode = WIFI_AUTH_OPEN;
        }

        esp_wifi_set_mode(WIFI_MODE_APSTA);
        esp_wifi_set_config(WIFI_IF_AP, &ap_config);

        ESP_LOGI("AP", "SoftAP started. SSID: %s", AP_SSID);
    #endif
}

void connect_to_wifi() {
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    
    create_netifs();        //Create Netifs Before WiFi Init
    wifi_enterprise_init();
    start_softap();
}