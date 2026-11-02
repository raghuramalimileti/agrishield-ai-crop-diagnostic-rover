#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <ESP32Servo.h>

// ========================================
// AGRISHIELD - ROVER CONTROLLER
// ESP32 + L298N + SENSORS + WEB DASHBOARD
// ESP32-CAM EXCLUDED
// ========================================

// Wi-Fi Access Point
const char* ssid = "AgriShield";
const char* password = "12345678";

WebServer server(80);

// ========================================
// MOTOR DRIVER PINS
// ========================================

#define IN1 13
#define IN2 14
#define IN3 25
#define IN4 26

// ========================================
// SENSOR PINS
// ========================================

#define DHTPIN 4
#define DHTTYPE DHT22

#define SOIL_PIN 34
#define LDR_PIN 32
#define IR_PIN 33

#define SERVO_PIN 23

DHT dht(DHTPIN, DHTTYPE);
Servo probeServo;

// Probe positions
#define PROBE_UP 90
#define PROBE_DOWN 40

// Sensor variables
float temperature = NAN;
float humidity = NAN;

int soilValue = 0;
int ldrValue = 0;
int irValue = 0;

// ========================================
// MOTOR CONTROL
// ========================================

void stopMotors() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void moveForward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void moveBackward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void turnLeft() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void turnRight() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// ========================================
// SOIL PROBE SERVO
// ========================================

void probeUp() {
  probeServo.write(PROBE_UP);
  Serial.println("Soil probe: UP");
}

void probeDown() {
  probeServo.write(PROBE_DOWN);
  Serial.println("Soil probe: DOWN");
}

// ========================================
// READ SENSORS
// ========================================

void readSensors() {

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();

  soilValue = analogRead(SOIL_PIN);
  ldrValue = analogRead(LDR_PIN);
  irValue = digitalRead(IR_PIN);
}

// ========================================
// SENSOR JSON RESPONSE
// ========================================

void sendSensorData() {

  readSensors();

  String json = "{";

  if (isnan(temperature)) {
    json += "\"temperature\":null,";
  } else {
    json += "\"temperature\":" + String(temperature, 1) + ",";
  }

  if (isnan(humidity)) {
    json += "\"humidity\":null,";
  } else {
    json += "\"humidity\":" + String(humidity, 1) + ",";
  }

  json += "\"soil\":" + String(soilValue) + ",";
  json += "\"ldr\":" + String(ldrValue) + ",";
  json += "\"ir\":" + String(irValue);

  json += "}";

  server.send(200, "application/json", json);

  Serial.println(json);
}

// ========================================
// DASHBOARD HTML
// ========================================

const char dashboard[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html>

<head>

<title>AgriShield Rover</title>

<meta name="viewport"
content="width=device-width, initial-scale=1">

<style>

body {
  background: #101820;
  color: white;
  font-family: Arial;
  text-align: center;
  margin: 0;
}

header {
  background: #174d32;
  padding: 18px;
}

h2 {
  margin: 5px;
}

.container {
  padding: 12px;
}

.card {
  background: #202e38;
  padding: 15px;
  margin: 12px auto;
  border-radius: 12px;
  max-width: 500px;
}

.sensor {
  display: grid;
  grid-template-columns: 1fr 1fr;
  gap: 10px;
}

.value {
  background: #30424e;
  padding: 15px;
  border-radius: 10px;
}

.value h3 {
  color: #9ee6b0;
}

button {
  border: none;
  border-radius: 10px;
  padding: 15px 20px;
  margin: 5px;
  font-size: 16px;
  color: white;
  background: #26834b;
  cursor: pointer;
}

button:active {
  background: #14552d;
}

.stop {
  background: #d32f2f;
}

.probe {
  background: #2469a0;
}

.grid {
  display: grid;
  grid-template-columns: repeat(3, 1fr);
  max-width: 300px;
  margin: auto;
}

.status {
  color: #9ee6b0;
}

</style>

</head>

<body>

<header>

<h2>AGRISHIELD</h2>

<p>Active AI Crop Diagnostic Rover</p>

<p class="status">ROVER DASHBOARD ONLINE</p>

</header>

<div class="container">

<div class="card">

<h3>Rover Movement</h3>

<div class="grid">

<div></div>

<button onclick="move('forward')">▲</button>

<div></div>

<button onclick="move('left')">◀</button>

<button class="stop"
onclick="move('stop')">STOP</button>

<button onclick="move('right')">▶</button>

<div></div>

<button onclick="move('backward')">▼</button>

<div></div>

</div>

</div>

<div class="card">

<h3>Soil Probe Control</h3>

<button class="probe"
onclick="probe('down')">
LOWER PROBE
</button>

<button class="probe"
onclick="probe('up')">
RAISE PROBE
</button>

<p id="probeStatus">Probe position: UP</p>

</div>

<div class="card">

<h3>Live Sensor Monitoring</h3>

<div class="sensor">

<div class="value">
<h3>Temperature</h3>
<p id="temp">--</p>
</div>

<div class="value">
<h3>Humidity</h3>
<p id="humidity">--</p>
</div>

<div class="value">
<h3>Soil Moisture</h3>
<p id="soil">--</p>
</div>

<div class="value">
<h3>Light (LDR)</h3>
<p id="ldr">--</p>
</div>

<div class="value">
<h3>IR Sensor</h3>
<p id="ir">--</p>
</div>

</div>

<p id="updated">Waiting for sensor data...</p>

</div>

</div>

<script>

function move(direction) {

  fetch('/move?dir=' + direction)
  .then(response => response.text())
  .then(data => console.log(data))
  .catch(error => console.log(error));

}

function probe(position) {

  fetch('/probe?pos=' + position)
  .then(response => response.text())
  .then(data => {

    document.getElementById('probeStatus')
    .innerText = 'Probe position: ' + position.toUpperCase();

  });

}

function updateSensors() {

  fetch('/sensors')
  .then(response => response.json())
  .then(data => {

    document.getElementById('temp').innerText =
      data.temperature === null ? 'ERROR' :
      data.temperature + ' °C';

    document.getElementById('humidity').innerText =
      data.humidity === null ? 'ERROR' :
      data.humidity + ' %';

    document.getElementById('soil').innerText =
      data.soil;

    document.getElementById('ldr').innerText =
      data.ldr;

    document.getElementById('ir').innerText =
      data.ir == 0 ? 'OBJECT DETECTED' : 'NO OBJECT';

    document.getElementById('updated').innerText =
      'Last updated: ' + new Date().toLocaleTimeString();

  })
  .catch(error => {

    document.getElementById('updated').innerText =
      'Sensor connection error';

  });

}

setInterval(updateSensors, 1500);

updateSensors();

</script>

</body>
</html>

)rawliteral";

// ========================================
// WEB SERVER ROUTES
// ========================================

void handleRoot() {
  server.send_P(200, "text/html", dashboard);
}

void handleMovement() {

  String direction = server.arg("dir");

  if (direction == "forward") {
    moveForward();
  }

  else if (direction == "backward") {
    moveBackward();
  }

  else if (direction == "left") {
    turnLeft();
  }

  else if (direction == "right") {
    turnRight();
  }

  else {
    stopMotors();
  }

  server.send(200, "text/plain", "Movement: " + direction);
}

void handleProbe() {

  String position = server.arg("pos");

  if (position == "down") {
    probeDown();
  }

  else {
    probeUp();
  }

  server.send(200, "text/plain", "Probe: " + position);
}

// ========================================
// SETUP
// ========================================

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println("================================");
  Serial.println("AGRISHIELD ROVER STARTING");
  Serial.println("================================");

  // Motor pins

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopMotors();

  // Sensor pins

  pinMode(SOIL_PIN, INPUT);
  pinMode(LDR_PIN, INPUT);
  pinMode(IR_PIN, INPUT);

  // DHT22

  dht.begin();

  // Servo

  probeServo.setPeriodHertz(50);
  probeServo.attach(SERVO_PIN, 500, 2400);

  probeUp();

  // ADC

  analogReadResolution(12);

  // Wi-Fi Access Point

  WiFi.mode(WIFI_AP);

  WiFi.softAP(ssid, password);

  delay(500);

  Serial.println();
  Serial.println("Wi-Fi Access Point Started");

  Serial.print("Network: ");
  Serial.println(ssid);

  Serial.print("Dashboard IP: ");
  Serial.println(WiFi.softAPIP());

  // Dashboard routes

  server.on("/", handleRoot);

  server.on("/move", handleMovement);

  server.on("/probe", handleProbe);

  server.on("/sensors", sendSensorData);

  server.begin();

  Serial.println("Web server started");

  Serial.println("AGRISHIELD ROVER READY");

  Serial.println("================================");
}

// ========================================
// MAIN LOOP
// ========================================

void loop() {

  server.handleClient();

}
