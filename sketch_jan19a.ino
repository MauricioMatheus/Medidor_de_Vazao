#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>


#include <PubSubClient.h>

#define pinBotao1 27 //D27

//Wifi

const char* SSID = "*********"; // Nome da rede wifi
const char* PASSWORD = "*******"; //Senha da rede
WiFiClient wifiClient;

//MQTT Server

const char* BROKER_MQTT = "iot.eclipse.org";
int BROKER_PORT = 1883;

#define ID_MQTT "ESP32_Mauricio"
#define TOPIC_PUBLISH "ESP32_Botao"
PubSubClient MQTT(wifiClient) //Instanciando o Cliente MQT passando o objeto 


void mantemConexoes(); //Garante que as conexões com WiFi e MQTT Broker se mantenham ativas
void conectaWifi();
void conectaMQTT();
void enviaPacote();

void setup() {
  pinMode(pinBotao1, INPUT_PULLUP);

  Serial.begin(115200);

  conectaWifi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT); //Conectando com o Broker

}

void loop() {
  mantemConexoes();
  enviaValores();
  MQTT.loop();
}

void mantemconexoes() {
  if (!MQTT.connected()){
    conectaMQTT(); // Se não há conexão com o Wifi, a conexão é refeita
  }
}

void conectaWifi(){
  if (Wifi.status() == WL_CONNECTED){
    return;
  }
  Serial.print("Conectando-se a rede: ");
  Serial.print(SSID);
  Serial.println(" aguarde!");

  WiFi.begin(SSID, PASSWORD); // Conecta na rede Wi-fi
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso, na rede: ");
  Serial.print(SSID);
  Serial.print(" IP obtido: ")
  Serial.println(WiFi.localIP());
}

void conectaMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Conectando ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao Broker com sucesso!");
    }
    else {
      Serial.println("Não foi possível se conectar ao Broker.");
      Serial.println("Nova tentativa de conexão em 10s");
      delay(10000);
    }
  }
}
