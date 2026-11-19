// CoolCore - CPU cooling simulator
// Main sketch: thermistor (CPU) + DHT11 (room) + potentiometer (CPU load) + button (emergency stop)
//              + tilt switch (case open) + status LEDs (green / yellow / red) + fan (L293D)

#include <DHT.h>

// ----- Pins -----
const int THERMISTOR_PIN = A0;
const int POT_PIN = A1;
const int GREEN_LED = A2;
const int YELLOW_LED = A3;
const int RED_LED = A4;
const int DHT_PIN = 2;
const int TILT_PIN = 3;
const int MOTOR_IN1 = 4;         // NEW: L293D pin 2 (Input 1)
const int MOTOR_EN = 5;          // NEW: L293D pin 1 (Enable, PWM speed)
const int BUTTON_PIN = 7;

// ----- Thermistor calibration (use your own measured values) -----
const float RAW_LOW = 512.0;    // raw value at room temperature
const float TEMP_LOW = 16.0;    // room temperature in °C
const float RAW_HIGH = 600.0;   // raw value when holding the thermistor
const float TEMP_HIGH = 33.0;   // skin temperature in °C

// ----- Temperature limits for the LEDs and the fan -----
const float WARM_TEMP = 25.0;   // from here: yellow LED + fan at medium speed
const float HOT_TEMP = 30.0;    // from here: red LED + fan at full speed

// ----- Fan speeds (NEW) -----
const int FAN_MEDIUM = 70;      // % (below ~40 % the motor may not start)
const int FAN_FULL = 100;       // %

// ----- DHT11 sensor -----
DHT dht(DHT_PIN, DHT11);

// ----- Timing -----
unsigned long lastReading = 0;
const unsigned long INTERVAL = 2000;   // read every 2 seconds (DHT11 is slow)

void setup() {
  Serial.begin(9600);
  dht.begin();
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // use the Arduino's internal resistor
  pinMode(TILT_PIN, INPUT_PULLUP);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(MOTOR_EN, OUTPUT);           // NEW
  pinMode(MOTOR_IN1, OUTPUT);          // NEW
  digitalWrite(MOTOR_IN1, HIGH);       // NEW: the fan always turns in the same direction
}

void loop() {
  // NEW: emergency button is checked on every loop, so the fan stops instantly
  if (readButton() == 1) {
    setFanSpeed(0);
  }

  if (millis() - lastReading >= INTERVAL) {
    lastReading = millis();

    // Thermistor (CPU)
    int raw = readRaw();
    float cpuTemp = rawToCelsius(raw);
    showStatus(cpuTemp);                // LEDs follow the CPU temperature

    // Inputs
    int load = readLoad();
    int buttonPressed = readButton();
    int caseOpen = readCaseOpen();

    // Fan (NEW)
    int fanPercent = decideFanSpeed(cpuTemp, buttonPressed);
    setFanSpeed(fanPercent);

    // DHT11 (room)
    float roomTemp = dht.readTemperature();
    float humidity = dht.readHumidity();

    if (isnan(roomTemp) || isnan(humidity)) {
      Serial.println("ERROR:DHT");
      return;
    }

    sendData(raw, cpuTemp, roomTemp, humidity, load, buttonPressed, caseOpen, fanPercent);
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

// Returns 1 if the case is open (switch tilted), 0 if closed (switch upright)
int readCaseOpen() {
  if (digitalRead(TILT_PIN) == HIGH) {
    return 1;
  }
  return 0;
}

// Turns on one LED depending on the CPU temperature
// green (below WARM_TEMP), yellow (WARM_TEMP to HOT_TEMP), red (HOT_TEMP or more)
void showStatus(float cpuTemp) {
  digitalWrite(GREEN_LED, LOW);     // first turn all LEDs off
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  if (cpuTemp < WARM_TEMP) {
    digitalWrite(GREEN_LED, HIGH);
  } else if (cpuTemp < HOT_TEMP) {
    digitalWrite(YELLOW_LED, HIGH);
  } else {
    digitalWrite(RED_LED, HIGH);
  }
}

// NEW: decides the fan speed (%) from the CPU temperature
// green -> off, yellow -> medium, red -> full. Emergency button -> always off.
int decideFanSpeed(float cpuTemp, int buttonPressed) {
  if (buttonPressed == 1) {
    return 0;
  }
  if (cpuTemp < WARM_TEMP) {
    return 0;
  } else if (cpuTemp < HOT_TEMP) {
    return FAN_MEDIUM;
  } else {
    return FAN_FULL;
  }
}

// NEW: sets the fan speed from 0 to 100 % (PWM on the L293D Enable pin)
void setFanSpeed(int percent) {
  int pwm = map(percent, 0, 100, 0, 255);   // analogWrite uses 0-255
  analogWrite(MOTOR_EN, pwm);
}

// Sends all data in one line:
// RAW:512;CPU:22.0;AMB:21.0;HUM:45;LOAD:75;BTN:0;CASE:0;FAN:70
void sendData(int raw, float cpuTemp, float roomTemp, float humidity, int load,
              int buttonPressed, int caseOpen, int fanPercent) {
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
  Serial.print(buttonPressed);
  Serial.print(";CASE:");
  Serial.print(caseOpen);          // changed: print instead of println
  Serial.print(";FAN:");           // NEW
  Serial.println(fanPercent);      // NEW: println goes at the very end
}