#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd (0x27, 16, 2);

float solar_cell = 0;

void setup()
{
  pinMode(A0, INPUT);
  Serial.begin(9600);
  lcd.init() ;               
  lcd.backlight();      //Serial 不需要此指令
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
}