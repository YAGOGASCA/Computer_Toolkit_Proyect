# CoolCore – CPU Cooling Simulator

Arduino + Node-RED dashboard – Computer Toolkit project

## Concept
CoolCore is a small model of a computer processor that heats up and cools itself down, just like a real PC. A thermistor acts as the CPU temperature sensor and a DHT11 measures the room. When the CPU gets warm, a fan turns on, and it runs faster when the CPU gets hot. Three LEDs (green, yellow, red) show the heat level on the object itself. The user can simulate CPU load with a potentiometer, stop the fan with an emergency button and the system detects when the case is opened.
It is made for students and PC enthusiasts who want to understand cooling. The dashboard works like PC monitoring software: it shows not only the temperature, but whether the CPU is heating or cooling, how long it stayed in the danger zone and how efficient the cooling is.

## Tier
Tier 2
- [x] Clear creative concept
- [x] At least 2 different sensors
- [x] At least 1 output/actuator reacting to sensor data
- [x] Arduino sends data over serial
- [ ] Node-RED validates messages with regex; malformed messages ignored
- [ ] Real-time dashboard with chart and gauge
- [ ] At least 2 live statistics
- [ ] At least 1 threshold alert
- [x] Analog sensor: raw value (0–1023) + converted value + explanation
- [x] Clean, commented Arduino code with functions
- [x] At least 3 input components, 1 not used in class
- [ ] Two-way communication: at least 2 dashboard controls
- [ ] Adjustable thresholds from the dashboard
- [ ] At least 2 advanced statistics + reset button
- [ ] At least 2 dashboard pages/tabs
- [ ] Custom HTML/CSS in a template node

## Components
- Arduino UNO
- Thermistor + 10 kΩ resistor → CPU temperature (A0)
- DHT11 module → room temperature and humidity (D2)
- Potentiometer → simulated CPU load (A1)
- Push button → emergency stop (D7)
- Tilt switch → case open detection (D3)
- 3 LEDs (green, yellow, red) + 220 Ω resistors → temperature status (A2, A3, A4)
- DC motor with fan blade + L293D driver → cooling fan (D4, D5)
- Power supply module + 9 V battery → separate power for the motor

## How CoolCore reacts
- Below 25 °C: green LED, fan off
- 25–30 °C: yellow LED, fan at 70 %
- 30 °C or more: red LED, fan at 100 %
- Emergency button: the fan stops immediately
- Case open (tilt switch): reported to the dashboard

## Thermistor conversion
The Arduino's 10-bit ADC turns the voltage on A0 into a raw value from 0 to 1023.
I measured two calibration points (room temperature and holding the thermistor
between my fingers) and convert the raw value to °C with a straight line between
these two points: I calculate how many degrees one raw step is worth and add that
to the room temperature. The dashboard shows the raw value next to the °C value.

## Serial message format
Arduino → Node-RED, one line every 2 seconds:
`RAW:512;CPU:22.0;AMB:21.0;HUM:45;LOAD:75;BTN:0;CASE:0;FAN:70`

- RAW: thermistor raw value (0–1023)
- CPU: CPU temperature in °C (thermistor)
- AMB: room temperature in °C (DHT11)
- HUM: room humidity in % (DHT11)
- LOAD: simulated CPU load in % (potentiometer)
- BTN: emergency button (1 = pressed, 0 = not pressed)
- CASE: case open (1 = open, 0 = closed)
- FAN: fan speed in % (0 = off, 70 = medium, 100 = full)

If the DHT11 reading fails, the Arduino sends `ERROR:DHT` instead.

Node-RED → Arduino: (to do)

## Live statistics
(to do)

## How to run
### 1. Hardware
- Connect the components as shown in the wiring diagram (`docs/`).
- Plug the power supply module into the breadboard (set to 5 V) and connect the 9 V battery to it.
  The motor is powered by this module, not by the Arduino. The module's GND is connected to the Arduino's GND.
- Connect the Arduino UNO to the laptop with the USB cable.
- Turn on the power supply module before testing the fan.

### 2. Arduino
1. Install the Arduino IDE.
2. Install these libraries in Tools → Manage Libraries:
   - DHT sensor library (Adafruit)
   - Adafruit Unified Sensor
3. Open `arduino/CPU_COOLER_CODE_FINAL/CPU_COOLER_CODE_FINAL.ino`.
4. Select the board: Tools → Board → Arduino Uno.
5. Select the port: Tools → Port → COMx (the port that appears when the Arduino is plugged in).
6. Click Upload.
7. Optional check: open the Serial Monitor at 9600 baud. A line like this appears every 2 seconds:
   `RAW:512;CPU:22.0;AMB:21.0;HUM:45;LOAD:75;BTN:0;CASE:0;FAN:70`

### 3. Node-RED
(to do: how to install Node-RED, which extra nodes to install, how to import
`node-red/flows.json`, which COM port to select and how to open the dashboard)

## Process & reflection
(to do – notes are kept in `docs/dev_log.md`)

## Sources & tools
- Claude (AI assistant) – used for planning and explanations