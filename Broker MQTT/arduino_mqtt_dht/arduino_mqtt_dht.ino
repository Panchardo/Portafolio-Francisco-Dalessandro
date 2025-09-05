#include "DHT.h"
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <PubSubClient.h>
#include <Arduino.h>

// Configuración de la red Ethernet
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 100, 120);  // Dirección IP del Arduino
IPAddress mqttserver(192, 168, 100, 27);  // Dirección IP del servidor MQTT
EthernetClient ethClient;

void callback(char* topic, byte* payload, unsigned int length) {
  // handle message arrived
}


PubSubClient mqttClient(mqttserver, 1883, callback, ethClient);

// Configuración del sensor DHT
#define DHTPIN 9  // Pin al que está conectado el sensor
#define DHTTYPE DHT11 // Tipo de sensor DHT
#define DHTOUT 8
DHT dht(DHTPIN,DHTOUT, DHTTYPE);

float temperature;
float humidity;

char *dtostrf (double val, signed char width, unsigned char prec, char *sout) {
  char fmt[20];
  sprintf(fmt, "%%%d.%df", width, prec);
  sprintf(sout, fmt, val);
  return sout;
}

void setup() {
  Serial.begin(9600);
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
  }
  mqttClient.setServer(mqttserver, 1883); // Puerto MQTT predeterminado
  // Nombre de usuario y contraseña para la autenticación MQTT
  dht.begin(); // Inicializa el sensor DHT
}

void loop() {

  if (!mqttClient.connected()) {
    Serial.print("Connecting to MQTT...");
    if (mqttClient.connect("ArduinoClient", "galileo", "galileo123")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
      return;
    }
  }
  // Lee la temperatura y humedad del sensor DHT
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  // Verifica si la lectura es válida
  if (!isnan(temperature) && !isnan(humidity)) {
    // Convierte la temperatura a cadena
    char tempStr[6];
    dtostrf(temperature, 5, 2, tempStr);
    
    // Publica la temperatura en el topic MQTT
    mqttClient.publish("sensor/temperatura", tempStr);
  }

  delay(2000);  // Espera 5 segundos antes de la próxima lectura
}
