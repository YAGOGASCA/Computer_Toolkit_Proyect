// DHT11 test: reads room temperature and humidity every 2 seconds

#include <DHT.h>

const int DHT_PIN = 2;
DHT dht(DHT_PIN, DHT11);               // tell the library which pin and sensor type

unsigned long lastReading = 0;
const unsigned long INTERVAL = 2000;   // DHT11 is slow: read every 2 seconds

void setup() {
  Serial.begin(9600);
  dht.begin();                         // start the sensor
}

void loop() {
  if (millis() - lastReading >= INTERVAL) {
    lastReading = millis();

    float roomTemp = dht.readTemperature();   // °C
    float humidity = dht.readHumidity();      // %

    if (isnan(roomTemp) || isnan(humidity)) {
      Serial.println("ERROR:DHT");            // reading failed
      return;
    }

    sendData(roomTemp, humidity);
  }
}

// Sends the data over serial in the format AMB:22.0;HUM:41
void sendData(float roomTemp, float humidity) {
  Serial.print("AMB:");
  Serial.print(roomTemp, 1);
  Serial.print(";HUM:");
  Serial.println(humidity, 0);
}