#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "esp_event.h"
#include "esp_wifi.h"
#include "nvs.h"
#include "nvs_flash.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#include "include/station.h"
#include "include/debug.h"

#define EXAMPLE_ESP_MAXIMUM_RETRY  3

static EventGroupHandle_t s_wifi_event_group;

#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT      BIT1

static const char *TAG = "wifi station";

static int s_retry_num = 0;
char ip_addr[16];
static void event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY) {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        } else {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
#if DEBUG_FLAG == 1
        ESP_LOGI(TAG, "got ip:%s",ip4addr_ntoa(&event->ip_info.ip));
#endif
        sprintf(ip_addr,"%s",ip4addr_ntoa(&event->ip_info.ip));
        s_retry_num = 0;
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }
}

unsigned char wifi_connect(char *ssid,char *psswd)
{
    s_wifi_event_group = xEventGroupCreate();
    tcpip_adapter_init();
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler, NULL));
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler, NULL));
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "H1",
            .password = "O1"
        },
    };
    
    strcpy((char *)&wifi_config.sta.ssid[0],ssid);
    strcpy((char *)&wifi_config.sta.password[0],psswd);
    
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) );
    ESP_ERROR_CHECK(esp_wifi_start() );
#if DEBUG_FLAG == 1
    ESP_LOGI(TAG, "wifi_init_sta finished.");
#endif
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
            pdFALSE,
            pdFALSE,
            portMAX_DELAY);
    if (bits & WIFI_CONNECTED_BIT) {
#if DEBUG_FLAG == 1
        ESP_LOGI(TAG, "connected to ap SSID:'%s' password:'%s'",
        		(char *)&wifi_config.sta.ssid[0], (char *)&wifi_config.sta.password[0]);
#endif
    } else if (bits & WIFI_FAIL_BIT) {
#if DEBUG_FLAG == 1
        ESP_LOGI(TAG, "Failed to connect to SSID:'%s', password:'%s'",
        		(char *)&wifi_config.sta.ssid[0], (char *)&wifi_config.sta.password[0]);
#endif
        ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
        ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
        vEventGroupDelete(s_wifi_event_group);
        return UNCONNECTED;
    } else {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
        ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
        ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
        vEventGroupDelete(s_wifi_event_group);
        return UNCONNECTED;
    }
    ESP_ERROR_CHECK(esp_event_handler_unregister(IP_EVENT, IP_EVENT_STA_GOT_IP, &event_handler));
    ESP_ERROR_CHECK(esp_event_handler_unregister(WIFI_EVENT, ESP_EVENT_ANY_ID, &event_handler));
    vEventGroupDelete(s_wifi_event_group);
    return CONNECTED;
}

