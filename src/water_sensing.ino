#define R 8
#define G 9
#define B 10

void setup(){
  Serial.begin(9600);     // Communication started with  9600 baud
  pinMode(R,OUTPUT);   
  pinMode(G,OUTPUT);   
  pinMode(B,OUTPUT);
}

void loop(){

  int sensor=analogRead(A0); // Incoming analog  signal read and appointed sensor
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
