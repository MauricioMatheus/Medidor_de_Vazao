#include <WiFi.h>
#include <PubSubClient.h>

// Configurações do Wi-Fi
const char* ssid = "**********";      // Substituir com a rede Wi-Fi
const char* password = "************"; // Substituir com a senha Wi-Fi

// Configurações do Broker MQTT
const char* mqtt_server = "mqtt.eclipseprojects.io"; 
const int mqtt_port = 1883;                   
const char* mqtt_topic = "agua/vazao";        

WiFiClient espClient;                // Cliente Wi-Fi
PubSubClient client(espClient);      // Cliente MQTT

// Função para conectar ao Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando ao WiFi ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);  // Conectando-se à rede Wi-Fi
  while (WiFi.status() != WL_CONNECTED) {  // Espera até a conexão ser estabelecida
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado ao Wi-Fi!");
}

// Função para conectar ao Broker MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando conectar ao MQTT...");
    if (client.connect("ESP32PublisherSubscriber")) {  // Conecta com o nome 'ESP32PublisherSubscriber'
      Serial.println("conectado!");
      client.subscribe(mqtt_topic);  // Inscreve-se no tópico 'agua/vazao'
    } else {
      Serial.print("Falha. Tentando novamente em 5 segundos.");
      delay(5000);
    }
  }
}

// Função de callback chamada sempre que uma mensagem for recebida (Conexão Publisher-Subscriber)
void callback(char* topic, byte* payload, unsigned int length) {
  String message = "";
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];  // Converte o payload de byte para string
  }
  Serial.print("--------------------Mensagem recebida: ");
  Serial.println(message);  // Exibe no Serial Monitor a mensagem recebida
}

void setup() {
  Serial.begin(115200); 
  setup_wifi();  // Conecta-se ao Wi-Fi
  client.setServer(mqtt_server, mqtt_port);  // Configura o Broker MQTT
  client.setCallback(callback);  // Definindo a função callback para receber as mensagens
}

void loop() {
  if (!client.connected()) {  // Verifica se está conectado ao Broker MQTT
    reconnect();  
  }
  client.loop();  

  // Simulação do valor de vazão de água
  float vazao = random(0, 100);  // Gera um valor aleatório de 0 a 100
  String payload = String(vazao); // Converte o valor para string
  client.publish(mqtt_topic, payload.c_str());  // Publica no tópico

  Serial.print("Vazão simulada: ");
  Serial.println(vazao);  // Imprime no Serial Monitor o valor da vazão simulada

  delay(10000);
}