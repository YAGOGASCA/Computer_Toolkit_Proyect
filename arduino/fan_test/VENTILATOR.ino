// Fan test: DC motor through an L293D driver.
// The potentiometer simulates how hot the CPU is (0-100 %).
// When it goes above FAN_ON_PERCENT, the fan turns on.

const int MOTOR_EN = 5;    // L293D pin 1 (Enable): speed with PWM
const int MOTOR_IN1 = 4;   // L293D pin 2 (Input 1): direction / on
const int POT_PIN = A1;

const int FAN_ON_PERCENT = 50;        // NEW: from this "heat" the fan turns on

unsigned long lastUpdate = 0;
const unsigned long INTERVAL = 100;   // update every 0.1 seconds

void setup() {
  Serial.begin(9600);
  pinMode(MOTOR_EN, OUTPUT);
  pinMode(MOTOR_IN1, OUTPUT);
  digitalWrite(MOTOR_IN1, HIGH);   

}

void loop() {
  if (millis() - lastUpdate >= INTERVAL) {
    lastUpdate = millis();

    int heat = map(analogRead(POT_PIN), 0, 1023, 0, 100);   // simulated heat 0-100 %

    if (heat >= FAN_ON_PERCENT) {     // NEW: hot -> fan ON
      setFanSpeed(100);
    } else {                          // NEW: cool -> fan OFF
      setFanSpeed(0);
    }

    Serial.print("HEAT:");
    Serial.print(heat);
    Serial.print(";FAN:");
    Serial.println(heat >= FAN_ON_PERCENT ? 1 : 0);
  }
}

// Sets the fan speed from 0 to 100 %
void setFanSpeed(int percent) {
  int pwm = map(percent, 0, 100, 0, 255);   // analogWrite uses 0-255
  analogWrite(MOTOR_EN, pwm);
}