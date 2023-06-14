#include ".\config.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

#define led D8

bool stateLedApi;

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
}

void loop() {
  WiFiClientSecure client;
  HTTPClient http;
  client.setInsecure(); // desativando o ssl
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
  delay(10);
}
