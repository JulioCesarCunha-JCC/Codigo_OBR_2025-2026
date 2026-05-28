#include <Wire.h>
#include "Adafruit_TCS34725.h"

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_2_4MS, TCS34725_GAIN_60X);

const int leds[] = { 2, 4, 6 };  //D2 Verde , D4 prata , D6 Vermelho
const float corVerde[] = { 80, 107, 95 };     //{red , green , blue }
const float corVermelho[] = { 119, 78, 75};  //{red , green , blue }
const float corPrata[] = {126, 110, 102,950}; //{red , green , blue}

const int adicional = 8;
const int verify = 5;


void setup() {
  Serial.begin(9600);
  //Serial.println("Color View Test!");
  if (tcs.begin()) {
    //Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1)
      ;  // halt!
  }
  for (int i = 0; i <= 3; i++) {
    pinMode(leds[i], OUTPUT);
  }
}

void loop() {
  float red, green, blue;
  uint16_t r,g,b,c;
  tcs.setInterrupt(false);  // Liga o LED do sensor
  delay(50);                // Aguarda 50ms para estabilizar a leitura
  tcs.getRGB(&red, &green, &blue);
  tcs.getRawData(&r, &g, &b, &c);
  tcs.setInterrupt(true);  // Desliga o LED do sensor

  float leituraSensor[] = { red, green, blue };
  int confirmaVerde = 0;
  int confirmaVermelho = 0;
  int confirmaPrata = 0;
  int verify = 5;  // Certifique-se de que este valor está correto

  // Loop principal
  for (int i = 0; i < verify; i++) {
    // Testa se a cor predominante é verde
    if (green > red && green > blue) {
      for (int j = 0; j < 3; j++) {  // Renomeado de 'i' para 'j'
        if ((leituraSensor[j] < (corVerde[j] + adicional)) && (leituraSensor[j] > (corVerde[j] - adicional))) {
          confirmaVerde++;
        }
      }
    }
    // Testa se a cor predominante é vermelha
    if (red > green && red > blue) {
      for (int j = 0; j < 3; j++) {  // Renomeado de 'i' para 'j'
        if ((leituraSensor[j] < (corVermelho[j] + adicional)) && (leituraSensor[j] > (corVermelho[j] - adicional))) {
          confirmaVermelho++;
        }
      }
    }
    // Testa se a cor predominante é prata
    for (int j = 0; j < 3; j++) {  // Renomeado de 'i' para 'j'
      if (c > corPrata[3]) {
        confirmaPrata++;
      }
    }
      
    

  }




  // Acende o LED correspondente
  if (confirmaVerde == (verify * 3)) {
    digitalWrite(leds[0], HIGH);
    digitalWrite(leds[1], LOW); 
    digitalWrite(leds[2], LOW);  // Acende LED verde
  }else if (confirmaVermelho == (verify * 3)) {
    digitalWrite(leds[2], HIGH);
    digitalWrite(leds[0], LOW);  // Acende LED vermelho
    digitalWrite(leds[1], LOW); 
  } else if(confirmaPrata == (verify * 3)){
    digitalWrite(leds[0], LOW);
    digitalWrite(leds[2], LOW);
    digitalWrite(leds[1], HIGH); // Acende LED branco
  }else{
    digitalWrite(leds[0], LOW);
    digitalWrite(leds[2], LOW);
    digitalWrite(leds[1], LOW);
  }

  // Debug para monitoramento
  Serial.print("Red: ");
  Serial.print(red);
  Serial.print(" Green: ");
  Serial.print(green);
  Serial.print(" Blue: ");
  Serial.print(blue);
  Serial.print(" Claridade: ");
  Serial.println(c);
}
