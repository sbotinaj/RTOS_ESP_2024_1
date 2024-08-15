// Utiliza a biblioteca led_rgb_lib.h
#include "led_rgb_lib.h"
// Define o pino de saída do LED RGB
#define LEDC_OUTPUT_IO (27)
// Define o canal do LED RGB
#define LEDC_CHANNEL LEDC_CHANNEL_0
// Define a resolução do duty cycle
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT
// Define o duty cycle
#define LEDC_DUTY (4096)
// Define a frequência do PWM
#define LEDC_FREQUENCY (4000)
// Define a cor vermelha
#define RED 0
// Define a cor verde
#define GREEN 1
// Define a cor azul
#define BLUE 2
// Define a cor amarela

//Implementa la funcion de inicializacion de los canales del LED RGB
void init_led_rgb(void)
{
    // Prepara y luego aplica la configuración del temporizador PWM LEDC
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .duty_resolution = LEDC_DUTY_RES,
        .timer_num = LEDC_TIMER,
        .freq_hz = LEDC_FREQUENCY,
        .clk_cfg = LEDC_AUTO_CLK};
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepara y luego aplica la configuración del canal PWM LEDC
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO,
        .duty = 0, // Establece el duty cycle al 0%
        .hpoint = 0};
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}