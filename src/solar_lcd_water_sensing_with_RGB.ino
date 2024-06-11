#include <LiquidCrystal_I2C.h>
#define R 8
#define G 9
#define B 10

LiquidCrystal_I2C lcd (0x27, 16, 2);

float solar_cell = 0;

void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(9600);
  lcd.init() ;               
  lcd.backlight();      //Serial 不需要此指令

  pinMode(R,OUTPUT);   
  pinMode(G,OUTPUT);   
  pinMode(B,OUTPUT);
}

void loop()
{
  solar_cell = (analogRead(A0) * (5.001 / 1023.001));
  Serial.println(solar_cell);
  lcd.setCursor(0, 0);
  lcd.print("Solar Cell Volt");
  lcd.setCursor(0, 1);
  lcd.print("Value = ");
  lcd.setCursor(8, 1);
  lcd.print(solar_cell);
  delay(10); // Delay a little bit to improve simulation performance

  int sensor=analogRead(A1); // Incoming analog  signal read and appointed sensor
  Serial.println(sensor);   //Wrote serial port

  if (sensor <= 100) {
    digitalWrite(R,HIGH);
    digitalWrite(G,LOW);
    digitalWrite(B,LOW);
  } else if (100 < sensor and sensor < 400) {
    digitalWrite(R,LOW);
    digitalWrite(G,HIGH);
    digitalWrite(B,LOW);
  } else {
    digitalWrite(R,LOW);
    digitalWrite(G,LOW);
    digitalWrite(B,HIGH);
  }
}