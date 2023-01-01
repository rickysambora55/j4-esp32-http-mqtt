#include <WiFi.h>
#include "ThingSpeak.h"
#include "DHT.h"
#define DHTPIN 4
#define DHTTYPE DHT22

const char* ssid = "Paperglass";   // your network SSID (name)
const char* password = "raonopassworde";   // your network password

WiFiClient  client;
DHT dht(DHTPIN, DHTTYPE);

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "43HEFUYW8U6LY14V";

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

// Variable to hold readings
float temperature;
float humidity;

void setup() {
  Serial.begin(115200);  //Initialize serial
  dht.begin();  //Initialize DHT
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  if ((millis() - lastTime) > timerDelay) {

    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
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
    Serial.print("Temperature (ºC): ");
    Serial.print(temperature);
    Serial.print(" Humidity (%): ");
    Serial.println(humidity);

    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);

    // Write to ThingSpeak. There are up to 8 fields in a channel, allowing you to store up to 8 different
    // pieces of information in a channel.  Here, we write to field 1.
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    if (x == 200) {
      Serial.println("Channel update successful.");
    }
    else {
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();
  }
  delay(100);
}
