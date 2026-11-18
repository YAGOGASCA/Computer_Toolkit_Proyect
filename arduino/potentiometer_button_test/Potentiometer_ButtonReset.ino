// Inputs test: potentiometer (CPU load 0-100%) and push button (emergency stop)
 
const int POT_PIN = A1;
const int BUTTON_PIN = 7;
 
unsigned long lastReading = 0;
const unsigned long INTERVAL = 500;    // read every 0.5 seconds
 
void setup() {
  Serial.begin(9600);
  pinMode(BUTTON_PIN, INPUT_PULLUP);   // use the Arduino's internal resistor
}
 
void loop() {
  if (millis() - lastReading >= INTERVAL) {
    lastReading = millis();
 
    int load = readLoad();
    int buttonPressed = readButton();
 
    sendData(load, buttonPressed);
  }
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
 
// Sends the data over serial in the format LOAD:75;BTN:0
void sendData(int load, int buttonPressed) {
  Serial.print("LOAD:");
  Serial.print(load);
  Serial.print(";BTN:");
  Serial.println(buttonPressed);
}

