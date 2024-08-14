#include <stdio.h>
#include "driver/ledc.h"
#include "esp_err.h"

#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE

typedef struct init_ch_led_rgb
{
    /*initialiced chanels*/
    ledc_channel_t LEDC_CHANNEL;
    ledc_intr_type_t LEDC_INTR_DISABLE;
    uint8_t LEDC_OUTPUT_IO;
    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
};

typedef struct init_duty_led_rgb
{
    /* data */
    uint32_t LEDC_DUTY_RES;
    uint32_t LEDC_FREQUENCY;
    uint8_t LEDC_OUTPUT_IO;
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
};
