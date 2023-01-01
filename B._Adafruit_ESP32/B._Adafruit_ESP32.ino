#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "DHT.h"

#define DHTPIN 4
#define DHTTYPE DHT11
#define WLAN_SSID       "Paperglass"
#define WLAN_PASS       "raonopassworde"
#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "rickysambora55"
#define AIO_KEY         "aio_iCiq32UPY76Xw48zqG036D7USMIt"

const int ledPin = 16;
uint32_t x = 0;
float temperature;
float humidity;

DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;     // Create an ESP8266 WiFiClient class to connect to the MQTT server.
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);        // Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Subscribe led = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/led");
Adafruit_MQTT_Publish dhtTemp = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/dhtTemp");
Adafruit_MQTT_Publish dhtHum = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/dhtHum");

void setup() {

  Serial.begin(115200);
  delay(10);
  pinMode(ledPin, OUTPUT);
  dht.begin();

  // Connect to WiFi access point.
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());
  mqtt.subscribe(&led);
}

void loop() {
  MQTT_connect();

  // Connect or reconnect to WiFi
  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("Attempting to connect");
    while (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(WLAN_SSID, WLAN_PASS);
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  // Get a new temperature reading
  humidity = dht.readHumidity();
  temperature = dht.readTemperature();
  if (isnan(humidity) || isnan(temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) {
    if (subscription == &led) {
      Serial.print(F("Got: "));
      Serial.println((char *)led.lastread);
      if (!strcmp((char*) led.lastread, "1"))
      {
        digitalWrite(ledPin, HIGH);
      }
      else
      {
        digitalWrite(ledPin, LOW);
      }
    }
  }

  if (! dhtTemp.publish(temperature)) {
    Serial.println(F("Failed to upload temperature"));
  } else {
    Serial.print("Temperature (ºC): ");
    Serial.print(temperature);
  }
  if (! dhtHum.publish(humidity)) {
    Serial.println(F("Failed to upload humidity"));
  } else {
    Serial.print(" Humidity (%): ");
    Serial.println(humidity);
  }
  delay(500);
}

void MQTT_connect() {

  int8_t ret;
  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }
  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
