#include <LiquidCrystal_I2C.h>
#include <DHT.h>

LiquidCrystal_I2C lcd (0x27, 16, 2);
DHT dht(7, DHT11); 

void setup(){
  Serial.begin(9600);
  dht.begin(); 
  lcd.init() ;               
  lcd.backlight();      //Serial 不需要此指令
}

void loop(){
  float H = dht.readHumidity();
  float T = dht.readTemperature(); 
  lcd.setCursor(0, 0);   
  lcd.print("Humidity(%):");
  lcd.setCursor(12, 0); 
  lcd.print(H);
  lcd.setCursor(0, 1);   
  lcd.print("Temp.(*C):");         
  lcd.setCursor(10, 1); lcd.print(T);
  delay(2000);
}
