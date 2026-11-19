// Tilt switch test: detects if the PC case is open (switch tilted)

const int TILT_PIN = 3;

unsigned long lastReading = 0;
const unsigned long INTERVAL = 500;    // read every 0.5 seconds

void setup() {
  Serial.begin(9600);
  pinMode(TILT_PIN, INPUT_PULLUP);     // use the Arduino's internal resistor
}

void loop() {
  if (millis() - lastReading >= INTERVAL) {
    lastReading = millis();

    int caseOpen = readCaseOpen();

    sendData(caseOpen);
  }
}

// Returns 1 if the case is open (switch tilted), 0 if closed (switch upright)
int readCaseOpen() {
  if (digitalRead(TILT_PIN) == HIGH) {
    return 1;
  }
  return 0;
}

// Sends the data over serial in the format CASE:0
void sendData(int caseOpen) {
  Serial.print("CASE:");
  Serial.println(caseOpen);
}