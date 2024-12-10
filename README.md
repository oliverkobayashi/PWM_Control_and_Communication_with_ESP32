# PWM_Control_and_Communication_with_ESP32

Este repositório contém a implementação de soluções práticas de controle PWM e comunicação serial utilizando o ESP32. O objetivo principal é desenvolver e documentar dois programas principais, cada um com requisitos e objetivos específicos, de forma a atender aos critérios de um projeto final de microcontroladores.

Cada programa está organizado em uma **pasta individual**, onde estão disponibilizados os seguintes itens:
- **Código fonte** (arquivo `.ino` ou equivalente)
- **Diagramas e esquemas** de ligação dos componentes
- **Imagens e prints** das simulações e testes realizados
- **Explicações detalhadas** do funcionamento de cada parte do código e das decisões de design.

---

## 📘 **Descrição dos Programas**

### 🔥 **Programa 1 - Controle do Servo Motor com Display OLED e LED Ring**
> **Local:** `/Programa1/`

Este programa utiliza a **plataforma Wokwi** e o **ESP32 com o framework Arduino IDE** para o controle de um **servo motor**, um **display OLED** e um **LED Ring Adafruit**. O objetivo é permitir o controle manual e automático do servo, além de indicar o ângulo do servo por meio de um **LED correspondente no LED Ring** e exibir informações no **display OLED**.

**🖥️ Demonstração do Projeto 1:**
> ![Demonstração do Projeto 1](Programa1/imagens/Projeto1_Demo.PNG)

---

### 🧾 **Requisitos Implementados**
- **⚙️ Modo Automático**: 
  - O servo se move de **0° a 180°** e volta para **0°** de forma contínua.
  - Há um **delay de 1 segundo** nas posições de 0° e 180° antes de iniciar o movimento de retorno.
  - O movimento se repete de forma **contínua**.
  
- **🎚️ Modo Manual**: 
  - O servo é controlado diretamente pelo **potenciômetro**.
  - O ângulo do servo é proporcional ao valor lido no potenciômetro, permitindo o ajuste manual de forma intuitiva.

- **🎮 Controle via Botões**:
  - **Botão de Loop** (GPIO 27): Ativa o modo automático.
  - **Botão de Modo Manual** (GPIO 26): Ativa o modo manual.
  - **Botão de Parada** (GPIO 25): Interrompe o funcionamento do servo, parando qualquer movimento.

- **📟 Exibição no Display OLED**: 
  - O display mostra o **ângulo do servo** e o **modo de operação** em tempo real.
  - Exibição típica no OLED:
    ```
    Angulo: 45 graus
    Modo: Automático
    ```

- **💡 Controle de LED Ring Adafruit**: 
  - O **LED correspondente ao ângulo do servo** é ativado de acordo com o seguinte esquema:
    - **LED 0**: 0° a 20°
    - **LED 1**: 21° a 40°
    - **LED 2**: 41° a 60°
    - **LED 3**: 61° a 80°
    - **LED 4**: 81° a 100°
    - **LED 5**: 101° a 120°
    - **LED 6**: 121° a 140°
    - **LED 7**: 141° a 160°
    - **LED 8**: 161° a 180°
  - Apenas o LED correspondente ao intervalo de ângulo é aceso.

---

### ⚙️ **Componentes Utilizados**
- **Servo Motor**: Controlado via PWM com a biblioteca **ESP32Servo**.
- **Display OLED**: Comunicação via I2C com as bibliotecas **Adafruit GFX** e **Adafruit SSD1306**.
- **Potenciômetro**: Conectado ao GPIO **34** para controle manual.
- **Botões**: Conectados aos GPIOs **25, 26 e 27** para controle dos modos de operação.
- **LED Ring Adafruit**: LEDs que acendem conforme o ângulo do servo, com controle de cor e intensidade via a biblioteca **Adafruit NeoPixel**.

---

### 🛠️ **Conexões Físicas**
| **Componente**         | **Pino do ESP32** |
|---------------------|------------------|
| **Servo Motor**       | GPIO 32           |
| **Display OLED (SDA)**| GPIO 21           |
| **Display OLED (SCL)**| GPIO 22           |
| **Potenciômetro**     | GPIO 34 (ADC)     |
| **Botão Loop**        | GPIO 27           |
| **Botão Manual**      | GPIO 26           |
| **Botão Parada**      | GPIO 25           |
| **LED Ring**          | GPIO 33           |

---

### 📂 **Arquivos do Programa**
- **`/Programa1/codigo.ino`**: Código fonte do programa com comentários explicativos.
- **`/Programa1/imagens/`**: Prints do Wokwi, exibição no OLED e detalhes de funcionamento.
- **`/Programa1/README.md`**: Explicação detalhada do Programa 1, incluindo o funcionamento do controle do servo, do display OLED e do LED Ring.

---


### 🌈 **Programa 2 - Controle de LED RGB com PWM**
> **Local:** `/Programa_2/`

O objetivo deste programa é controlar um **LED RGB** por meio de **modulação PWM** utilizando a **programação nativa ESP-IDF**. Este programa não utiliza o Arduino IDE e foi feito no ambiente **ESP-IDF**. O controle permite modificar o brilho de cada uma das cores RGB de forma individual.

**Requisitos Implementados:**
- **Controle PWM das cores (R, G, B)**:
  - As cores **Red (R)**, **Green (G)** e **Blue (B)** são controladas separadamente.
  - Cada cor possui um duty cycle independente que varia de **0% a 100%**.
- **Incremento Personalizado**:
  - O duty cycle de cada cor aumenta em incrementos de valor predefinido.
  - Incremento aplicado:
    - **R = incremento * 2**
    - **G = incremento**
    - **B = incremento * 3**
- **Exibição de Mensagens no Monitor Serial**:
  - As mensagens de log incluem o duty cycle de cada cor e os incrementos aplicados.
  - Exemplo de saída no monitor serial:
    ```
    [INFO] Incremento atual: 10
    [INFO] Duty Cycle R: 20%, G: 10%, B: 30%
    ```

**Componentes Utilizados:**
- **LED RGB (cátodo comum)**: Controlado via 3 canais de PWM.
- **Resistores**: Resistores de 220 ohms foram utilizados para limitar a corrente.

> **Arquivos disponíveis:**
- **`/Programa_2/main.c`**: Código fonte com controle PWM de LED RGB.
- **`/Programa_2/imagens/`**: Prints de simulação no Wokwi e imagens do terminal serial.

---

## 📜 **Como Executar o Projeto**

### 🔧 **Executar o Programa 1**
1. **Simulação Wokwi**:
   - Acesse o [Wokwi](https://wokwi.com/).
   - Importe o arquivo **`/Programa_1/codigo.ino`** no Wokwi.
   - Simule o circuito conforme o diagrama disponível na pasta **`/Programa_1/imagens/`**.

2. **Execução no ESP32**:
   - Carregue o arquivo **`/Programa_1/codigo.ino`** no **Arduino IDE**.
   - Conecte o **ESP32** ao computador e envie o código para a placa.
   - Conecte os seguintes componentes:
     - **Servo Motor** no GPIO 32.
     - **Display OLED** com comunicação I2C.
     - **Botões** no GPIOs 25, 26 e 27.
     - **Potenciômetro** no GPIO 34.
     - **LED Ring** no GPIO 33.

---

## 📚 **Referências**
- [Documentação da biblioteca Adafruit GFX](https://learn.adafruit.com/adafruit-gfx-graphics-library/overview)
- [Documentação da biblioteca Adafruit SSD1306](https://learn.adafruit.com/monochrome-oled-breakouts/overview)
- [Documentação ESP32Servo](https://github.com/PaulStoffregen/Servo)
- [Documentação do ESP-IDF](https://docs.espressif.com/projects/esp-idf/en/latest/esp32/)
