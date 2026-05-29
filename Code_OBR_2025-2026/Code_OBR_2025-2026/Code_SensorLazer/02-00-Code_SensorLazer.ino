#include <HCSR04.h>
#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

int ledB = 2;
int ledA = 4;
int ledV = 6;

void setup () {
    Serial.begin(9600); 
    pinMode(ledB, OUTPUT); 
    pinMode(ledA, OUTPUT); 
    pinMode(ledV, OUTPUT); 

    while (!Serial) {
        delay(1);
    }
  
    Serial.println("Adafruit VL53L0X test");
    if (!lox.begin()) {
        while (1);
    }
   
    Serial.println(F("VL53L0X API Simple Ranging example\n\n")); 
}


//Leitura e condicionais para verificação de objeto 
void loop () {
    VL53L0X_RangingMeasurementData_t measure;
    
    Serial.print("Reading a measurement... ");
    lox.rangingTest(&measure, false);

    int distancia = ((measure.RangeMilliMeter) - 26) / 10;
    Serial.println(distancia);
    int contagem_desvia = 0;
    int contagem_taperto = 0;
    int contagem_talonge = 0;
    int verificacoes = 5;

    for (int i = 0; i < verificacoes; i++){
      delay(1);
      lox.rangingTest(&measure, false);
      int distancia = ((measure.RangeMilliMeter) - 26)/10;
    if (distancia < 8) {
        contagem_desvia++;
    } 
    else if (distancia >= 8 && distancia <= 18) {
        contagem_taperto++;
    } 
    else if (distancia > 20) {
        contagem_talonge++;
    }
    }


    if (contagem_desvia >= verificacoes) {
        digitalWrite(ledB, HIGH);
        digitalWrite(ledA, LOW);
        digitalWrite(ledV, LOW);
    } 
    else if (contagem_taperto >= verificacoes) {
        digitalWrite(ledB, LOW);
        digitalWrite(ledA, HIGH);
        digitalWrite(ledV, LOW);
    } 
    else if (contagem_talonge >= verificacoes) {
        digitalWrite(ledB, LOW);
        digitalWrite(ledA, LOW);
        digitalWrite(ledV, HIGH);
    } 
   
}
