# Computer_Toolkit_Proyect
Arduino + Node-RED dashboard – proyecto de Computer Toolkit
# CoolCore – CPU Cooling Simulator

## Concept
CoolCore is a small model of a computer processor that heats up and cools itself down, just like a real PC. A thermistor acts as the CPU temperature sensor and a DHT11 measures the room. When the CPU gets hot, a fan speeds up following a fan curve, an LED bar shows the heat level and a buzzer warns at critical temperature. The user can simulate CPU load, switch modes with an IR remote and detect when the case is opened.
It is made for students and PC enthusiasts who want to understand cooling. The dashboard works like PC monitoring software: it shows not only the temperature, but whether the CPU is heating or cooling, how long it stayed in the danger zone and how efficient the cooling is.

## Tier
Tier 2
- [ ] Clear creative concept
- [ ] At least 2 different sensors
- [ ] At least 1 output/actuator reacting to sensor data
- [ ] Arduino sends data over serial
- [ ] Node-RED validates messages with regex; malformed messages ignored
- [ ] Real-time dashboard with chart and gauge
- [ ] At least 2 live statistics
- [ ] At least 1 threshold alert
- [ ] Analog sensor: raw value (0–1023) + converted value + explanation
- [ ] Clean, commented Arduino code with functions
- [ ] At least 3 input components, 1 not used in class
- [ ] Two-way communication: at least 2 dashboard controls
- [ ] Adjustable thresholds from the dashboard
- [ ] At least 2 advanced statistics + reset button
- [ ] At least 2 dashboard pages/tabs
- [ ] Custom HTML/CSS in a template node

## Components
- Arduino UNO
- Thermistor (CPU temperature) and DHT11 (room temperature and humidity)
- Potentiometer (CPU load), IR receiver and remote (modes), tilt switch (case open), push button (emergency stop)
- DC motor with fan, L293D, power supply module
- 8 LEDs with 74HC595, LCD1602, active buzzer

## Live statistics
(to do)

## How to run
(to do)

## Process & reflection
(to do)

## Sources & tools
- Claude (AI assistant) – used for planning and explanations