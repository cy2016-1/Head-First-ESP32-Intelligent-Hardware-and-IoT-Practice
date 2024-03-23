#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


#define LED_NUM GPIO_NUM_1
#define BUTTON_NUM GPIO_NUM_21

void static button_isr_handler(void *arg)
{
    gpio_set_level(LED_NUM, !gpio_get_level(LED_NUM));
}

void led_init()
{
    gpio_config_t io_config;
    io_config.mode = GPIO_MODE_INPUT_OUTPUT;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_DISABLE;
    io_config.pin_bit_mask = 1ULL<<LED_NUM;
    gpio_config(&io_config);
}

void button_init()
{
    gpio_config_t io_config;
    io_config.mode = GPIO_MODE_INPUT;
    io_config.pin_bit_mask = 1ULL<<BUTTON_NUM;
    io_config.intr_type = GPIO_INTR_NEGEDGE;
    io_config.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_config.pull_up_en = GPIO_PULLUP_ENABLE;
    gpio_config(&io_config);

    gpio_set_intr_type(BUTTON_NUM, GPIO_INTR_NEGEDGE);
    gpio_install_isr_service(ESP_INTR_FLAG_EDGE);

    gpio_isr_handler_add(BUTTON_NUM, button_isr_handler, NULL);

}

void app_main(void)
{
    printf("start app!");
    led_init();
    button_init();
    while(1)
    {
        vTaskDelay(500/portTICK_PERIOD_MS);
    }
}
