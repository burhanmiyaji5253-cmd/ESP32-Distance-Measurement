# ESP32 Ultrasonic Distance Measurement (Web Server)

Reads distance from an HC-SR04 ultrasonic sensor wired to an ESP32 and serves a
live-updating web page showing the reading, over Wi-Fi.

🎥 Watch the full demo video on Youtube: https://youtube.com/shorts/ahQNf9ook2c

## How it works

The ESP32 runs its own web server (`WebServer` library) and serves a
**self-contained HTML page built directly into the firmware** as a C++ string
(see `handleRoot()` in `firmware/distance_sensor/distance_sensor.ino`) — there's
no separate `.html` file to upload.

| Route | What it does |
|---|---|
| `GET /` | Serves the HTML page. A `<script>` in the page calls `fetch('/distance')` once per second (`setInterval`) and writes the result into the page. |
| `GET /distance` | Triggers `readDistanceCM()`, which pulses the HC-SR04's trig pin, times the echo pulse with `pulseIn()`, converts it to centimeters (`duration * 0.034 / 2`), and returns it as plain text. |

Note: `pulseIn()` blocks until it gets a reading (or times out), so each
`/distance` request briefly blocks the ESP32's main loop — fine for a single-client
hobby project at this update rate, but worth knowing if you extend this.

## Hardware

HC-SR04 ultrasonic sensor → ESP32:

| HC-SR04 pin | ESP32 GPIO |
|---|---|
| Trig | 5  |
| Echo | 18 |
| VCC  | 5V *(see note below)* |
| GND  | GND |

> **Note:** the HC-SR04 is a 5V sensor, but its Echo pin outputs a 5V signal —
> feeding that directly into an ESP32 GPIO (3.3V logic) can damage the pin over
> time. A common fix is a simple voltage divider (e.g. 1kΩ + 2kΩ resistors) on the
> Echo line, or a level shifter. Confirm what you actually did in
> [`docs/wiring.md`](docs/wiring.md).

## Prerequisites

**Arduino IDE** (2.x recommended) with the **ESP32 board package** installed
(Boards Manager → search "esp32" → install by Espressif Systems).

**Arduino libraries** — both bundled with the ESP32 board package, no separate
install needed:
- `WiFi`
- `WebServer`

No Python is used in this project.

## Setup

1. Wire the HC-SR04 as above (with a voltage divider/level shifter on Echo).
2. Open `firmware/distance_sensor/distance_sensor.ino` in Arduino IDE.
3. Set `ssid` / `password` to your Wi-Fi network's credentials.
4. Select your ESP32 board + port, upload.
5. Open Serial Monitor at **115200 baud** — note the IP address it prints once
   connected.
6. Visit `http://<that-ip>/` in a browser on the same Wi-Fi network. The reading
   updates automatically every second.

## Repo structure

```
esp32-distance-sensor/
├── README.md
├── LICENSE
├── .gitignore
├── .gitattributes 
├── firmware/
│   └── distance_sensor/
│       └── distance_sensor.ino
├── docs/
│   └── wiring.md          
```

## License

MIT — see [LICENSE](LICENSE).

## Credits

Created by Burhanuddin Miyajiwala.
