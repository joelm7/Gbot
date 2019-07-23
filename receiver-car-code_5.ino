#include <SPI.h>
#include "RF24.h"
//#include <ArduinoJson.h>
#include <Servo.h>          //Servo motor library. This is standard library
#include <NewPing.h>        //Ultrasonic sensor function library. You must install this library

int a = 0;
int b = 0;
int c = 0;
int LED1 = 39;        //led Red pin
int LED2 = 41;        //led Green pin
int LED3 = 43;        //led blue pin
int blink_time = 500;
int standby = 3000;
  int s[3]={0,0,0};

const int enbA = 2;     //Right Motor (enable)
const int enbB = 3;     //Left Motor (enable)

const int IN1 = 22;    //Right Motor (-)
const int IN2 = 23;    //Right Motor (+)
const int IN3 = 24;    //Left Motor (+)
const int IN4 = 25;    //Right Motor (-)

int RightSpd = 200;
int LeftSpd = 200;

int trigPin = 32;     // trig pin of HC-SR04
int echoPin = 33;     // Echo pin of HC-SR04
//------------------
#define maximum_distance 200
boolean goesForward = false;
//int distance = 100;
NewPing sonar(trigPin, echoPin, 200); //sensor function
Servo servo_motor; //servo name
//----------------

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
unsigned long time_count = 0;
int sampling_time = 500;

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
  data[2] = 5;
  Serial.begin(9600);
  radio.begin();                     //Start the nRF24 communicate
  radio.openReadingPipe(1, pipe);    //Sets the address of the transmitter to which the program will receive data.
  radio.startListening();            //analogWrite(enbA, RightSpd);
  analogWrite(enbB, LeftSpd);
  //--------------------------
  servo_motor.attach(5); //our servo pin
  servo_motor.write(115);
 // delay(2000);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
  distance = readPing();
  delay(100);
}
/*
void Triger()
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);        // send waves for 10 us
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH);  // receive reflected waves
  distance = duration / 58.138;       // convert to distance
}
*/


void loop() {
  a = digitalRead(8);
  b = digitalRead(7);
  c = digitalRead(6);

  if (radio.available()) {
    radio.read(data, sizeof(data));
  }

  Serial.print("MODE");
  Serial.println(data[2]);
  
  if (data[2]==5)
//  if (!radio.available())
  {
    standbyblink();
  }

  switch (data[2]) {
    case 0:
      Serial.print("x=");
      Serial.print(data[0]);
      Serial.print("y=");
      Serial.print(data[1]);
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
  // 

  if (millis() > time_count && millis() < time_count + sampling_time) {    
    if (!radio.available()){
  s[0]=1;
  }}
  if (millis() > time_count + sampling_time && millis() < time_count + 2*sampling_time) {
    if (!radio.available()){
  s[1]=1;
    }}
  if (millis() > time_count + 2*sampling_time && millis() < time_count + 3*sampling_time) {
    if (!radio.available()){
  s[2]=1;
  }}
if (s[0]==1 && s[1]==1 && s[2]==1) data[2]=5;

if (millis() > time_count + 3*sampling_time){
  time_count = millis();
s[0]=0;
s[1]=0;
s[2]=0;
}


/*

  if (time_count < 1000) 
  {    
    if (!radio.available())
     {
      
     }
  }
    if (time_count > 1000 && time_count < 2000) 
  {    

  }
      if (time_count > 2000 && time_count < 3000) 
  {    
    if (!radio.available())
     {
      s[2]=1;
   
     }
  }//&& s[4]==1) data[2]=5
     if (s[0]==1 && s[1]==1 && s[2]==1 && s[3]==1) ;
     i++;
     if (i>5) i=0;
  */    

      if (data[1] < 120) {
        //forward
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }

      if (data[1] > 180) {
        //backward
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      }

      if (data[0] > 370) {
        //left
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      }
      if (data[0] < 320) {
        //right
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }
   /*   if (data[0] > 370 && data[0] < 120) {
        //front left
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
      }
      if (data[0] > 370 && data[1] > 180) {
        //Back left
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
      }
      if (data[0] < 320 && data[0] < 120) {
        //front right
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);
      }
      if (data[0] < 320 && data[1] > 180) {
        //back right
        analogWrite(enbA, RightSpd);
        analogWrite(enbB, LeftSpd);
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
      }
*/

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
        Serial.print("stop");
        analogWrite(enbA, 0);
        analogWrite(enbB, 0);
      }
      break;
    case 2:
    distance = readPing();
      digitalWrite(LED1, LOW);
      digitalWrite(LED2, HIGH);
      digitalWrite(LED3, LOW);
        analogWrite(enbA, 120);
        analogWrite(enbB, 120);
  int distanceRight = 0;
  int distanceLeft = 0;
  delay(50);

  if (distance <= 20){
    moveStop();
    delay(300);
    moveBackward();
    delay(400);
    moveStop();
    delay(300);
    distanceRight = lookRight();
    delay(300);
    distanceLeft = lookLeft();
    delay(300);

    if (distance >= distanceLeft){
      turnRight();
      moveStop();
    }
    else{
      turnLeft();
      moveStop();
    }
  }
  else{
    moveForward(); 
  }
    distance = readPing();
 /*     Triger();
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
      }*/
      break;
  }
}

int lookRight(){  
  servo_motor.write(50);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
}

int lookLeft(){
  servo_motor.write(170);
  delay(500);
  int distance = readPing();
  delay(100);
  servo_motor.write(115);
  return distance;
  delay(100);
}

int readPing(){
  delay(70);
  int cm = sonar.ping_cm();
  if (cm==0){
    cm=250;
  }
  return cm;
}

void moveStop(){
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward(){

  if(!goesForward){

    goesForward=true;
    
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW); 
  }
}

void moveBackward(){

  goesForward=false;

        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
  
}

void turnLeft(){

        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
  
  delay(500);
  
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
  
}

void turnRight(){
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, HIGH);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);

  delay(500);
  
        digitalWrite(IN1, HIGH);
        digitalWrite(IN2, LOW);
        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
}
void standbyblink()
{
  if (millis() > time_led && millis() < time_led + blink_time) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
  }
  if (millis() > time_led + blink_time && millis() < time_led + 2 * blink_time) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
  }
  if (millis() > time_led + 2 * blink_time && millis() < time_led + 3 * blink_time) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
  }
  if (millis() > time_led + 3 * blink_time && millis() < time_led + 4 * blink_time) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, HIGH);
  }
  if (millis() > time_led + 4 * blink_time && millis() < time_led + 5 * blink_time) {
    digitalWrite(LED1, LOW);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, HIGH);
  }
  if (millis() > time_led + 5 * blink_time && millis() < time_led + 6 * blink_time) {
    digitalWrite(LED1, HIGH);
    digitalWrite(LED2, HIGH);
    digitalWrite(LED3, LOW);
  }
  if (millis() > time_led + 7 * blink_time) {
    time_led = millis();
  }
}
