# ESP32-atv2


atividade 01
código:
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

<img width="1693" height="655" alt="image" src="https://github.com/user-attachments/assets/7958f9e5-a3f0-4192-9062-d8418bbe6153" />



atividade 02
código:
#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHT_PIN 33   // Sensor DHT11
#define LDR_PIN 39   // Sensor LDR
#define POT_PIN 34   // Sensor Potenciômetro

#define SSID "Paulo"       // Substitua pelo seu SSID
#define PASSWORD "12345678"  // Substitua pela sua senha do WiFi

DHT dht(DHT_PIN, DHT11);  // Inicializa o sensor DHT11
WebServer server(80);     // Inicia o servidor web

// Função para lidar com a requisição dos sensores
void handleSensors() {
  // Leitura dos sensores
  float temp = dht.readTemperature();  // Temperatura
  float umidade = dht.readHumidity();  // Umidade
  int luz = map(analogRead(LDR_PIN), 0, 4095, 0, 100);  // LDR (luz)
  int pot = analogRead(POT_PIN);  // Potenciômetro (0-4095)

  // Criar um documento JSON
  StaticJsonDocument<200> doc;
  doc["temp"] = temp;
  doc["umidade"] = umidade;
  doc["luz"] = luz;
  doc["pot"] = pot;

  // Converter o JSON para string
  String json;
  serializeJson(doc, json);

  // Retorna o JSON como resposta para o cliente
  server.send(200, "application/json", json);
}

// Função para renderizar a página principal
void handleRoot() {
  String html = R"(
<html>
  <head>
    <title>Dashboard de Sensores</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
      body {
        font-family: 'Arial', sans-serif;
        text-align: center;
        background-color: #121212;
        color: white;
        transition: background-color 1s;
      }
      
      h1 {
        margin-top: 20px;
        font-size: 2.5em;
      }

      .sensor-container {
        display: flex;
        justify-content: center;
        margin-top: 30px;
        gap: 50px;
      }

      .sensor {
        background-color: #333;
        border-radius: 15px;
        padding: 20px;
        width: 250px;
        box-shadow: 0px 4px 20px rgba(0, 0, 0, 0.2);
        transition: transform 0.3s ease;
      }

      .sensor:hover {
        transform: translateY(-10px);
      }

      .sensor h3 {
        font-size: 1.8em;
      }

      .value {
        font-size: 2.5em;
        font-weight: bold;
      }

      .value-unit {
        font-size: 1.2em;
        color: #ddd;
      }

      #temp, #umidade, #luz, #pot {
        margin-top: 20px;
      }

      /* Animações */
      @keyframes rain {
        0% { opacity: 0.5; transform: translateY(-20px); }
        100% { opacity: 1; transform: translateY(100vh); }
      }

      .rain {
        position: absolute;
        top: 0;
        left: 50%;
        width: 2px;
        height: 30px;
        background-color: rgba(255, 255, 255, 0.7);
        animation: rain 1s infinite linear;
        pointer-events: none;
        z-index: -1;
      }

      @keyframes wind {
        0% { transform: rotate(0deg); }
        100% { transform: rotate(360deg); }
      }

      .wind {
        position: absolute;
        top: 50%;
        left: 50%;
        width: 50px;
        height: 50px;
        border: 10px solid #fff;
        border-radius: 50%;
        border-top: 10px solid transparent;
        border-right: 10px solid transparent;
        animation: wind 2s infinite linear;
        z-index: -1;
      }

      /* Estilo para o modo claro */
      .light-mode {
        background-color: #ffffff;
        color: black;
      }

      /* Estilos do Sol e Fogos */
      .sun {
        position: absolute;
        top: 10%;
        left: 50%;
        width: 100px;
        height: 100px;
        background-color: yellow;
        border-radius: 50%;
        box-shadow: 0px 0px 50px 20px rgba(255, 255, 0, 0.7);
        z-index: -1;
        animation: sunAnimation 5s infinite;
      }

      @keyframes sunAnimation {
        0% { transform: scale(1); opacity: 0.8; }
        100% { transform: scale(1.1); opacity: 1; }
      }

      .fireworks {
        position: absolute;
        top: 30%;
        left: 50%;
        width: 100px;
        height: 100px;
        background-color: red;
        border-radius: 50%;
        box-shadow: 0px 0px 50px 20px rgba(255, 0, 0, 0.7);
        animation: fireworksAnimation 1s infinite;
      }

      @keyframes fireworksAnimation {
        0% { transform: scale(1); opacity: 0.8; }
        100% { transform: scale(1.5); opacity: 1; }
      }

    </style>
  </head>
  <body>
    <h1>Dashboard de Sensores</h1>

    <div class="sensor-container">
      <div id="temp" class="sensor">
        <h3>Temperatura</h3>
        <div class="value">Carregando...</div>
        <div class="value-unit">°C</div>
      </div>

      <div id="umidade" class="sensor">
        <h3>Umidade</h3>
        <div class="value">Carregando...</div>
        <div class="value-unit">%</div>
      </div>

      <div id="luz" class="sensor">
        <h3>Luz (LDR)</h3>
        <div class="value">Carregando...</div>
        <div class="value-unit">%</div>
      </div>

      <div id="pot" class="sensor">
        <h3>Potenciômetro</h3>
        <div class="value">Carregando...</div>
        <div class="value-unit">0-4095</div>
      </div>
    </div>

    <!-- Animações de clima -->
    <div id="rain" style="display: none;"></div>
    <div id="wind" style="display: none;"></div>
    <div id="sun" style="display: none;" class="sun"></div>
    <div id="fireworks" style="display: none;" class="fireworks"></div>

    <script>
      // Função para buscar dados do servidor
      function fetchData() {
        fetch('/sensors')
          .then(response => response.json())
          .then(data => {
            // Atualiza os valores na página
            document.getElementById("temp").getElementsByClassName("value")[0].innerText = data.temp.toFixed(2);
            document.getElementById("umidade").getElementsByClassName("value")[0].innerText = data.umidade.toFixed(2);
            document.getElementById("luz").getElementsByClassName("value")[0].innerText = data.luz;
            document.getElementById("pot").getElementsByClassName("value")[0].innerText = data.pot;

            // Modo Claro para luz < 20%
            if (data.luz <= 20) {
              document.body.classList.add("light-mode");
            } else {
              document.body.classList.remove("light-mode");
            }

            // Animação de chuva se a umidade for alta
            if (data.umidade > 70) {
              document.getElementById("rain").style.display = 'block';
              for (let i = 0; i < 50; i++) {
                let rainDrop = document.createElement('div');
                rainDrop.classList.add('rain');
                rainDrop.style.left = `${Math.random() * 100}vw`;
                rainDrop.style.animationDuration = `${Math.random() * 2 + 2}s`;
                document.getElementById("rain").appendChild(rainDrop);
              }
            } else {
              document.getElementById("rain").style.display = 'none';
            }

            // Animação de vento se a luz for baixa
            if (data.luz < 30) {
              document.getElementById("wind").style.display = 'block';
            } else {
              document.getElementById("wind").style.display = 'none';
            }

            // Animação de Sol se a temperatura for maior que 25°C
            if (data.temp > 19) {
              document.getElementById("sun").style.display = 'block';
            } else {
              document.getElementById("sun").style.display = 'none';
            }

            // Animação de Fogos se a temperatura for maior que 30°C
            if (data.temp > 22) {
              document.getElementById("fireworks").style.display = 'block';
            } else {
              document.getElementById("fireworks").style.display = 'none';
            }
          })
          .catch(error => console.error('Erro ao buscar dados:', error));
      }

      // Atualiza os dados a cada 1 segundo
      setInterval(fetchData, 1000);
      fetchData();  // Busca os dados na primeira vez
    </script>
  </body>
</html>
)";

  server.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  dht.begin();  // Inicia o sensor DHT11
  
  // Conectar-se à rede WiFi
  WiFi.mode(WIFI_STA);  // Modo cliente
  WiFi.begin(SSID, PASSWORD);
  
  // Aguardar conexão com o WiFi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("\nWiFi conectado!");
  Serial.print("IP do ESP32: ");
  Serial.println(WiFi.localIP());  // Exibe o IP do ESP32 no monitor serial
  
  // Configura as rotas
  server.on("/sensors", handleSensors);
  server.on("/", handleRoot);
  
  // Inicia o servidor web
  server.begin();
}

void loop() {
  server.handleClient();  // Aguarda requisições do cliente
}

<img width="1772" height="747" alt="image" src="https://github.com/user-attachments/assets/14a0f14b-b5e8-4221-8b25-f8ff18a9e842" />
