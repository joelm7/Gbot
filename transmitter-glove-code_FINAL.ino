#include <SPI.h>
#include "RF24.h"
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
const int Buttonpin = 5;
int LED1 = 8;
int LED2 = 7;
int LED3 = 6;
int MODE = 0;
int stateButton;
long time = 0;
long debounce = 200;
MPU6050 mpu;
int16_t ax, ay, az;
int16_t gx, gy, gz;

//Define packet for the direction (X axis and Y axis)
int data[3];

//9 and 10 are a digital pin numbers to which signals CE and CSN are connected.
RF24 radio(9, 10);

//Create a pipe addresses for the communicate
const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup(void) {
  Serial.begin(9600);
  pinMode(Buttonpin, INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  digitalWrite(LED1, HIGH);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3, LOW);
  Wire.begin();
  mpu.initialize();              //Initialize the MPU object
  radio.begin();                 //Start the nRF24 communicate
  radio.openWritingPipe(pipe);   //Sets the address of the receiver to which the program will send data.
}

void loop(void) {

  stateButton = digitalRead(Buttonpin);
  if (stateButton == HIGH && millis() - time > debounce) {
    MODE++;
    if (MODE == 3)
    {
      MODE = 0;
    }
    switch (MODE) {
      case 0:
        digitalWrite(LED1, HIGH);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, LOW);
        break;
      case 1:
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, HIGH);
        digitalWrite(LED3, LOW);
        break;
      case 2:
        digitalWrite(LED1, LOW);
        digitalWrite(LED2, LOW);
        digitalWrite(LED3, HIGH);
        break;
    }
    /*
        if(MODE == 0){
          MODE = 1;
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, HIGH);
          digitalWrite(LED3, LOW);
        } else if(MODE == 2){
           MODE = 1;
          digitalWrite(LED1, LOW);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, HIGH);
        } else if(MODE == 3){
          MODE = 1;
          digitalWrite(LED1, HIGH);
          digitalWrite(LED2, LOW);
          digitalWrite(LED3, LOW);
        }
    */
    time = millis();
  }

  mpu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);

  Serial.print("  x coordinate: ");
  Serial.print(data[0]);
  Serial.print("  y coordinate: ");
  Serial.print(data[1]);
  Serial.print("  mode: ");
  Serial.println(data[2]);


  data[0] = map(ax, -17000, 17000, 300, 400 ); //Send X axis data
  data[1] = map(ay, -17000, 17000, 100, 200);  //Send Y axis data
  data[2] = MODE;
  
  radio.write(data, sizeof(data));

}
