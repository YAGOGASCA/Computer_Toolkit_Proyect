// LED test: green / yellow / red "traffic light".
// The potentiometer decides which LED is on.

const int GREEN_LED = A2;
const int YELLOW_LED = A3;
const int RED_LED = A4;
const int POT_PIN = A1;

unsigned long lastUpdate = 0;
const unsigned long INTERVAL = 100;   // update every 0.1 seconds

void setup() {
  Serial.begin(9600);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
}

void loop() {
  if (millis() - lastUpdate >= INTERVAL) {
    lastUpdate = millis();

    int value = map(analogRead(POT_PIN), 0, 1023, 0, 100);   // 0 to 100
    showStatus(value);

    Serial.print("VALUE:");
    Serial.println(value);
  }
}

// Turns on one LED: green (0-49), yellow (50-79) or red (80-100)
void showStatus(int value) {
  digitalWrite(GREEN_LED, LOW);     // first turn all LEDs off
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  if (value < 50) {
    digitalWrite(GREEN_LED, HIGH);
  } else if (value < 80) {
    digitalWrite(YELLOW_LED, HIGH);
  } else {
    digitalWrite(RED_LED, HIGH);
  }
}
