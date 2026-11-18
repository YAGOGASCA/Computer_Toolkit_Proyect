// Thermistor test: reads the raw value (0-1023) and converts it to °C
// using a linear calibration between two measured points.
 
const int THERMISTOR_PIN = A0;
 
// Calibration points (measure them yourself and replace these numbers)
const float RAW_LOW = 470.0;    // raw value at room temperature
const float TEMP_LOW = 16.7;    // room temperature in °C (read from the DHT11)
const float RAW_HIGH = 600.0;   // raw value when holding the thermistor
const float TEMP_HIGH = 33.0;   // skin temperature in °C
 
unsigned long lastReading = 0;
const unsigned long INTERVAL = 1000;   // read every 1 second
 
void setup() {
  Serial.begin(9600);
}
 
void loop() {
  // millis() instead of delay(): the Arduino never blocks
  if (millis() - lastReading >= INTERVAL) {
    lastReading = millis();
 
    int raw = readRaw();
    float celsius = rawToCelsius(raw);
 
    sendData(raw, celsius);
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
 
// Sends the data over serial in the format RAW:512;CPU:22.0
void sendData(int raw, float celsius) {
  Serial.print("RAW:");
  Serial.print(raw);
  Serial.print(";CPU:");
  Serial.println(celsius, 1);
}
