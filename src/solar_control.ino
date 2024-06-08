// Servo motor library
#include <Servo.h>
// Initialize variables
int mode = 0;
int axe = 0;            
int buttonState1 = 0;    
int buttonState2  = 0;   
int prevButtonState1 = 0;
int prevButtonState2 = 0; 
 
int ldrtopr = A0;               // top-right LDR                          
int ldrtopl = A1;               // top-left LDR                          
int ldrbotr = A2;               // bottom-right LDR                     
int ldrbotl = A3;               // bottom-left  LDR                   
int topl = 0;
int topr = 0; 
int botl = 0;
int botr = 0;

//Declare two servos
Servo servo_updown;
Servo servo_rightleft;

int  threshold_value = 15;           // measurement sensitivity

void setup()
{
  Serial.begin(9600);                                     // serial connection setup
  Serial.println("CLEARDATA");                            // clear all data that’s been place in already
  Serial.println("LABEL,t,voltage,current,power,Mode");   // define the column headings (PLX-DAQ command)

  pinMode(12, INPUT);              // Mode switch Button
  pinMode(11, INPUT);              // Axis switch
  pinMode(A4, INPUT);              // Potentiometer for right-left movement and for up-down movement
  
  servo_updown.attach(9);          // Servo motor up-down movement
  servo_rightleft.attach(10);      // Servo motor right-left movement
}

void loop()
{
  //  pv_power();
  char  Mode;
  float volt = analogRead(A5)*5.0/1023;
  float voltage = 2*volt;                 // Volt=(R1/R1+R2)*Voltage / R1=R2=10Ohms  => voltage=2*volt)
  float current = voltage/20;             // I=voltage/(R1+R2) 
  float power  = voltage*current;
  // Serial.print("DATA,TIME,");             // PLX-DAQ command
  // Serial.print(voltage);                  // send the voltage to serial port
  // Serial.print(",");
  // Serial.print(current);                  // send the current to serial port
  // Serial.print(",");
  // Serial.print(power);                    // send the power to serial port
  // Serial.print(",");
  // Serial.print(axe);
  // Serial.print(",");
       
  buttonState1 = digitalRead(12);
  if (buttonState1 != prevButtonState1)  {
    if (buttonState1 == HIGH) {
      Serial.println(buttonState1);
      //Change mode and ligh up the correct  indicator  
      if (mode == 1) {
        mode = 0;
      } else {
        mode = 1;
      }
    }
  }
  prevButtonState1 = buttonState1;
  
  delay(50);                  // Wait for 50 millisecond(s)
  
  if (mode == 0) {
    Mode='M';
    Serial.println(Mode);     // send Mode "Manual" to serial port    
    manualsolartracker();
  } else {                    // mode automatic
    Mode = 'A';
    Serial.println(Mode);      
    automaticsolartracker();  // send Mode "Automatic" to serial port
  } 
}

void  automaticsolartracker(){
  
  // capturing analog values of each LDR
  topr= analogRead(ldrtopr);         // capturing analog value of top right LDR
  topl= analogRead(ldrtopl);         // capturing analog value of top left LDR
  botr= analogRead(ldrbotr);         // capturing analog value of bot right LDR
  botl= analogRead(ldrbotl);         // capturing analog value of bot left LDR

  // float voltage1 = topr * (5.0 / 1023.0);
  // float voltage2 = topl * (5.0 / 1023.0);
  // float voltage3 = botr * (5.0 / 1023.0);
  // float voltage4 = botl * (5.0 / 1023.0);

  // Print the voltages to the serial monitor
  // Serial.print("Voltage 1: ");
  // Serial.print(voltage1);
  // Serial.println(" V");
  
  // Serial.print("Voltage 2: ");
  // Serial.print(voltage2);
  // Serial.println(" V");
  
  // Serial.print("Voltage 3: ");
  // Serial.print(voltage3);
  // Serial.println(" V");
  
  // Serial.print("Voltage 4: ");
  // Serial.print(voltage4);
  // Serial.println(" V");

  // calculating average
  int avgtop = (topr + topl) / 2;     // average of top LDRs
  int avgbot = (botr + botl) / 2;     // average of bottom LDRs
  int avgleft = (topl + botl) / 2;    // average of left LDRs
  int avgright = (topr + botr) / 2;   // average of right LDRs

  // Serial.print(avgtop);
  // Serial.print("\n");
  // Serial.print(avgbot);
  // Serial.print("\n");
  // Serial.print(avgleft);
  // Serial.print("\n");
  // Serial.print(avgright);
  // Serial.print("\n");
  
  // Get the different  
  int diffelev = avgtop - avgbot + 20;      // Get the different average betwen LDRs top and LDRs bot
  int diffazi = avgleft - avgright - 10;    // Get the different average betwen LDRs right and LDRs left

  Serial.print(diffelev);
  Serial.print("\n");
  Serial.print(diffazi);
  Serial.print("\n");
  
  // left-right movement of  solar tracker
  if (abs(diffazi) >= threshold_value){         // Change position only if light difference is bigger than threshold_value
    if (diffazi > 0) {
      if (servo_rightleft.read() < 180) {
        servo_rightleft.write((servo_rightleft.read() + 3));
      }
    }
    if (diffazi < 0) {
      if (servo_rightleft.read() > 0) {
        servo_rightleft.write((servo_rightleft.read() - 3));
      }
    }
  }
           
  // up-down movement of solar tracker
  if (abs(diffelev) >= threshold_value){        // Change position only if light difference is bigger than thethreshold_value
    if (diffelev > 0) {
      if (servo_updown.read() < 100) {
        servo_updown.write((servo_updown.read() + 3));
      }
    }
    if (diffelev < 0) {
      if (servo_updown.read() > 10) {
        servo_updown.write((servo_updown.read() - 3));
      }
    }
  }       
}  

void manualsolartracker(){
  buttonState2  = digitalRead(11);
  if (buttonState2 != prevButtonState2) {
    if (buttonState2  == HIGH) {
      //Change mode and ligh up the correct indicator  
      if  (axe == 1) {
        axe = 0;
      } else {
        axe = 1;
      }
    }
  }
  prevButtonState2 = buttonState2;
  delay(50); // Wait for 50  millisecond(s)
  if (axe == 0) {     //control right-left movement
    servo_rightleft.write(map(analogRead(A4),  0, 1023, 0, 180));
  } else { // //control up-down movement
    servo_updown.write(map(analogRead(A4),  0, 1023, 10, 100));
  } 
}
