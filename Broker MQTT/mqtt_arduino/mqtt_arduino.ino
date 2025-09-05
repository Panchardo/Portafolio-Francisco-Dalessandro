#include <SPI.h>
#include <Ethernet.h> // Si estás utilizando Ethernet
//#include <WiFi.h>   // Si estás utilizando WiFi
#include <PubSubClient.h>

// Configuración de red
const char* ssid = "TuSSID"; // Cambia por tu SSID si estás usando WiFi
const char* password = "TuContraseña"; // Cambia por tu contraseña si estás usando WiFi
const char* mqttServer = "tu.direccion.broker"; // Cambia por la dirección IP o URL de tu broker MQTT
const int mqttPort = 1883; // Cambia por el puerto de tu broker MQTT
const char* mqttUser = "usuario"; // Cambia por el usuario de tu broker MQTT si es necesario
const char* mqttPassword = "contraseña"; // Cambia por la contraseña de tu broker MQTT si es necesario

EthernetClient ethClient; // Si estás utilizando Ethernet
//WiFiClient wifiClient;  // Si estás utilizando WiFi
PubSubClient client(ethClient); // Si estás utilizando Ethernet
//PubSubClient client(wifiClient);  // Si estás utilizando WiFi

void setup() {
  Serial.begin(9600);
  // Conexión a la red
  // WiFi.begin(ssid, password);  // Si estás utilizando WiFi
  // while (WiFi.status() != WL_CONNECTED) {
  //   delay(1000);
  //   Serial.println("Conectando a WiFi...");
  // }
  // Serial.println("Conexión WiFi establecida");
  
  // Conexión MQTT
  client.setServer(mqttServer, mqttPort);
  //client.setCallback(callback); // Si necesitas un callback para procesar mensajes entrantes
  // while (!client.connected()) {
  //   if (client.connect("GalileoClient", mqttUser, mqttPassword)) {
  //     Serial.println("Conexión MQTT establecida");
  //   } else {
  //     Serial.println("Error al conectar al broker MQTT, intentando de nuevo en 5 segundos...");
  //     delay(5000);
  //   }
  // }
  // client.subscribe("topic"); // Si necesitas suscribirte a un tema
}

void loop() {
  // Reconnect if connection is lost
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Publicar un mensaje cada cierto tiempo
  client.publish("topic", "Hello from Galileo");
  delay(5000); // Espera 5 segundos antes de publicar el siguiente mensaje
}

void reconnect() {
  // Intenta conectar al broker MQTT
  while (!client.connected()) {
    Serial.println("Conectando al broker MQTT...");
    if (client.connect("GalileoClient", mqttUser, mqttPassword)) {
      Serial.println("Conexión MQTT establecida");
      //client.subscribe("topic"); // Si necesitas suscribirte a un tema después de la reconexión
    } else {
      Serial.println("Error al conectar al broker MQTT, intentando de nuevo en 5 segundos...");
      delay(5000);
    }
  }
}
