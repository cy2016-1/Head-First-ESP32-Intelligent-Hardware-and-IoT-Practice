#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "driver/gpio_filter.h"

 void  handle_button_event(void *args)
{
    gpio_set_level(GPIO_NUM_1, !gpio_get_level(GPIO_NUM_1));
}

void led_init()
{
    gpio_config_t io_config;
    io_config.mode = GPIO_MODE_INPUT_OUTPUT;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.pin_bit_mask = 1ULL<<1;
    gpio_config(&io_config);
}


void app_main(void)
{
    printf("start app!");
    led_init();

    gpio_config_t io_config;
    io_config.mode = GPIO_MODE_DEF_INPUT;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_ENABLE;
    io_config.pin_bit_mask = 1ULL<<21;
    gpio_config(&io_config);


    gpio_set_intr_type(GPIO_NUM_21, GPIO_INTR_NEGEDGE);

    gpio_install_isr_service(ESP_INTR_FLAG_EDGE);
    gpio_isr_handler_add(GPIO_NUM_21, handle_button_event, NULL);
    while(1)
    {
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}
