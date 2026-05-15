#include <WiFi.h>
#include <PubSubClient.h>
#include <WiFiClientSecure.h>
#include <ESP32Servo.h>

/*const char* ssid = "Primavera26";
const char* password = "Ib3r02026pR1m";*/

/*const char* ssid = "iPhone";
const char* password = "marcoqwe1";*/

const char* ssid = "Totalplay-38AB_EXT";
const char* password = "38AB408C5wN8PyuJ";
const char* mqtt_server = "e4f0d50b37b04ea79745872566f605ff.s1.eu.hivemq.cloud";
int pinServo = 18;
int pinServo_q2 = 19;
int pinServo_q3 = 21;

WiFiClientSecure espClient;
PubSubClient client(espClient);

Servo miServo;
Servo miServo_q2;
Servo miServo_q3;

void callback(char* topic, byte* payload, unsigned int length) {
  //String message = "";

  char msg[length+1];
  memcpy(msg,payload,length);
  msg[length] = '\0';
  int val = atoi(msg);

  /*for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }*/

  //int val = message.toInt();

  if(strcmp(topic,"q1")==0){

    Serial.print(topic);
    Serial.print(": ");
    Serial.println(val);
    miServo.write(val);
    
  }else if(strcmp(topic,"q2")==0){

    Serial.print(topic);
    Serial.print(": ");
    Serial.println(val);
    miServo_q2.write(val);

  }else if(strcmp(topic,"q3")==0){

    Serial.print(topic);
    Serial.print(": ");
    Serial.println(val);
    miServo_q3.write(val);
  }

}

void reconnect() {
  
  static unsigned long lastTry = 0;

  if(millis() - lastTry > 2000 && !client.connected()){

    lastTry = millis();

    Serial.println("Intentando conectar MQTT...");

    if (client.connect("ESP32Client","MarcoA","HATeR3__")) {
      Serial.println("Conectado");
      client.subscribe("q1");
      client.subscribe("q2");
      client.subscribe("q3");
     }else{
      Serial.println("Error.");
      Serial.print(client.state());
      delay(2000);
    }
    
  }
}

void setup() {
  Serial.begin(115200);

  miServo.attach(pinServo);
  miServo_q2.attach(pinServo_q2);
  miServo_q3.attach(pinServo_q3);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.println("WiFi conectado");

  espClient.setInsecure(); //Para pruebas (no valida certificado)
  client.setServer(mqtt_server, 8883);
  client.setCallback(callback);
}

void loop() {

  if (!client.connected()) reconnect();
  client.loop();


}