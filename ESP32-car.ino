#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "ESP32_CAR";
const char* password = "12345678";

WebServer server(80);

// Motor Pins
#define IN1 26
#define IN2 27
#define IN3 14
#define IN4 12

// LED Pin
#define LED_PIN 4

// ================= MOTOR CONTROL =================

void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(LED_PIN, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(LED_PIN, HIGH);
}

void backward() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(LED_PIN, HIGH);
}

void left() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  digitalWrite(LED_PIN, HIGH);
}

void right() {
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  digitalWrite(LED_PIN, HIGH);
}

// ================= WEBPAGE =================

String webpage = R"rawliteral(
<!DOCTYPE html>
<html lang="en">

<head>
<meta charset="UTF-8">
<meta name="viewport"
content="width=device-width, initial-scale=1.0, maximum-scale=1.0, user-scalable=no">

<title>ESP32 Wi-Fi Car</title>

<style>

* {
    box-sizing: border-box;
    -webkit-tap-highlight-color: transparent;
}

html, body {
    margin: 0;
    padding: 0;
    width: 100%;
    min-height: 100%;
    background: #101522;
    color: white;
    font-family: Arial, sans-serif;
    overflow-x: hidden;
}

body {
    min-height: 100vh;
    min-height: 100dvh;
    display: flex;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    padding: 12px;
    touch-action: manipulation;
}

.header {
    text-align: center;
    margin-bottom: 12px;
}

.header h2 {
    margin: 0;
    font-size: clamp(20px, 4vw, 30px);
    color: #40c4ff;
}

.header p {
    margin: 5px 0 0;
    color: #aab5cc;
    font-size: 13px;
}

.controller {
    width: min(100%, 760px);
    display: flex;
    align-items: center;
    justify-content: space-around;
    gap: 16px;
    padding: 18px;
    background: #1b2335;
    border: 1px solid #303b52;
    border-radius: 24px;
}

.control-group {
    display: grid;
    grid-template-columns: repeat(3, minmax(48px, 75px));
    grid-template-rows: repeat(3, minmax(48px, 75px));
    gap: 8px;
    justify-content: center;
    align-items: center;
}

button {
    width: 100%;
    height: 100%;
    min-width: 0;
    border: 1px solid #536887;
    border-radius: 17px;
    background: #283952;
    color: white;
    font-size: clamp(22px, 5vw, 32px);
    font-weight: bold;
    cursor: pointer;
    user-select: none;
    touch-action: none;
    box-shadow: 0 4px 0 #151d2b;
    transition: background 0.1s, transform 0.1s;
}

button:active,
button.active {
    background: #00a8ff;
    transform: translateY(3px);
    box-shadow: 0 1px 0 #151d2b;
}

.forward {
    grid-column: 2;
    grid-row: 1;
}

.left {
    grid-column: 1;
    grid-row: 2;
}

.stop {
    grid-column: 2;
    grid-row: 2;
    background: #d9364f;
    border-color: #ff667a;
    font-size: 24px;
}

.stop:active,
.stop.active {
    background: #ff1738;
}

.right {
    grid-column: 3;
    grid-row: 2;
}

.backward {
    grid-column: 2;
    grid-row: 3;
}

.info {
    text-align: center;
    margin-top: 15px;
    color: #aab5cc;
    font-size: 12px;
}

.status {
    color: #50e3a4;
    font-weight: bold;
}

/* Landscape phone layout */

@media screen and (orientation: landscape) and (max-height: 600px) {

    body {
        padding: 6px 16px;
    }

    .header {
        margin-bottom: 6px;
    }

    .header h2 {
        font-size: 21px;
    }

    .header p {
        display: none;
    }

    .controller {
        width: min(100%, 850px);
        padding: 10px;
        gap: 12px;
    }

    .control-group {
        grid-template-columns: repeat(3, min(12vh, 65px));
        grid-template-rows: repeat(3, min(12vh, 65px));
        gap: 6px;
    }

    .info {
        margin-top: 8px;
    }
}

/* Small screens */

@media screen and (max-width: 380px) {

    .controller {
        padding: 10px;
        gap: 6px;
    }

    .control-group {
        grid-template-columns: repeat(3, minmax(42px, 60px));
        grid-template-rows: repeat(3, minmax(42px, 60px));
        gap: 5px;
    }
}

</style>
</head>

<body>

<div class="header">
    <h2>🚗 ESP32 Wi-Fi Car</h2>
    <p>Connect to ESP32_CAR to control your car</p>
</div>

<div class="controller">

    <!-- LEFT CONTROL PAD -->

    <div class="control-group">

        <button class="forward"
        data-command="forward"
        aria-label="Forward">⬆</button>

        <button class="left"
        data-command="left"
        aria-label="Left">⬅</button>

        <button class="stop"
        data-command="stop"
        aria-label="Stop">■</button>

        <button class="right"
        data-command="right"
        aria-label="Right">➡</button>

        <button class="backward"
        data-command="back"
        aria-label="Backward">⬇</button>

    </div>

    <!-- RIGHT STATUS -->

    <div style="text-align:center; min-width:100px;">
        <div style="font-size:42px;">🚘</div>
        <div style="font-size:14px; margin-top:8px;">
            CAR STATUS
        </div>
        <div class="status" id="status">READY</div>
    </div>

</div>

<div class="info">
    Hold an arrow to move • Release to stop
</div>

<script>

const statusText = document.getElementById("status");
const buttons = document.querySelectorAll("[data-command]");

let activeCommand = "stop";
let requestNumber = 0;

async function send(command) {

    const currentRequest = ++requestNumber;
    activeCommand = command;

    statusText.textContent = command.toUpperCase();

    try {
        const response = await fetch("/" + command, {
            cache: "no-store"
        });

        if (!response.ok) {
            throw new Error("Request failed");
        }

        if (currentRequest === requestNumber) {
            statusText.textContent = command.toUpperCase();
        }

    } catch (error) {
        if (currentRequest === requestNumber) {
            statusText.textContent = "CONNECTION ERROR";
        }
    }
}

function releaseControl(event) {

    const button = event.currentTarget;

    if (!button.classList.contains("active")) {
        return;
    }

    button.classList.remove("active");

    if (activeCommand !== "stop") {
        send("stop");
    }
}

buttons.forEach(button => {

    const command = button.dataset.command;

    button.addEventListener("pointerdown", event => {

        event.preventDefault();

        if (command === "stop") {
            buttons.forEach(b => b.classList.remove("active"));
            send("stop");
            return;
        }

        buttons.forEach(b => b.classList.remove("active"));
        button.classList.add("active");

        try {
            button.setPointerCapture(event.pointerId);
        } catch (error) {}

        send(command);
    });

    button.addEventListener("pointerup", releaseControl);
    button.addEventListener("pointercancel", releaseControl);
    button.addEventListener("lostpointercapture", releaseControl);

});

document.addEventListener("visibilitychange", () => {
    if (document.hidden && activeCommand !== "stop") {
        buttons.forEach(b => b.classList.remove("active"));
        send("stop");
    }
});

window.addEventListener("blur", () => {
    if (activeCommand !== "stop") {
        buttons.forEach(b => b.classList.remove("active"));
        send("stop");
    }
});

</script>

</body>
</html>
)rawliteral";

// ================= REQUEST HANDLERS =================

void handleRoot() {
  server.sendHeader("Cache-Control", "no-store");
  server.send(200, "text/html; charset=UTF-8", webpage);
}

void setup() {

  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  stopCar();

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password);

  Serial.println();
  Serial.println("ESP32 Wi-Fi Car Started");
  Serial.print("Connect to Wi-Fi: ");
  Serial.println(ssid);
  Serial.print("Open browser at: http://");
  Serial.println(WiFi.softAPIP());

  server.on("/", handleRoot);

  server.on("/forward", []() {
    forward();
    server.send(200, "text/plain; charset=UTF-8", "OK");
  });

  server.on("/back", []() {
    backward();
    server.send(200, "text/plain; charset=UTF-8", "OK");
  });

  server.on("/left", []() {
    left();
    server.send(200, "text/plain; charset=UTF-8", "OK");
  });

  server.on("/right", []() {
    right();
    server.send(200, "text/plain; charset=UTF-8", "OK");
  });

  server.on("/stop", []() {
    stopCar();
    server.send(200, "text/plain; charset=UTF-8", "OK");
  });

  server.begin();

  Serial.println("Web server started!");
}

void loop() {
  server.handleClient();
}
