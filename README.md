# IoT Escape Room — Operation Breakout

An escape-room prototype combining an Arduino puzzle device with a Flask web
application. Players solve physical and software puzzles, submit unlock keys in
the browser, and progress through game phases. The server can queue LoRaWAN
downlinks through The Things Network (TTN) to update the device state.

## Project structure

| Path | Purpose |
| --- | --- |
| `frontend/frontend.ino` | Arduino sketch entry point |
| `frontend/game.cpp` | Device state machine and puzzle coordination |
| `frontend/phase.cpp` | Potentiometer-controlled LED synchronization puzzle |
| `frontend/temperature.cpp` | DHT11 temperature threshold check |
| `frontend/password.cpp` | Serial password puzzle |
| `frontend/backend.cpp` | RN2483 communication using the rn2xx3 library |
| `frontend/_debug_backend.cpp` | Alternative RN2483 implementation using raw commands |
| `frontend/epaper.cpp` | E-paper code and employee-record rendering |
| `frontend/src/epd1in54_V2/` | Bundled Waveshare display driver, drawing helpers and fonts |
| `server/app.py` | Flask pages and API routes |
| `server/db_queries.py` | SQLite schema, queries and phase initialization |
| `server/adapter.py` | TTN downlink integration |
| `server/settings.py` | Server phases, database path and downlink configuration |
| `server/templates/` | Browser interface and JavaScript |
| `Doxyfile` | Doxygen configuration |
| `DOCUMENTATION.md` | Documentation scope and generation notes |

## Game flow

The server stores phases in this order:

1. `WAIT_INITIALIZATION`: a player is registered and awaits activation.
2. `PHASE_PUZZLE`: the player synchronizes two blinking LEDs while warming the
   temperature sensor. The device reveals a puzzle key.
3. `SERIAL_PUZZLE`: the player enters the password through the serial monitor and
   receives the next key.
4. `BACKEND_PUZZLE`: the player uses the browser-based measurement lookup to solve
   an intentional SQL-injection puzzle.
5. `COMPLETED`: all puzzle keys have been accepted.

Players submit keys on the player page. A button press on the Arduino enters
`WAIT_BACKEND`, sends an uplink, and checks for a queued state update. If no valid
state is received, the device returns to its previous state.

The current firmware starts directly in `PHASE_PUZZLE`. The server still creates
new players in `WAIT_INITIALIZATION` and advances them when their activation code
is submitted.

## Run the web application

Use Python 3.11 or newer; the server uses `enum.StrEnum`.

From the project root on Linux or macOS:

```bash
cd server
python -m venv .venv
source .venv/bin/activate
python -m pip install -r requirements.txt
python app.py
```

On Windows PowerShell, activate the environment with:

```powershell
.venv\Scripts\Activate.ps1
```

Open `http://127.0.0.1:5000` in a browser.

The server creates `database.db` in its working directory on startup. It initializes
the player, measurement and game-phase tables, seeds an empty measurement table,
and updates the ordered phase definitions and unlock keys.

### Start a game

1. Open `/admin` and register a username with a unique activation code.
2. Open `/player` and enter that activation code.
3. Solve each puzzle and submit its key in the matching form.
4. Use **Reset** to return the player to `PHASE_PUZZLE`.

The browser stores the player ID in local storage and a `player_id` cookie.
**Logout** clears those local values; it does not delete the player or reset the
stored phase.

### Server configuration

| Setting | Default | Purpose |
| --- | --- | --- |
| `debug` environment variable | `True` | Suppress TTN downlinks when set to `true`, `1` or `t` (case insensitive) |
| `applicationName` environment variable | `your-application-name` | TTN application identifier |
| `endDeviceName` environment variable | `your-end-device-name` | TTN end-device identifier |
| `APIKey` environment variable | `your-api-key` | TTN API key for scheduling downlinks |
| `DB_PATH` in `server/settings.py` | `database.db` | SQLite database path |

The `debug` setting controls outgoing device messages. It is separate from Flask's
development debugger; `python app.py` starts Flask with its debugger disabled.

To enable TTN downlinks on Linux or macOS, set the environment variables before
starting the server:

```bash
export debug=false
export applicationName='your-application-id'
export endDeviceName='your-device-id'
export APIKey='your-ttn-api-key'
python app.py
```

`adapter.py` uses the TTN EU1 API, application port 1, normal priority and
unconfirmed downlinks. Each request replaces the device's queued downlinks.

## Set up the Arduino device

The sketch identifies an Arduino Pro Mini with an ATmega328P. The pin assignments
also follow the Uno/Nano-style mapping. Select the board and processor matching
your actual hardware in the Arduino IDE.

Required components include a DHT11 sensor, a potentiometer, a push button,
reference and adjustable LEDs, a red/green status LED, a servo, an RN2483 LoRaWAN
module and a Waveshare 1.54-inch V2 e-paper display.

Install these Arduino libraries:

- DHT sensor library by Adafruit
- Adafruit Unified Sensor
- Servo
- An RN2483 library exposing `rn2xx3.h`

The sketch also uses `SoftwareSerial` and `SPI`. The e-paper driver is included
in the repository.

### Select a backend implementation

`backend.cpp` and `_debug_backend.cpp` define the same public backend functions.
Keep only one implementation in the sketch's compiled source set before building;
otherwise, duplicate definitions prevent linking. Move the unused file outside
the sketch folder.

The current `backend.cpp` returns before its OTAA join code. The alternative
`_debug_backend.cpp` contains the raw-command configuration and join sequence.
For device communication, configure `APP_EUI` and `APP_KEY` in the selected file
and ensure its initialization performs the required join.

Open `frontend/frontend.ino` and set the serial monitor to **57600 baud**. The
password puzzle processes input when it receives a newline.

### Pin assignments

| Arduino pin | Component or signal |
| --- | --- |
| D0 / D1 | Hardware serial RX / TX |
| D2 | Push button, active low with internal pull-up |
| D3 | Servo signal |
| D4 | DHT11 data |
| D5 | Red status LED channel |
| D6 | Green status LED channel |
| D7 | E-paper BUSY |
| D8 | E-paper RESET |
| D9 | E-paper DC |
| D10 | E-paper CS |
| D11 | E-paper MOSI |
| D12 | Arduino RX, connected to RN2483 TX |
| D13 | E-paper SCK |
| A0 | Potentiometer |
| A1 | Reference phase LED |
| A2 | Adjustable phase LED |
| A3 | Arduino TX, connected to RN2483 RX |
| A4 | RN2483 RESET, active low |
| A5 | Unassigned |

Check `frontend/pinout.h` and the display driver's `epdif.h` against your wiring.
The bundled driver also defines `PWR_PIN` as D6 and drives it high during
initialization, sharing the green status LED pin. Resolve that assignment for
your hardware. The RN2483 RX connection requires the 3.3 V signal level noted
in `pinout.h`.

### Puzzle behavior

- **Phase:** a 1000 ms blink period, a hidden 347 ms offset and a synchronization
  tolerance of 30 ms. Synchronization must hold for at least 3000 ms.
- **Temperature:** a threshold of 25 °C, checked at intervals of at least 2000 ms.
  Invalid readings retain the last valid threshold state.
- **Password:** the current password is defined in `frontend/password.cpp`.
- **Display:** revealed codes remain visible for approximately 10 seconds before
  the game loop clears them.
- **Servo:** the puzzle phases map to 0°, 22°, 44°, 66° and 90° positions.

The employee-record rendering functions are documented, but their implementation
bodies remain commented out. The active display path uses `show_code()`.

## API overview

| Method | Route | Purpose |
| --- | --- | --- |
| GET | `/` or `/home` | Home page |
| GET, POST | `/admin` | List or register players |
| GET | `/player` | Player interface |
| GET | `/injection` | Backend puzzle page for a player in the correct phase |
| POST | `/api/player/activate` | Activate a player using `activation_code` |
| GET | `/api/player/<player_id>` | Read a player's identity and phase |
| POST | `/api/player/<player_id>/reset` | Reset the player to `PHASE_PUZZLE` |
| POST | `/api/player/<player_id>/advance` | Submit JSON containing `phase` and `key` |
| POST | `/api/injection` | Submit a measurement lookup using `sensor_value` |

This project deliberately includes a vulnerable SQL lookup as a game mechanic.
The existing `server/readme.md` contains a solution payload for that puzzle.
The player cookie and activation flow are prototype game mechanisms, and the
admin page has no authentication.

## License

The project includes an MIT license in `LICENSE`. Bundled Waveshare and font
sources retain their own copyright and license notices.
