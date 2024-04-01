#include <stdio.h>
#include "esp_wifi.h"
#include "freertos/FreeRTOS.h"
#include "nvs_flash.h"
#include "main.h"
#include "esp_log.h"



void test_wifi_event_cb(void* event_handler_arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    printf("event %s, id = %ld \n\n", event_base, event_id);
    //打印backtrace
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect();
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
        printf("hehe got ip:" IPSTR "\n" , IP2STR(&event->ip_info.ip));
        ESP_LOGI("", "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}





void app_main(void)
{

    nvs_flash_init();
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();
    

    esp_event_handler_instance_t wifi_event_handler_instance;
    esp_event_handler_instance_t get_ip_event_handler_instance;
    esp_event_handler_instance_register(WIFI_EVENT,
                                              ESP_EVENT_ANY_ID,
                                              test_wifi_event_cb,
                                              NULL,
                                              &wifi_event_handler_instance);

    esp_event_handler_instance_register(IP_EVENT,
                                              IP_EVENT_STA_GOT_IP,
                                              test_wifi_event_cb,
                                              NULL,
                                              &get_ip_event_handler_instance);


                             

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "Hello",
            .password = "13632796092"
        },
    };
    esp_wifi_set_config(WIFI_IF_STA, &wifi_config);
    esp_wifi_start();

    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);
       // printf("Hello, world!\n");
    }


}
