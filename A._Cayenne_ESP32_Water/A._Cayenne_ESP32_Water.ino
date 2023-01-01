#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP32.h>

const int ledPin = 5;
const int sensorPin = A0; //sensor pin connected to analog pin A0
float liquid_level;
int liquid_percentage;
int top_level = 1900; //can be changed to any value of maximum sensor value
int bottom_level = 0;

// WiFi network info.
char ssid[] = "AAC Domain";
char wifiPassword[] = "owo20232";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "fa9de470-7f7b-11ed-b193-d9789b2af62b";
char password[] = "3b449543f2b1413ddffea61e5828a05ff09f07d6";
char clientID[] = "b7560c00-86ac-11ed-8d53-d7cd1025126a";

void setup() {
  Serial.begin(115200);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  pinMode(ledPin, OUTPUT);
  pinMode(sensorPin, INPUT);
}

void loop() {
  Cayenne.loop();
  liquid_level = analogRead(sensorPin);
  liquid_percentage = ((liquid_level - bottom_level) / top_level) * 100;
  Serial.println(liquid_percentage);
  delay(1000);
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT(1)
{
  Cayenne.virtualWrite(1, liquid_percentage, "Water Tank", "%");
}

CAYENNE_IN(0)
{
  digitalWrite(ledPin, !getValue.asInt());
}
