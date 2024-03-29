#include <SPI.h>
#include "RF24.h"
//#include <ArduinoJson.h>
int a = 0;
int b = 0;
int c = 0;
int LED1 = 39;
int LED2 = 41;
int LED3 = 43;
int blink_time = 500;

const int enbA = 2;
const int enbB = 3;

const int IN1 = 22;    //Right Motor (-)
const int IN2 = 23;    //Right Motor (+)
const int IN3 = 24;    //Left Motor (+)
const int IN4 = 25;    //Right Motor (-)

int RightSpd = 200;
int LeftSpd = 200;

int trigPin = 32;      // trig pin of HC-SR04
int echoPin = 33;     // Echo pin of HC-SR04
long duration, distance;
unsigned long time_dist = 0;

int data[3];  //Define packet for the direction (X axis and Y axis)
String content;
char state;

//Define object from RF24 library - 9 and 10 are a digital pin numbers to which signals CE and CSN are connected
RF24 radio(9, 10);

//Create a pipe addresses for the communicate
const uint64_t pipe = 0xE8E8F0F0E1LL;

unsigned long time_led = 0;

void setup() {
  pinMode(enbA, OUTPUT);
  pinMode(enbB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(trigPin, OUTPUT);         // set trig pin as output
  pinMode(echoPin, INPUT);          //set echo pin as input to capture reflected waves
data[2]=5;
  Serial.begin(9600);
  radio.begin();                    //Start the nRF24 communicate
  radio.openReadingPipe(1, pipe);   //Sets the address of the transmitter to which the program will receive data.
  radio.startListening();             //analogWrite(enbA, RightSpd);
  analogWrite(enbB, LeftSpd);
}

void Triger()
{
      digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);     // send waves for 10 us
      delayMicroseconds(10);
      duration = pulseIn(echoPin, HIGH); // receive reflected waves
      distance = duration / 58.138;   // convert to distance
 }
 void standbyblink()
 {
  if (millis() > time_led && millis() < time_led + blink_time) {
    
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);     
}
if (millis() > time_led + blink_time && millis() < time_led + 2*blink_time) {
   
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH);
}
if (millis() > time_led + 2*blink_time && millis() < time_led + 3*blink_time) {
  
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW); 
}
if (millis() > time_led + 3*blink_time && millis() < time_led + 4*blink_time) {
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH); 

}
if (millis() > time_led + 4*blink_time && millis() < time_led + 5*blink_time) {
  
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, HIGH); 
}
if (millis() > time_led + 5*blink_time && millis() < time_led + 6*blink_time) {
  
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW); 
}       
if (millis() > time_led + 7*blink_time){
   time_led = millis();
}
  }
 
void loop() {
  a = digitalRead(8);
  b = digitalRead(7);
  c = digitalRead(6);

  if (radio.available()) {
    radio.read(data, sizeof(data));
  }

  Serial.print("MODE");
  Serial.println(data[2]);
 // if (data[2]==5)
  if (!radio.available())
  {
standbyblink();
}

  switch (data[2]) {
    case 0:
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        if (!radio.available())
  {
standbyblink();
}
  Serial.print("x=");
  Serial.print(data[0]);
  Serial.print("y=");
  Serial.print(data[1]);
    
      if (data[1] < 130) {
        //forward
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }

      if (data[1] > 160) {
        //backward
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      }

      if (data[0] > 360) {
        //left
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      }

      if (data[0] < 330) {
        //right
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }

      if (data[0] > 330 && data[0] < 360 && data[1] > 130 && data[1] < 160) {
        //stop car
        analogWrite(enbA, 0);
        analogWrite(enbB, 0);
      }
      break;

    case 1:
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH);

      if (a == LOW && b == LOW && c == HIGH) {
        //forward
        Serial.print("moving forward");
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }

      if (a == LOW && b == HIGH && c == LOW) {
        //backward
        Serial.print("moving backward");
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      }

      if (a == LOW && b == HIGH && c == HIGH) {
        //left
        Serial.print("left");
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      }

      if (a == HIGH && b == LOW && c == LOW) {
        //right
        Serial.print("right");
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }

      if (a == HIGH && b == LOW && c == HIGH) {
        //stop car
      //  Serial.print("stop");
        analogWrite(enbA, 0);
        analogWrite(enbB, 0);
      }
      break;
    case 2:
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);

 Triger();
      delay(10);
      
      if (distance > 19)
      {
        //forward
        analogWrite(enbA, 120);
        analogWrite(enbB, 120);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }

      if (distance < 18)
      {
        //stop car
        analogWrite(enbA, 0);
        analogWrite(enbB, 0);
        delay(500);
        //backward
        analogWrite(enbA, 150);
        analogWrite(enbB, 150);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        delay(500);
        //stop car
        analogWrite(enbA, 0);
        analogWrite(enbB, 0);
        delay(100);

        analogWrite(enbA, 120);
        analogWrite(enbB, 120);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
        delay(500);
      }
      break;
  }
}
