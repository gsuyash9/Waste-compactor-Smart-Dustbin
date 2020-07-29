#include <GSM.h>

const int FSR_PIN = A0;
const int motorPin1= 3;
const int motorPin2=4;
const int pingPin = 7; // Trigger Pin of Ultrasonic Sensor
const int echoPin = 6; // Echo Pin of Ultrasonic Sensor
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 3230.0; // Measured resistance of 3.3k resistor

void setup()
{
  
  Serial.begin(9600);  
   pinMode(FSR_PIN, INPUT);
   pinMode(echoPin, INPUT);
   pinMode(motorPin1, OUTPUT);
   pinMode(motorPin2, OUTPUT);
   pinMode(pingPin, OUTPUT);
   
}
void loop()
{
   long duration, inches, cm;
   digitalWrite(pingPin, LOW);
   delayMicroseconds(2);
   digitalWrite(pingPin, HIGH);
   delayMicroseconds(10);
   digitalWrite(pingPin, LOW);
   duration = pulseIn(echoPin, HIGH);
   //inches = microsecondsToInches(duration);
   cm = microsecondsToCentimeters(duration);
  if(cm<=20 && cm>5){
     compress();
  }
  if(cm<=5)
  {
    sendSms(); 
  }
 
}

long microsecondsToCentimeters(long microseconds) {
   return microseconds / 29 / 2;
}
void compress()
{
  float force;
   digitalWrite(motorPin1, HIGH);
   digitalWrite(motorPin2, LOW);
   
   while(motorPin1 == HIGH && motorPin2 == LOW )
   {
      force=maxForce();
      if(force>60)
      {
        while(force>0)
        {
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, HIGH);
          force--;
        }
          digitalWrite(motorPin1, LOW);
          digitalWrite(motorPin2, LOW);
        break;
      }
      delay(100);
   }
}
void sendSms()
{
   Serial.print("\r");
  delay(1000);                  
  Serial.print("AT+CMGF=1\r");    
  delay(1000);
  /*Replace XXXXXXXXXX to 10 digit mobile number*/
  Serial.print("AT+CMGS=\"+91XXXXXXXXXX\"\r");    
  delay(1000);
  //The text of the message to be sent.
  Serial.print("Alert! bin is full please collect it from address XYX");   
  delay(1000);
  Serial.write(0x1A);
  delay(1000);
}
float maxForce()
{
   int fsrADC = analogRead(FSR_PIN);
 
  float force=0.0;
  if (fsrADC != 0) 
  {
    
    float fsrV = fsrADC * VCC / 1023.0;
    
    float fsrR = R_DIV * (VCC / fsrV - 1.0);
   // Serial.println("Resistance: " + String(fsrR) + " ohms");
    // Guesstimate force based on slopes in figure 3 of
    // FSR datasheet:
    
    float fsrG = 1.0 / fsrR; // Calculate conductance
      if(force
      force =  fsrG / 0.000642857;
  }
    return force;
}
