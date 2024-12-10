#include <Wire.h>                // Biblioteca para comunicação I2C
#include <Adafruit_GFX.h>        // Biblioteca para gráficos
#include <Adafruit_SSD1306.h>    // Biblioteca para o display OLED
#include <ESP32Servo.h>          // Biblioteca para controle de servo no ESP32
#include <Adafruit_NeoPixel.h>   // Biblioteca para controle do LED Ring

// Configurações do display OLED
#define SCREEN_WIDTH 128         // Largura do display OLED
#define SCREEN_HEIGHT 64         // Altura do display OLED
#define OLED_RESET -1            // Não é necessário resetar o OLED no ESP32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Configuração do servo motor
Servo meuServo;
const int pinoServo = 32; // Pino onde o sinal do servo está conectado (GPIO 32)

// Configuração dos pinos de botões e potenciômetro
const int botaoLoop = 27;       // Botão para iniciar o modo automático
const int botaoManual = 26;     // Botão para iniciar o modo manual
const int botaoParada = 25;     // Botão para parar o sistema
const int potenciometro = 34;   // Pino ADC do potenciômetro (entrada analógica)

// Configuração do LED Ring
#define LED_PIN 33               // Pino de controle de dados para o LED Ring
#define NUM_LEDS 16              // Quantidade de LEDs no LED Ring
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);

// Variáveis de controle do estado do sistema
int angulo = 0;                // Ângulo atual do servo
bool aumentando = true;        // Indica se o servo está aumentando ou diminuindo o ângulo
bool modoAutomatico = false;   // Indica se o sistema está no modo automático
bool modoManual = false;       // Indica se o sistema está no modo manual
bool sistemaParado = true;     // Indica se o sistema está parado

void setup() {
  // Inicia a comunicação serial para debug
  Serial.begin(115200);

  // Configura os botões como entradas com pull-up interno
  pinMode(botaoLoop, INPUT_PULLUP);
  pinMode(botaoManual, INPUT_PULLUP);
  pinMode(botaoParada, INPUT_PULLUP);
  
  // Configura o ADC para o potenciômetro (resolução de 12 bits, valor entre 0 e 4095)
  analogReadResolution(12);

  // Configura a comunicação I2C com os pinos SDA e SCL
  Wire.begin(21, 22); // SDA = 21, SCL = 22

  // Inicializa o display OLED no endereço I2C 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao iniciar o display SSD1306"));
    for (;;); // Loop infinito se o display não for detectado
  }

  exibirMensagem("Sistema Iniciado!");
  delay(2000); // Pausa para exibir a mensagem inicial
  
  // Inicializa o servo no pino definido
  meuServo.attach(pinoServo, 500, 2500); 

  // Inicializa o LED Ring
  strip.begin();
  strip.show();
}

void loop() {
  // Verifica o estado dos botões
  if (digitalRead(botaoParada) == LOW) pararSistema();
  if (digitalRead(botaoLoop) == LOW) iniciarModoAutomatico();
  if (digitalRead(botaoManual) == LOW) iniciarModoManual();
  
  // Executa o controle do servo de acordo com o modo selecionado
  if (modoAutomatico && !sistemaParado) moverServoAutomatico();
  if (modoManual && !sistemaParado) moverServoManual();
}

void pararSistema() {
  // Para o funcionamento do sistema
  modoAutomatico = false;
  modoManual = false;
  sistemaParado = true;
  mostrarNoDisplay("Sistema Parado", "Aguardando comando");
  meuServo.detach(); // Desliga o controle do servo
}

void iniciarModoAutomatico() {
  modoAutomatico = true;
  modoManual = false;
  sistemaParado = false;
  mostrarNoDisplay("Modo Automatico", "Iniciado");
  meuServo.attach(pinoServo, 500, 2500); // Reanexa o controle do servo
}

void iniciarModoManual() {
  modoAutomatico = false;
  modoManual = true;
  sistemaParado = false;
  mostrarNoDisplay("Modo Manual", "Use o Potenciometro");
  meuServo.attach(pinoServo, 500, 2500); // Reanexa o controle do servo
}

void moverServoAutomatico() {
  // Faz o servo se mover automaticamente de 0° a 180° e depois de volta para 0°
  if (aumentando) {
    angulo++;
    if (angulo >= 180) {
      delay(1000); // Pausa de 1 segundo ao atingir 180°
      aumentando = false;
    }
  } else {
    angulo--;
    if (angulo <= 0) {
      delay(1000); // Pausa de 1 segundo ao atingir 0°
      aumentando = true;
    }
  }
  
  meuServo.write(angulo);
  controlarLED(angulo);
  mostrarNoDisplay("Angulo: " + String(angulo) + " graus", "Modo: Automatico");
  delay(aumentando ? 100 : 200);
}

void moverServoManual() {
  // Controla o ângulo do servo com base no potenciômetro
  int valorPotenciometro = analogRead(potenciometro); 
  int angulo = map(valorPotenciometro, 0, 4095, 0, 180); 
  meuServo.write(angulo);
  controlarLED(angulo);
  mostrarNoDisplay("Angulo: " + String(angulo) + " graus", "Modo: Manual");
  delay(100);
}

void controlarLED(int angulo) {
  // Ativa o LED correspondente no LED Ring de acordo com o ângulo do servo
  int ledIndex = angulo / 20; // Cada LED corresponde a 20 graus
  if (ledIndex > 8) ledIndex = 8; // Garante que o LED 8 seja o máximo
  strip.clear();
  strip.setPixelColor(ledIndex, strip.Color(255, 0, 0)); // Ativa o LED na cor vermelha
  strip.show();
}

void mostrarNoDisplay(String mensagem1, String mensagem2) {
  // Mostra duas linhas de texto no display OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(mensagem1);
  display.println(mensagem2);
  display.display();
}

void exibirMensagem(String mensagem) {
  // Mostra uma única mensagem no display OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(mensagem);
  display.display();
}
