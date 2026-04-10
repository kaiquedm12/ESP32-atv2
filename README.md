# ESP32 com Arduino — Controle de LEDs e Dashboard de Sensores via Wi-Fi

**Disciplina:** Internet das Coisas (IoT)  
**Plataforma:** ESP32 + Arduino IDE  
**Grupo:** 6

---

## Introdução

O ESP32 é um microcontrolador de baixo custo e alto desempenho, amplamente utilizado em projetos de Internet das Coisas (IoT). Graças ao seu módulo Wi-Fi integrado, é possível criar servidores web embarcados que permitem monitorar e controlar periféricos diretamente pelo navegador, sem a necessidade de infraestrutura adicional.

Este repositório apresenta duas atividades práticas que exploram esse potencial:

- **Atividade 01** — Controle remoto de LEDs via ponto de acesso Wi-Fi (modo AP).
- **Atividade 02** — Dashboard de sensores (temperatura, umidade, luminosidade e potenciômetro) via Wi-Fi em modo cliente (modo STA).

Os códigos-fonte de cada atividade estão organizados em pastas separadas:

```
atividade_01/   → Controle de LEDs
atividade_02/   → Dashboard de Sensores
```

---

## Atividade 01 — Controle de LEDs via Wi-Fi (Modo AP)

📁 Código-fonte: [`atividade_01/main.ino`](atividade_01/main.ino)

### Descrição

Nesta atividade, o ESP32 é configurado como **ponto de acesso Wi-Fi (AP)**. Qualquer dispositivo conectado à rede gerada pelo ESP32 pode acessar, pelo navegador, uma interface web para ligar e desligar quatro LEDs individualmente.

### Hardware utilizado

| Componente    | Pino GPIO |
|---------------|-----------|
| LED 1         | GPIO 4    |
| LED 2         | GPIO 0    |
| LED 3         | GPIO 2    |
| LED 4         | GPIO 15   |

### Funcionamento

1. O ESP32 cria uma rede Wi-Fi com o SSID `ESP32_IoT_Grupo_6` e senha `12345678`.
2. Ao acessar o IP do ESP32 no navegador (`192.168.4.1`), uma página com botões ON/OFF para cada LED é exibida.
3. Cada clique realiza uma requisição HTTP POST para a rota `/led`, acionando o GPIO correspondente.

### Bibliotecas necessárias

- `WiFi.h` (nativa do ESP32)
- `WebServer.h` (nativa do ESP32)

### Resultado

![Atividade 01](https://github.com/user-attachments/assets/7958f9e5-a3f0-4192-9062-d8418bbe6153)

---

## Atividade 02 — Dashboard de Sensores via Wi-Fi (Modo STA)

📁 Código-fonte: [`atividade_02/main.ino`](atividade_02/main.ino)

### Descrição

Nesta atividade, o ESP32 conecta-se a uma rede Wi-Fi existente (modo **STA**) e disponibiliza um dashboard web que exibe, em tempo real, os dados lidos de três sensores. A página atualiza os valores automaticamente a cada segundo e apresenta animações visuais de acordo com as condições ambientais detectadas.

### Hardware utilizado

| Componente       | Pino GPIO |
|------------------|-----------|
| Sensor DHT11     | GPIO 33   |
| Sensor LDR       | GPIO 39   |
| Potenciômetro    | GPIO 34   |

### Funcionamento

1. O ESP32 conecta-se à rede Wi-Fi configurada nas constantes `SSID` e `PASSWORD`.
2. Um servidor web é iniciado com duas rotas:
   - `/` — Página HTML do dashboard.
   - `/sensors` — Endpoint JSON com os valores dos sensores.
3. O JavaScript da página consulta `/sensors` a cada 1 segundo e atualiza a interface.
4. Animações visuais (chuva, vento, sol, fogos) são ativadas conforme os valores lidos:
   - **Umidade > 70%** → animação de chuva.
   - **Luz < 30%** → animação de vento.
   - **Temperatura > 19 °C** → animação de sol.
   - **Temperatura > 22 °C** → animação de fogos.
   - **Luz ≤ 20%** → modo claro (fundo branco).

### Bibliotecas necessárias

- `WiFi.h` (nativa do ESP32)
- `WebServer.h` (nativa do ESP32)
- `DHT.h` — [DHT sensor library (Adafruit)](https://github.com/adafruit/DHT-sensor-library)
- `ArduinoJson.h` — [ArduinoJson](https://arduinojson.org/)

### Resultado

![Atividade 02](https://github.com/user-attachments/assets/14a0f14b-b5e8-4221-8b25-f8ff18a9e842)

---

## Como usar

1. Instale a [Arduino IDE](https://www.arduino.cc/en/software) e adicione o suporte ao ESP32 via Gerenciador de Placas.
2. Instale as bibliotecas listadas em cada atividade pelo Gerenciador de Bibliotecas.
3. Abra o arquivo `.ino` da atividade desejada.
4. Ajuste as credenciais Wi-Fi no código (quando aplicável).
5. Faça o upload para o ESP32 e acesse o IP exibido no Monitor Serial pelo navegador.

---

## Estrutura do Repositório

```
ESP32-atv2/
├── atividade_01/
│   └── main.ino      # Controle de LEDs via AP
├── atividade_02/
│   └── main.ino      # Dashboard de sensores via STA
└── README.md
```
