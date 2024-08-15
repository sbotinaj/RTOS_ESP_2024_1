#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/uart.h"

#include <string.h>


#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (27) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY               (4096) // Set duty to 50%. (2 ** 13) * 50% = 4096
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz
// Defines UART macros
#define UART_USED UART_NUM_1 // UART number used
#define BUF_SIZE (1024) // UART buffer size
#define RX_UART (UART_PIN_NO_CHANGE) // RX pin
#define TX_UART (UART_PIN_NO_CHANGE) // TX pin

// configurate uart1

void init_uart(void)
{
    const uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};
    ESP_ERROR_CHECK(uart_param_config(UART_USED, &uart_config));
    ESP_ERROR_CHECK(uart_set_pin(UART_USED, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    ESP_ERROR_CHECK(uart_driver_install(UART_USED, 2048, 0, 0, NULL, 0));
}

// Create Task to read data from UART
void read_uart(void *pvParameter)
{
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE);
    while (1)
    {
        int len = uart_read_bytes(UART_USED, data, BUF_SIZE, 20 / portTICK_PERIOD_MS);
        if (len > 0)
        {
            data[len] = 0;
            ESP_LOGI("UART", "Read data: %s", data);
        }
    }
    free(data);
}
//  Task to change color of LED RGB according to the data read from UART
void change_color(void *pvParameter)
{
    uint8_t *data = (uint8_t *)malloc(BUF_SIZE + 1);
    while (1)
    {
        int len = uart_read_bytes(UART_USED, data, BUF_SIZE, 20 / portTICK_PERIOD_MS);
        if (len > 0)
        {
            data[len] = 0;
            if (strncmp(data, "RED", 3) == 0)
            {
                if(strcmp(data[4],"R")==0){
                    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY);
                    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
                    ESP_LOGI("LED RGB", "Color: RED");
                }
/*                 ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, LEDC_DUTY);
                ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
                ESP_LOGI("LED RGB", "Color: RED"); */
            }
            else
            {
                ESP_LOGI("LED RGB", "Color: NONE");
            }
        }
    }
    free(data);
}

void app_main(void)
{
    // Set the LEDC peripheral configuration
    init_led_rgb();
    // Set the UART peripheral configuration
    init_uart();
    // Set the LEDC channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = LEDC_OUTPUT_IO,
        .duty = 0, // Set duty to 0%
        .hpoint = 0};
    while (1)   
    {
        
        xTaskCreate(read_uart, "read_uart", 1024 * 2, NULL, 1, NULL);
        xTaskCreate(change_color, "change_color", 1024 * 2, NULL, 1, NULL);
    }
    

}
