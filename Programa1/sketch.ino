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

// Configuração do servo
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

// Variáveis de controle
int angulo = 0;
bool aumentando = true;
bool modoAutomatico = false;
bool modoManual = false;
bool sistemaParado = true;

void setup() {
  // Inicia a comunicação serial para debug
  Serial.begin(115200);

  // Configura os botões como entradas com pull-up interno
  pinMode(botaoLoop, INPUT_PULLUP);
  pinMode(botaoManual, INPUT_PULLUP);
  pinMode(botaoParada, INPUT_PULLUP);
  
  // Configura o ADC para o potenciômetro
  analogReadResolution(12); // Resolução de 12 bits (0 a 4095)

  // Configura os pinos SDA e SCL para o I2C
  Wire.begin(21, 22); // SDA = 21, SCL = 22

  // Inicializa o display OLED no endereço I2C 0x3C
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("Falha ao iniciar o display SSD1306"));
    for (;;); 
  }

  exibirMensagem("Sistema Iniciado!");
  delay(2000); 
  
  // Inicializa o servo no pino definido
  meuServo.attach(pinoServo, 500, 2500); 

  // Inicializa o LED Ring
  strip.begin();
  strip.show();
}

void loop() {
  if (digitalRead(botaoParada) == LOW) pararSistema();
  if (digitalRead(botaoLoop) == LOW) iniciarModoAutomatico();
  if (digitalRead(botaoManual) == LOW) iniciarModoManual();
  
  if (modoAutomatico && !sistemaParado) moverServoAutomatico();
  if (modoManual && !sistemaParado) moverServoManual();
}

void pararSistema() {
  modoAutomatico = false;
  modoManual = false;
  sistemaParado = true;
  mostrarNoDisplay("Sistema Parado", "Aguardando comando");
  meuServo.detach(); // Desliga o servo
}

void iniciarModoAutomatico() {
  modoAutomatico = true;
  modoManual = false;
  sistemaParado = false;
  mostrarNoDisplay("Modo Automatico", "Iniciado");
  meuServo.attach(pinoServo, 500, 2500);
}

void iniciarModoManual() {
  modoAutomatico = false;
  modoManual = true;
  sistemaParado = false;
  mostrarNoDisplay("Modo Manual", "Use o Potenciometro");
  meuServo.attach(pinoServo, 500, 2500);
}

void moverServoAutomatico() {
  if (aumentando) {
    angulo++;
    if (angulo >= 180) {
      delay(1000); // Adiciona delay de 1 segundo ao atingir 180 graus
      aumentando = false;
    }
  } else {
    angulo--;
    if (angulo <= 0) {
      delay(1000); // Adiciona delay de 1 segundo ao atingir 0 graus
      aumentando = true;
    }
  }
  
  meuServo.write(angulo);
  controlarLED(angulo);
  mostrarNoDisplay("Angulo: " + String(angulo) + " graus", "Modo: Automatico");
  delay(aumentando ? 100 : 200);
}

void moverServoManual() {
  int valorPotenciometro = analogRead(potenciometro); 
  int angulo = map(valorPotenciometro, 0, 4095, 0, 180); 
  meuServo.write(angulo);
  controlarLED(angulo);
  mostrarNoDisplay("Angulo: " + String(angulo) + " graus", "Modo: Manual");
  delay(100);
}

void controlarLED(int angulo) {
  int ledIndex = angulo / 20; 
  if (ledIndex > 8) ledIndex = 8; 
  strip.clear();
  strip.setPixelColor(ledIndex, strip.Color(255, 0, 0)); 
  strip.show();
}

void mostrarNoDisplay(String mensagem1, String mensagem2) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(mensagem1);
  display.println(mensagem2);
  display.display();
}

void exibirMensagem(String mensagem) {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.println(mensagem);
  display.display();
}
