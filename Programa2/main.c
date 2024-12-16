#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

// Definição dos pinos para o LED RGB
#define PINO_RED   25                              // Pino GPIO para o LED Vermelho (R)
#define PINO_GREEN 26                              // Pino GPIO para o LED Verde (G)
#define PINO_BLUE  27                              // Pino GPIO para o LED Azul (B)

// Configurações do LEDC (LED Control)
#define LEDC_TIMER           LEDC_TIMER_0           // Timer usado para o controle do PWM
#define LEDC_MODE            LEDC_LOW_SPEED_MODE    // Modo de velocidade baixa
#define LEDC_CHANNEL_RED     LEDC_CHANNEL_0         // Define o canal vermelho
#define LEDC_CHANNEL_GREEN   LEDC_CHANNEL_1         // Define o canal verde
#define LEDC_CHANNEL_BLUE    LEDC_CHANNEL_2         // Define o canal azul
#define LEDC_DUTY_RES        LEDC_TIMER_8_BIT       // Resolução de 8 bits (0-255)
#define LEDC_FREQUENCY       5000                   // Frequência de 5kHz
#define INCREMENTO           5                      // Incremento do duty cycle

// Função para configurar o canal de PWM
void configurar_pwm(ledc_channel_config_t *channel_config, int gpio_num, int channel) {
    channel_config->channel    = (ledc_channel_t)channel; 
    channel_config->duty       = 0;             // Duty cycle inicial
    channel_config->gpio_num   = gpio_num;
    channel_config->speed_mode = LEDC_MODE;
    channel_config->hpoint     = 0;
    channel_config->timer_sel  = LEDC_TIMER;
}

// Tarefa para controle de PWM
void tarefa_pwm(void *pvParameters) {
    // Configuração dos três canais PWM (R, G, B)
    ledc_channel_config_t ledc_channel_red, ledc_channel_green, ledc_channel_blue;

    configurar_pwm(&ledc_channel_red, PINO_RED, LEDC_CHANNEL_RED);
    configurar_pwm(&ledc_channel_green, PINO_GREEN, LEDC_CHANNEL_GREEN);
    configurar_pwm(&ledc_channel_blue, PINO_BLUE, LEDC_CHANNEL_BLUE);

    ledc_channel_config(&ledc_channel_red);
    ledc_channel_config(&ledc_channel_green);
    ledc_channel_config(&ledc_channel_blue);

    // Variáveis para controle de duty cycle
    int duty_red = 0;
    int duty_green = 0;
    int duty_blue = 0;
    int incremento = INCREMENTO;
    int contador = 0;

    while (1) {
        // Cálculo dos duty cycles para R, G e B
        duty_red = (incremento * 2) % 256; 
        duty_green = (incremento * 1) % 256;  
        duty_blue = (incremento * 3) % 256; 

        // Atualização dos canais PWM
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_RED, duty_red);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_RED);

        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_GREEN, duty_green);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_GREEN);

        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL_BLUE, duty_blue);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL_BLUE);

        // Exibir o duty cycle no terminal a cada 5 loops
        if (contador % 1 == 0) {
            printf("Incremento: %d, Duty Cycle (R, G, B): %d, %d, %d\n", incremento, duty_red, duty_green, duty_blue);
        }
        
        // Atualizar o valor do incremento
        incremento += INCREMENTO;
        if (incremento > 255) {
            incremento = 0;         // Reseta o incremento
        }

        // Delay de 0,1 segundo para liberar o processador
        vTaskDelay(50 / portTICK_PERIOD_MS);
        contador++;
    }
}

void app_main(void) {
    // Configuração do temporizador do PWM
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,         
        .duty_resolution = LEDC_DUTY_RES, 
        .timer_num = LEDC_TIMER,          
        .freq_hz = LEDC_FREQUENCY,        
        .clk_cfg = LEDC_AUTO_CLK          
    };
    ledc_timer_config(&ledc_timer);

    // Criar a tarefa de controle de PWM
    xTaskCreate(tarefa_pwm, "Tarefa PWM", 2048, NULL, 1, NULL);
}
