// CoolCore - CPU cooling simulator
// Main sketch: thermistor (CPU) + DHT11 (room) + potentiometer (CPU load) + button (emergency stop)

#include <DHT.h>

// ----- Pins -----
const int THERMISTOR_PIN = A0;
const int POT_PIN = A1;
const int DHT_PIN = 2;
const int BUTTON_PIN = 7;

// ----- Thermistor calibration (use your own measured values) -----
const float RAW_LOW = 470.0;    // raw value at room temperature
const float TEMP_LOW = 16.7;    // room temperature in °C
const float RAW_HIGH = 600.0;   // raw value when holding the thermistor
const float TEMP_HIGH = 33.0;   // skin temperature in °C

// ----- DHT11 sensor -----
DHT dht(DHT_PIN, DHT11);

// ----- Timing -----
unsigned long lastReading = 0;
const unsigned long INTERVAL = 2000;   // read every 2 seconds (DHT11 is slow)

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // use the Arduino's internal resistor
}

void loop() {
  if (millis() - lastReading >= INTERVAL) {
    lastReading = millis();

    // Thermistor (CPU)
    int raw = readRaw();
    float cpuTemp = rawToCelsius(raw);

    // DHT11 (room)
    float roomTemp = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Inputs
    int load = readLoad();
    int buttonPressed = readButton();

    if (isnan(roomTemp) || isnan(humidity)) {
      Serial.println("ERROR:DHT");
      return;
    }

    sendData(raw, cpuTemp, roomTemp, humidity, load, buttonPressed);
  }
}

// Reads the analog value of the thermistor (0-1023)
int readRaw() {
  return analogRead(THERMISTOR_PIN);
}

// Converts the raw value to °C with a straight line between two points
float rawToCelsius(int raw) {
  float degreesPerStep = (TEMP_HIGH - TEMP_LOW) / (RAW_HIGH - RAW_LOW);
  return TEMP_LOW + (raw - RAW_LOW) * degreesPerStep;
}

// Reads the potentiometer and converts 0-1023 to 0-100 %
int readLoad() {
  int raw = analogRead(POT_PIN);
  return map(raw, 0, 1023, 0, 100);
}

// Returns 1 if the button is pressed, 0 if not
int readButton() {
  if (digitalRead(BUTTON_PIN) == LOW) {
    return 1;
  }
  return 0;
}

// Sends all data in one line: RAW:512;CPU:22.0;AMB:21.0;HUM:45;LOAD:75;BTN:0
void sendData(int raw, float cpuTemp, float roomTemp, float humidity, int load, int buttonPressed) {
  Serial.print("RAW:");
  Serial.print(raw);
  Serial.print(";CPU:");
  Serial.print(cpuTemp, 1);
  Serial.print(";AMB:");
  Serial.print(roomTemp, 1);
  Serial.print(";HUM:");
  Serial.print(humidity, 0);
  Serial.print(";LOAD:");
  Serial.print(load);
  Serial.print(";BTN:");
  Serial.println(buttonPressed);
}
