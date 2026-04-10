#include <WiFi.h>
#include <WebServer.h>

// WiFi AP
#define SSID_AP "ESP32_IoT_Grupo_6"
#define PASSWORD_AP "12345678"

// LEDs
#define LED1 4
#define LED2 0
#define LED3 2
#define LED4 15

WebServer server(80);


int leds[4] = {LED1, LED2, LED3, LED4};
bool ledState[4] = {false, false, false, false};


void handleRoot() {
  String html = R"rawliteral(
<html>
  <head>
    <title>Controle LEDs</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">

    <style>
      body {
        font-family: Arial;
        text-align: center;
        background: #121212;
        color: white;
      }

      .led {
        margin: 20px;
      }

      button {
        padding: 12px 20px;
        margin: 5px;
        border: none;
        border-radius: 8px;
        font-size: 16px;
      }

      .on { background: green; color: white; }
      .off { background: red; color: white; }
    </style>
  </head>

  <body>
    <h1>Controle de LEDs</h1>

    <div class="led">
      <h3>LED 1</h3>
      <button class="on" onclick="toggleLED(0,1)">ON</button>
      <button class="off" onclick="toggleLED(0,0)">OFF</button>
    </div>

    <div class="led">
      <h3>LED 2</h3>
      <button class="on" onclick="toggleLED(1,1)">ON</button>
      <button class="off" onclick="toggleLED(1,0)">OFF</button>
    </div>

    <div class="led">
      <h3>LED 3</h3>
      <button class="on" onclick="toggleLED(2,1)">ON</button>
      <button class="off" onclick="toggleLED(2,0)">OFF</button>
    </div>

    <div class="led">
      <h3>LED 4</h3>
      <button class="on" onclick="toggleLED(3,1)">ON</button>
      <button class="off" onclick="toggleLED(3,0)">OFF</button>
    </div>

    <script>
      function toggleLED(led, state) {
        fetch('/led?led=' + led + '&state=' + state, {
          method: 'POST'
        });
      }
    </script>
  </body>
</html>
)rawliteral";

  server.send(200, "text/html", html);
}

// =======================
// Controle dos LEDs
// =======================
void handleLED() {
  if (server.hasArg("led") && server.hasArg("state")) {
    int ledNum = server.arg("led").toInt();
    int state = server.arg("state").toInt();

    if (ledNum >= 0 && ledNum < 4) {
      digitalWrite(leds[ledNum], state ? HIGH : LOW);
      ledState[ledNum] = state;
    }
  }

  server.send(200, "application/json", "{\"ok\":true}");
}

// =======================
// Setup
// =======================
void setup() {
  Serial.begin(115200);

  // Inicia AP
  WiFi.softAP(SSID_AP, PASSWORD_AP);

  Serial.println("WiFi AP iniciado!");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  // Configura LEDs
  for (int i = 0; i < 4; i++) {
    pinMode(leds[i], OUTPUT);
    digitalWrite(leds[i], LOW);
  }

  // Rotas
  server.on("/", handleRoot);
  server.on("/led", HTTP_POST, handleLED);

  server.begin();
}

// =======================
// Loop
// =======================
void loop() {
  server.handleClient();
}
