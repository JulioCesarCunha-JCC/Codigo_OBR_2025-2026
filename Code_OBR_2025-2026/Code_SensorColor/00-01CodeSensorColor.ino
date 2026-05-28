#include <Wire.h>
#include <Adafruit_TCS34725.h>

// Inicializa o sensor com tempo de integração de 2.4ms e ganho de 60x
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);

// Pinos dos LEDs: Índice 0 = Verde (D2), 1 = Prata (D4), 2 = Vermelho (D6)
const int leds[] = { 2, 4, 6 };

// Valores de calibração para comparação (Valores do Código 2)
const float corVerde[] = { 69, 107, 88 };     // {red, green, blue}
const float corVermelho[] = { 116, 79, 73 };  // {red, green, blue}
const float corPrata[] = { 92, 92, 80, 850 }; // {red, green, blue, claridade}

const int adicional = 8; // Margem de tolerância para as cores
const int verify = 5;    // Quantidade de leituras para confirmação

void setup() {
  Serial.begin(9600);
 
  if (tcs.begin()) {
    Serial.println("Sensor TCS34725 encontrado!");
  } else {
    Serial.println("Nenhum TCS34725 encontrado ... verifique as conexoes");
    while (1); // Trava o programa se o sensor não for encontrado
  }

  // Corrigido: loop vai apenas de 0 a 2 (3 pinos) para evitar estouro de array
  for (int i = 0; i < 3; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  int confirmaVerde = 0;
  int confirmaVermelho = 0;
  int confirmaPrata = 0;

  // Realiza 5 leituras reais consecutivas para garantir precisão
  for (int i = 0; i < verify; i++) {
    float red, green, blue;
    uint16_t r, g, b, c;

    tcs.setInterrupt(false); // Liga o LED do sensor
    delay(35);               // Tempo para estabilizar a leitura (ajustado para o tempo de 2.4ms)
    tcs.getRGB(&red, &green, &blue);
    tcs.getRawData(&r, &g, &b, &c);
    tcs.setInterrupt(true);  // Desliga o LED do sensor

    float leituraSensor[] = { red, green, blue };

    // Testa se a cor predominante é verde e se está dentro da margem
    if (green > red && green > blue) {
      bool rgbValido = true;
      for (int j = 0; j < 3; j++) {
        if (!((leituraSensor[j] < (corVerde[j] + adicional)) && (leituraSensor[j] > (corVerde[j] - adicional)))) {
          rgbValido = false;
        }
      }
      if (rgbValido) confirmaVerde++;
    }

    // Testa se a cor predominante é vermelha e se está dentro da margem
    if (red > green && red > blue) {
      bool rgbValido = true;
      for (int j = 0; j < 3; j++) {
        if (!((leituraSensor[j] < (corVermelho[j] + adicional)) && (leituraSensor[j] > (corVermelho[j] - adicional)))) {
          rgbValido = false;
        }
      }
      if (rgbValido) confirmaVermelho++;
    }

    // Testa se a claridade atinge o nível do prata
    if (c > corPrata[3]) {
      confirmaPrata++;
    }
   
    // Pequeno intervalo entre as leituras de verificação
    delay(15);
  }

  // Faz uma última leitura rápida apenas para mostrar no Monitor Serial atualizado
  float r_print, g_print, b_print;
  uint16_t rc, gc, bc, cc;
  tcs.getRGB(&r_print, &g_print, &b_print);
  tcs.getRawData(&rc, &gc, &bc, &cc);

  // Acende o LED correspondente se passar em TODAS as 5 leituras
  if (confirmaVerde == verify) {
    digitalWrite(leds[0], HIGH); // Verde
    digitalWrite(leds[1], LOW);
    digitalWrite(leds[2], LOW);
    Serial.print(" -> COR DETECTADA: VERDE");
  }
  else if (confirmaVermelho == verify) {
    digitalWrite(leds[2], HIGH); // Vermelho
    digitalWrite(leds[0], LOW);
    digitalWrite(leds[1], LOW);
    Serial.print(" -> COR DETECTADA: VERMELHO");
  }
  else if (confirmaPrata == verify) {
    digitalWrite(leds[1], HIGH); // Prata (Branco)
    digitalWrite(leds[0], LOW);
    digitalWrite(leds[2], LOW);
    Serial.print(" -> COR DETECTADA: PRATA");
  }
  else {
    // Apaga todos se não houver certeza absoluta da cor
    digitalWrite(leds[0], LOW);
    digitalWrite(leds[1], LOW);
    digitalWrite(leds[2], LOW);
  }

  // Debug para monitoramento no Serial Monitor (Corrigido com aspas)
  Serial.print(" | Vermelho: "); Serial.print(r_print);
  Serial.print(" | Verde: ");    Serial.print(g_print);
  Serial.print(" | Azul: ");     Serial.print(b_print);
  Serial.print(" | Claridade: "); Serial.println(cc);

  delay(500); // Aguarda meio segundo antes de reiniciar o ciclo principal
}
