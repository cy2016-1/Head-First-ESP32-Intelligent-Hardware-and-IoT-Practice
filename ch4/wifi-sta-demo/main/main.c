#include <stdio.h>
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "main.h"
#include "esp_log.h"

void test_esp_event_cb(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    } else if  (event_base == IP_EVENT && event_id == IP_EVENT_ETH_GOT_IP)
    {
        ip_event_got_ip_t* getInfoStruct = (ip_event_got_ip_t*)event_data;
        esp_ip4_addr_t ip = getInfoStruct->ip_info.ip;
        printf("get ip success!" IPSTR "\n" , IP2STR(&ip));
    }
}

void init_wifi()
{
    //初始化
    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    //注册事件回调
    esp_event_handler_instance_t wifi_event_handler;
    esp_event_handler_instance_register(WIFI_EVENT,
                                              ESP_EVENT_ANY_ID,
                                              test_esp_event_cb,
                                              NULL,
                                              &wifi_event_handler);
                                              
    esp_event_handler_instance_t ip_event_handler;
    esp_event_handler_instance_register(IP_EVENT,
                                            IP_EVENT_ETH_GOT_IP,
                                            test_esp_event_cb,
                                            NULL,
                                            &ip_event_handler);

    //wifi的配置与启动                                 
    wifi_init_config_t config =   WIFI_INIT_CONFIG_DEFAULT();                                   
    esp_wifi_init(&config);

    esp_wifi_set_mode(WIFI_MODE_STA);
    wifi_config_t wifi_config =  {
        .sta = {
            .ssid = "Hello",
            .password = "13632796092",
        },
    };
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

}

void app_main(void)
{
    init_wifi();
    while(1)
    {
        vTaskDelay(1000/portTICK_PERIOD_MS);
    }
}
