#include <ESP8266WiFi.h>
#include<FirebaseArduino.h>
#include <dht.h>
#include <ArduinoJson.h>
int trigPin = D1;      // trig pin of HC-SR04
int echoPin = D2;     // Echo pin of HC-SR04
int f;
long duration, distance;

#define FIREBASE_HOST "gbot-431d9.firebaseio.com"                     //Your Firebase Project URL goes here without "http:" , "\" and "/"
#define FIREBASE_AUTH "J2KwVbpwhc43c70gyNa4gKdRZirmIX45mCK3B5gw"       //Your Firebase Database Secret goes here
#define WIFI_SSID "AndroidAP"                                       //your WiFi SSID for which yout NodeMCU connects
#define WIFI_PASSWORD "mnmnmnmn"                    //Password of your wifi network 

dht DHT; // Data Pin of DHT 11 , for NodeMCU D5 GPIO no. is 14
unsigned long time_dht = 0;
unsigned long time_dist = 0;
String motion;

void setup()
{
  Serial.begin(9600);
//  NodeMCU.begin(9600);
  pinMode(D1, INPUT);
  pinMode(D2, OUTPUT);
  pinMode(D6, OUTPUT);
  pinMode(D7, OUTPUT);
  pinMode(D8, OUTPUT);
  pinMode(trigPin, OUTPUT);         // set trig pin as output
  pinMode(echoPin, INPUT);          //set echo pin as input to capture reflected waves

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected:");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
void firebasereconnect()
{
  Serial.println("Trying to reconnect");
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
}
/*
void dist() {
  if (millis() > time_dist + 900) {
      time_dist = millis();
    StaticJsonBuffer<1000> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(NodeMCU);
    if (root == JsonObject::invalid())
      return;
    Serial.println("JSON received and parsed");
    root.prettyPrintTo(Serial);
    int data1 = root["data1"];
  
    Serial.print("distance: ");
    Serial.println(data1);
    Firebase.setInt ("Distance", data1);
  }
}
*/

void loop()
{
  if (Firebase.failed())
  {
    Serial.print("setting number failed:");
    Serial.println(Firebase.error());
    firebasereconnect();
    return;
  }
if (millis() > time_dist + 1000) {
    time_dist = millis();
        digitalWrite(trigPin, LOW);
      delayMicroseconds(2);
      digitalWrite(trigPin, HIGH);     // send waves for 10 us
      delayMicroseconds(10);
      duration = pulseIn(echoPin, HIGH); // receive reflected waves
      distance = duration / 58.138;   // convert to 
     Serial.print("distance: ");
    Serial.println(distance);
    Firebase.setInt ("Distance", distance);
}
//dist();

  /*
    if (millis() > time_dist + 2000) {
    time_dist = millis();
    int incoming = NodeMCU.available();
    Serial.print(incoming);
    if (incoming > 0)
    {
    distance = NodeMCU.parseInt(); // reads integers as integer rather than ASCI,anything else returns 0NodeMCU.parseInt()
    Serial.print("distance: ");
    Serial.println(distance);
    Firebase.setInt ("Distance", distance);

    String content = "";
    char character;
    while (NodeMCU.available()) {
    character = NodeMCU.read();
    content.concat(character);
    distance =(content.toInt());
    // f = (content.toInt());
    }  */
  if (millis() > time_dht + 5000) {
    time_dht = millis();
    DHT.read11(14);
    float h = DHT.humidity;
    float t = DHT.temperature;  // Reading temperature as Celsius (the default)
    Firebase.setFloat ("Temp", t);
    Firebase.setFloat ("Humidity", h);
    Firebase.setInt ("Distance", distance);
    Serial.println(h);
    Serial.println(t);
  }
  motion = Firebase.getString("motion");
  f = (motion.toInt());
  Serial.println(f);

  if (f == 1) {
    digitalWrite(D6, LOW);
    digitalWrite(D7, LOW);
    digitalWrite(D8, HIGH);
  }
  if (f == 2) {
    digitalWrite(D6, LOW);
    digitalWrite(D7, HIGH);
    digitalWrite(D8, LOW);
  }
  if (f == 3) {
    digitalWrite(D6, LOW);
    digitalWrite(D7, HIGH);
    digitalWrite(D8, HIGH);
  }
  if (f == 4) {
    digitalWrite(D6, HIGH);
    digitalWrite(D7, LOW);
    digitalWrite(D8, LOW);
  }
  if (f == 0) {
    digitalWrite(D6, HIGH);
    digitalWrite(D7, LOW);
    digitalWrite(D8, HIGH);
  }
}
