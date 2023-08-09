#include ".\config.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define led D8

//Variaveis led
bool stateLedApi;
unsigned long lastTimeLed = 0;
unsigned long timerDelayLed = 10;

//Sensor ultrassonico
unsigned long lastTimeSensor = 0;
unsigned long timerDelaySensor = 50;

const int trigPin = D6;
const int echoPin = D5;

//definindo a velocidade do com em cm/uS
#define SOUND_VELOCITY 0.034

long duration;
float distanceCm;
float distanceInch;

void setup() {
  //Conectando no wifi
  Serial.begin(115200);
  delay(10);
  Serial.println();
  Serial.print("Conectando no Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  
  // put your setup code here, to run once:
  pinMode(led,OUTPUT);

  //Criar um Client para o site
  WiFiClientSecure client;
  HTTPClient http;

  client.setInsecure(); // desativando o ssl
  http.begin(client,"https://iot-turma-quarta-feira.onrender.com/");
  int httpCode = http.GET();
  if(httpCode > 0){
    String payload = http.getString();
    Serial.println(payload);
  }else{
    Serial.print("Erro ao tentar pegar dados da Api");
  }

  pinMode(trigPin,OUTPUT);
  pinMode(echoPin,INPUT);
}

void loop() {
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure(); // desativando o ssl
  //se o tempo atual(millis) menos a ultima marcação(lastTime) for menor que tempo escolhido
  //eu faço a tarefa e reinicio a marcação passando o tempo atual
  if((millis()-lastTimeLed > timerDelayLed)){
    //faço algo
    http.begin(client,"https://iot-turma-quarta-feira.onrender.com/led/state-led");
    int httpCode = http.GET();
    String payload = http.getString();
    Serial.print(payload);
  
    StaticJsonDocument<54> doc;
  
    DeserializationError error = deserializeJson(doc,payload);
    if(error){
      Serial.print(F("deu erro ao transformar em documento o nosso json da api"));
      Serial.println(error.f_str());
    }
    bool state = doc["state"];
    Serial.println(state);
    digitalWrite(led,state);
    lastTimeLed=millis();
  }
  //Lendo o sinal
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);

  duration = pulseIn(echoPin,HIGH);
  distanceCm = duration * SOUND_VELOCITY/2;

  if((millis() - lastTimeSensor > timerDelaySensor)){
    http.begin(client,"https://iot-turma-quarta-feira.onrender.com/sensor/acesso");
    http.addHeader("Content-Type","application/json");
    StaticJsonDocument<100> SensorDocument;
    SensorDocument["nome"] = "Porta De Entrada";
    SensorDocument["value"] = distanceCm;
    if(distanceCm < 150 and distanceCm > 50){
      char bufferDoJsonEmString[100];
      serializeJson(SensorDocument,bufferDoJsonEmString);
      int httpReponseCode = http.POST(bufferDoJsonEmString);
      String ResponseServer = http.getString();
      Serial.println(ResponseServer);
    }
    lastTimeSensor = millis();
  }
}
