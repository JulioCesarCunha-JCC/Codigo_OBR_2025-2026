#include "Wire.h"
#include <MPU6050_light.h>
int Acelera = 6;
int Reduz = 2;
int Default = 4;

MPU6050 mpu(Wire);
unsigned long timer = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();
  
  byte status = mpu.begin();
  Serial.print(F("MPU6050 status: "));
  Serial.println(status);
  while(status!=0){ } // stop everything if could not connect to MPU6050
  
  Serial.println(F("Calculo do offsets, NÃO MOVA O SENSOR"));
  delay(1000);
  mpu.calcOffsets(); // Giroscópio e acelerômetro
  Serial.println("Done!\n");
  pinMode(Acelera, OUTPUT);
  pinMode(Reduz, OUTPUT);
  pinMode(Default, OUTPUT);
}

//Leituras e condicionais para identificação do ângulo do robô 

void loop() {
  mpu.update();
  
  if((millis()-timer)>100){ // print data every 10ms
  Serial.print("X : ");
  Serial.print(mpu.getAngleX());
  Serial.print("\tY : ");
  Serial.print(mpu.getAngleY());
  Serial.print("\tZ : ");
  Serial.println(mpu.getAngleZ());
  timer = millis();  
  }
    int anguloX = mpu.getAngleX();
    int anguloAcelera = 0;
    int anguloReduz = 0;
    int anguloDefault = 0;
    int verificacoes = 5;
    for (int i = 0; i < verificacoes; i++){
      mpu.update();
      int anguloX = mpu.getAngleX();
      if (anguloX <= -12){
      anguloAcelera++;
      }
      else if (anguloX >= 7){
      anguloReduz++;
      }
      else if(anguloX < 5 && anguloX > -12){
      anguloDefault++;
      }
    }
    if (anguloAcelera >= verificacoes){
      digitalWrite(Acelera, HIGH);
     
    }
    else if  (anguloAcelera < verificacoes){
      digitalWrite(Acelera, LOW);
      
    }
    if (anguloReduz >= verificacoes){
      digitalWrite(Reduz, HIGH);
   
    }
    else if  (anguloReduz < verificacoes){
      digitalWrite(Reduz, LOW);
  
    }
    if (anguloDefault >= verificacoes){
      digitalWrite(Default, HIGH);
   
    }
    else if  (anguloDefault < verificacoes){
      digitalWrite(Default, LOW);
  
    }
}
