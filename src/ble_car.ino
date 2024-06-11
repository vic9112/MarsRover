#include <DHT.h>
#include <DHT_U.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <SparkFun_APDS9960.h>

SoftwareSerial bluetooth(3,11); // RX, TX
LiquidCrystal_I2C lcd (0x27, 16, 2);
DHT dht(4, DHT11); 
char t;
int trigPinR = A0;    // 超音波感測器 Trig腳接 Arduino pin 11
int echoPinR = A1;    // 超音波感測器 Echo 腳接 Arduino pin 12
int trigPinL = 12;    // 超音波感測器 Trig腳接 Arduino pin 11
int echoPinL = 13;    // 超音波感測器 Echo 腳接 Arduino pin 12
int trigPinM = A2;    // 超音波感測器 Trig腳接 Arduino pin 11
int echoPinM = A3;    // 超音波感測器 Echo 腳接 Arduino pin 12

long durationR, cmR, durationL, cmL, durationM, cmM;  //宣告計算距離時，需要用到的兩個實數

#define M1 5
#define M2 6
#define M3 7
#define M4 8
#define M5 9
#define M6 10

#define auto_spd 128
#define mnul_spd 128
#define distance 22
bool auto_mode = false;
bool turn = true;

// Global Variables
SparkFun_APDS9960 apds = SparkFun_APDS9960();
uint16_t ambient_light = 0;
uint16_t red_light = 0;
uint16_t green_light = 0;
uint16_t blue_light = 0;
 
void setup()
{
  Serial.begin(9600);
  // BLE
  bluetooth.begin(9600);
  pinMode(M1,OUTPUT);   
  pinMode(M2,OUTPUT);   
  pinMode(M3,OUTPUT);   
  pinMode(M4,OUTPUT);   
  pinMode(M5,OUTPUT);   
  pinMode(M6,OUTPUT);

  pinMode(trigPinR, OUTPUT);      //Arduino 對外啟動距離感測器Trig腳，射出超音波 
  pinMode(echoPinR, INPUT);       //超音波被障礙物反射後，Arduino讀取感測器Echo腳的時間差
  pinMode(trigPinL, OUTPUT);      //Arduino 對外啟動距離感測器Trig腳，射出超音波 
  pinMode(echoPinL, INPUT); 
  pinMode(trigPinM, OUTPUT);      //Arduino 對外啟動距離感測器Trig腳，射出超音波 
  pinMode(echoPinM, INPUT);       //超音波被障礙物反射後，Arduino讀取感測器Echo腳的時間差

  // DHT & LCD
  dht.begin(); 
  lcd.init() ;               
  lcd.backlight();
   // Initialize APDS-9960 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9960 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9960 init!"));
  }
  
  // Start running the APDS-9960 light sensor (no interrupts)
  if ( apds.enableLightSensor(false) ) {
    Serial.println(F("Light sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during light sensor init!"));
  }
  
  // Wait for initialization and calibration to finish
  delay(500);
}

int cnt = 0;
void loop() {
  // Humidity & Temperature

  float H = dht.readHumidity();
  float T = dht.readTemperature(); 

  if(cnt<40){
      lcd.setCursor(0, 0);   
      lcd.print("Humidity(%):");
      lcd.setCursor(12, 0); 
      lcd.print(H);
      lcd.setCursor(0, 1);   
      lcd.print("Temp.(*C):");         
      lcd.setCursor(10, 1); lcd.print(T);
      cnt++;
    }
  else if(cnt==40){lcd.clear();
        cnt++;
      if (  !apds.readAmbientLight(ambient_light) ||
        !apds.readRedLight(red_light) ||
        !apds.readGreenLight(green_light) ||
        !apds.readBlueLight(blue_light) ) {
    Serial.println("Error reading light values");
  } else {
    Serial.print("Ambient: ");
    Serial.print(ambient_light);
    Serial.print(" Red: ");
    Serial.print(red_light);
    Serial.print(" Green: ");
    Serial.print(green_light);
    Serial.print(" Blue: ");
    Serial.println(blue_light);
  }
  }
  else if(cnt<80){
      lcd.setCursor(0, 0);   
      lcd.print("A:");
      lcd.print(ambient_light);
      lcd.setCursor(8, 0);   
      lcd.print("R:");
      lcd.print(red_light);
      lcd.setCursor(0, 1);   
      lcd.print("G:");
      lcd.print(green_light);
      lcd.setCursor(8, 1);   
      lcd.print("B:");
      lcd.print(blue_light);
      cnt++;
    }
  else{
      cnt=0;
    }

  
  // Bluetooth Control
  if(bluetooth.available() > 0) {
    t = bluetooth.read();
    Serial.println(t);
  }

  if(t=='E'){
    auto_mode = true;
  }

  if(t=='G' or t=='g'){
    auto_mode = false;
  }
  
  if(auto_mode) {
    if(t=='G' or t=='g'){
      auto_mode = !auto_mode;
    }
    digitalWrite(trigPinR, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPinR, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinR, LOW);
    pinMode(echoPinR, INPUT);
    durationR = pulseIn(echoPinR, HIGH);
    cmR = (durationR/2) / 29.1;  
    
    digitalWrite(trigPinL, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPinL, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinL, LOW);
    pinMode(echoPinL, INPUT);
    durationL = pulseIn(echoPinL, HIGH);
    cmL = (durationL/2) / 29.1;  

    digitalWrite(trigPinM, LOW);
    delayMicroseconds(5);
    digitalWrite(trigPinM, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPinM, LOW);
    pinMode(echoPinM, INPUT);
    durationM = pulseIn(echoPinM, HIGH);
    cmM = (durationM/2) / 29.1;  
    // Turn Right
    if (cmM < distance && cmL < distance){
      analogWrite(M1,auto_spd-10);
      analogWrite(M2,auto_spd-10);
      analogWrite(M3,0);
      analogWrite(M4,0);
      analogWrite(M5,auto_spd-10);
      analogWrite(M6,auto_spd-10);
    // Turn Left
    } else if (cmM < distance && cmR < distance){
      analogWrite(M1,auto_spd);
      analogWrite(M2,0);
      analogWrite(M3,auto_spd);
      analogWrite(M4,auto_spd);
      analogWrite(M5,0);
      analogWrite(M6,auto_spd);
    // Backward
    } else if (cmM < distance) {
      analogWrite(M1,mnul_spd);
      analogWrite(M2,0);
      analogWrite(M3,mnul_spd);
      analogWrite(M4,0);
      analogWrite(M5,mnul_spd);
      analogWrite(M6,mnul_spd);
      
    // Turn Left
    } else if (cmR < distance){
      analogWrite(M1,auto_spd);
      analogWrite(M2,0);
      analogWrite(M3,auto_spd);
      analogWrite(M4,auto_spd);
      analogWrite(M5,0);
      analogWrite(M6,auto_spd);
    // Turn Right
    } else if(cmL < distance){
      analogWrite(M1,auto_spd-10);
      analogWrite(M2,auto_spd-10);
      analogWrite(M3,0);
      analogWrite(M4,0);
      analogWrite(M5,auto_spd-10);
      analogWrite(M6,auto_spd-10);
    // Go Straight
    } else {
      analogWrite(M1,auto_spd-20);
      analogWrite(M2,auto_spd-20);
      analogWrite(M3,0);
      analogWrite(M4,auto_spd);
      analogWrite(M5,0);
      analogWrite(M6,auto_spd);
    }
    
    Serial.print("cmR: ");
    Serial.print(cmR);
    Serial.print(", cmL: ");
    Serial.print(cmL);
    Serial.print(", cmM: ");
    Serial.println(cmM);
  } else { 
    switch(t) {
      case 'A':
        {  //move forward
          analogWrite(M1,mnul_spd);
          analogWrite(M2,mnul_spd);
          analogWrite(M3,0);
          analogWrite(M4,mnul_spd);
          analogWrite(M5,0);
          analogWrite(M6,mnul_spd);
        }
      break;

      case 'C':
        {  //move backward
          analogWrite(M1,mnul_spd);
          analogWrite(M2,0);
          analogWrite(M3,mnul_spd);
          analogWrite(M4,0);
          analogWrite(M5,mnul_spd);
          analogWrite(M6,mnul_spd);
        }
      break;

      case 'B':
        {  //move right
          analogWrite(M1,mnul_spd);
          analogWrite(M2,mnul_spd);
          analogWrite(M3,0);
          analogWrite(M4,0);
          analogWrite(M5,mnul_spd);
          analogWrite(M6,mnul_spd);
        }
      break;

      case 'D':
        {  //move left
          analogWrite(M1,mnul_spd);
          analogWrite(M2,0);
          analogWrite(M3,mnul_spd);
          analogWrite(M4,mnul_spd);
          analogWrite(M5,0);
          analogWrite(M6,mnul_spd);
        }
      break;

      case 0:
        {  // Stop
          digitalWrite(M1,LOW);
          digitalWrite(M2,LOW);
          digitalWrite(M3,LOW);
          digitalWrite(M4,LOW);
          digitalWrite(M5,LOW);
          digitalWrite(M6,LOW);
        }
      break;       
    } 
  }
}
