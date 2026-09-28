# Flight Computer

A 4-layer rocket/UAV flight computer built around a **Teensy 4.0**. It combines redundant inertial and barometric sensing, GPS, a real-time clock, microSD logging, a CAN bus interface, and a buzzer and LED for status.

> **Status:** Prototype / pre-fab. See [Known issues](#known-issues--to-do-before-ordering) before sending to a board house.

---

## 1. Layers and stackup

The board has 4 copper layers and a finished thickness of 1.6 mm. Every component is on the top side.

| # | Layer | Material | Thickness | Use |
|---|---|---|---|---|
| — | Top silkscreen | — | — | Reference designators |
| — | Top solder mask | — | 0.01 mm | |
| 1 | **F.Cu** (top) | Copper | 0.035 mm | Components, signal routing, small 3.3 V pours, GPS antenna keepout |
| — | Prepreg | FR4 (εr 4.5, tan δ 0.02) | 0.10 mm | |
| 2 | **In1.Cu** | Copper | 0.035 mm | **Solid GND plane** |
| — | Core | FR4 (εr 4.5, tan δ 0.02) | 1.24 mm | |
| 3 | **In2.Cu** | Copper | 0.035 mm | **Solid GND plane** |
| — | Prepreg | FR4 (εr 4.5, tan δ 0.02) | 0.10 mm | |
| 4 | **B.Cu** (bottom) | Copper | 0.035 mm | Signal routing |
| — | Bottom solder mask | — | 0.01 mm | |
| | **Total** | | **1.6 mm** | |

**Layer strategy:** Signals run on the two outer layers. Both inner layers are ground, so every signal trace sits 0.1 mm above an unbroken ground reference. This keeps return paths short and cuts EMI. 3.3 V has no plane of its own; it is routed as tracks, with small copper pours on the top layer.

**Routing rules used**

- Track width: 0.2 mm for all nets, power included.
- Vias: 0.6 mm pad with 0.3 mm drill, through-hole.
- One **blind via** (top → In1) at (245, 29). Many low-cost fabs don't support blind vias or charge extra, so consider changing it to a through via.
- Surface finish: not set in the stackup. Choose **ENIG** when ordering; it gives a flatter surface for the LGA sensor packages than HASL.

---

## 2. Copper weights

| Layer | Thickness | Weight |
|---|---|---|
| F.Cu (top) | 35 µm | **1 oz** |
| In1.Cu | 35 µm | **1 oz** |
| In2.Cu | 35 µm | **1 oz** |
| B.Cu (bottom) | 35 µm | **1 oz** |

1 oz on every layer is plenty: the board carries at most a few hundred mA of 3.3 V logic and sensor current. A 0.2 mm, 1 oz outer trace handles roughly 0.5–0.7 A for a 10 °C temperature rise (IPC-2221 estimate).

Some fabs default to 0.5 oz on inner layers. Specify 1 oz inner copper if you want the stackup built exactly as designed.

---

## 3. Connectors (and why they're there)

| Ref | Connector | Purpose |
|---|---|---|
| **J1** | Würth 693072010801 microSD socket (push-push) | **Flight data logging.** Sensor data is written to the card in SPI mode on the Teensy's main SPI bus, and the card is read back on a PC after recovery. |
| **J4** | 1×2 pin header, 2.54 mm (CANH, CANL) | **CAN bus link** to other avionics modules, such as recovery/pyro, power, or telemetry boards. CAN is differential and tolerates noise, which suits the harsh wiring inside a vehicle. R4 (120 Ω) terminates the bus on this board, so it is meant to sit at one end of the bus. |
| — | Teensy 4.0 micro-USB (on the Teensy module) | **Power, programming and debug.** It is currently the **only power input** to the whole board and is also used for firmware upload and serial output. |

**Deliberately absent**

- **No RF/antenna connector.** The SAM-M8Q GPS has a built-in patch antenna.
- **No battery or power-input connector.** Everything runs from USB through the Teensy (see Protection).

**Notes**

- J4 has **no ground pin**. CAN nodes need a shared ground reference, so consider a 3-pin (CANH/CANL/GND) connector.
- For flight, consider a locking connector instead of the 2.54 mm header on J4, such as JST-GH or Molex Pico-Lock.

---

## 4. Sensor list

| Ref | Part | Measures | Range / key spec | Bus | Why it's on the board |
|---|---|---|---|---|---|
| U9 | Bosch **BMI088** | 3-axis accel + 3-axis gyro | ±24 g accel, ±2000 °/s gyro | SPI | Main IMU for attitude and navigation. Designed to reject vibration, which matters under motor burn. |
| U2 | Bosch **BNO055** | 9-axis (accel, gyro, magnetometer) with onboard fusion | ±16 g, ±2000 °/s, outputs absolute orientation | I²C | Second IMU. Provides ready-made quaternion/Euler output and a magnetic heading, and cross-checks the BMI088. |
| U10 | ADI **ADXL375** | 3-axis high-g accelerometer | ±200 g | SPI | Captures boost, deployment and landing shocks that saturate the ±24 g/±16 g IMUs. |
| U6 | Bosch **BMP390** (primary) | Barometric pressure + temperature | 300–1250 hPa, about ±0.25 m relative | I²C (0x76) | Altitude estimate and apogee detection. |
| U12 | Bosch **BMP390** (secondary) | Barometric pressure + temperature | Same as U6 | I²C (0x77) | Redundant altimeter, so one failed or disagreeing baro doesn't trigger a bad deployment decision. |
| U4 | u-blox **SAM-M8Q** | GNSS position, velocity, time | GPS/GLONASS/Galileo, about 2.5 m CEP | UART | Recovery location and trajectory logging. u-blox M8 receivers are limited to 50 km altitude and 500 m/s; check this for high-performance flights. |
| U7 | TI **INA226-Q1** | Bus voltage, current, power | 0–36 V bus, 16-bit ADC | I²C | Battery and power monitoring. *Currently not connected to a shunt or bus; see Known issues.* |
| IC1 | **DS3231** | Real-time clock (not a sensor) | TCXO, about ±2 ppm | I²C (0x68) | Real-world timestamps on log files. |

---

## 5. Protection

**What's on the board**

| Feature | Part | What it protects against |
|---|---|---|
| Voltage supervisor | **U8 TPS389033-Q1** | Monitors the 3.3 V rail and pulls the Teensy ON/OFF line low on undervoltage. Its delay is set by C6 (1.5 nF). *See Known issues: on a Teensy 4.0, ON/OFF is not a true reset.* |
| CAN transceiver built-in protection | **U5 TJA1051** | The CANH/CANL pins survive about ±58 V faults. The chip also has thermal shutdown and a TXD dominant time-out, so a stuck MCU can't jam the bus. |
| Ground planes | In1 + In2 | Two solid ground layers shield signals and reduce emitted and picked-up noise. |
| GPS keepout | Rule area under U4 | No vias or pads under the patch antenna, which preserves GPS sensitivity. |
| Decoupling | 100 nF–1 µF next to the sensors | Local supply filtering for the BMI088, BNO055, ADXL375 and BMP390s. |
| Pull-ups on open-drain/idle lines | R14, R15, R16, R17 | Keep the BNO055 bootloader pin, the ON/OFF line and the RTC outputs in known states instead of floating. |

**What's *not* on the board (by design or still to do)**

- No reverse-polarity protection, fuse or PTC. There is no power input connector, so power only comes through the Teensy's USB and regulator.
- No TVS/ESD diodes on the CAN lines (J4) or the microSD socket. Adding a CAN ESD part (e.g. PESD2CAN / NUP2105L) is recommended, since J4 goes off-board.
- No brown-out holdup or backup battery. The DS3231 VBAT pin is unconnected and the GPS V_BCKP pin is unconnected.
- No pyro/deployment outputs, so there is no arming or continuity-check circuitry.

---

## 6. Dimensions

| | |
|---|---|
| **Board outline** | **110.0 × 56.0 mm**, rectangular, square corners |
| **Thickness** | 1.6 mm |
| **Component side** | Top only |
| **Mounting holes** | 4 × Ø3.0 mm non-plated (fits M2.5; use 3.2 mm if you want M3 clearance). Each has a Ø6.5 mm keepout. |

**Mounting hole positions** (measured from the board's top-left corner, X right, Y down)

| Hole | X | Y |
|---|---|---|
| H4 (top-left) | 3.0 mm | 4.5 mm |
| H2 (top-right) | 105.0 mm | 4.5 mm |
| H3 (bottom-left) | 3.5 mm | 52.5 mm |
| H2 (bottom-right) | 106.0 mm | 52.0 mm |

The holes are **not on a perfect rectangle**: the bottom pair is offset by 0.5–1.0 mm from the top pair. Align them to a clean pattern (e.g. 102 × 48 mm) if the board mounts to a sled or standoffs.

**Tall components**

| Part | Height above board |
|---|---|
| BZ1 buzzer (Ø12 mm) | about 9.5 mm |
| U4 SAM-M8Q (15.5 × 15.5 mm) | 6.3 mm |
| U1 Teensy 4.0 (35.6 × 17.8 mm) | Depends on mounting (headers or soldered flat) |

---

## Appendix A: Teensy 4.0 pin map

| Teensy pin | Signal | Connected to |
|---|---|---|
| 5 | BUZZER | BZ1 (+), ADXL375 INT1, BMI088 INT3 ⚠️ |
| 6 | LED | D1 through R11 |
| 7 | ADXL_CS | ADXL375 chip select |
| 8, 34 | BMI088 data | BMI088 CSB2 / SDO1 / SDO2 / INT1 ⚠️ |
| 9, 33 | BMI088 INT2 | BMI088 INT2 |
| 10 | SD_CS | microSD DAT3/CD |
| 11 | MOSI | microSD CMD, ADXL375 SDI |
| 12 | MISO | microSD DAT0, ADXL375 SDO |
| 13 | SCK | microSD CLK, ADXL375 SCLK |
| 14 (TX3) | GPS_RX | SAM-M8Q RXD |
| 15 (RX3) | GPS_TX | SAM-M8Q TXD |
| 16 (SCL1) | I2C_SCL | BNO055, BMP390 ×2, DS3231, INA226 |
| 17 (SDA1) | I2C_SDA | BNO055, BMP390 ×2, DS3231, INA226 |
| 22 (CTX1) | CAN_TX | TJA1051 TXD |
| 23 (CRX1) | CAN_RX | TJA1051 RXD |
| 26 (MOSI1) | BMI088_SDI | BMI088 SDA/SDI |
| 27 (SCK1) | BMI088_SCK | BMI088 SCL/SCK |
| ON/OFF | nRESET | TPS3890 RESET_N (R15 1 MΩ pull-up) |
| 3V3 (bottom pad) | +3.3V | Board 3.3 V rail |
| GND (pin 32, bottom pad) | GND | Board ground |

⚠️ = see Known issues below.

## Appendix B: I²C address map (`Wire1`)

| Device | Address | Set by |
|---|---|---|
| BNO055 | 0x29 | COM3 tied to 3.3 V |
| BMP390 (U6) | 0x76 | SDO tied to GND |
| BMP390 (U12) | 0x77 | SDO tied to 3.3 V |
| DS3231 | 0x68 | Fixed |
| INA226 | **Undefined** | A0/A1 floating ⚠️ |

---

## Known issues / to do before ordering

### Likely functional bugs

1. **GPS is powered through a 4.7 kΩ resistor (R3).** It won't power up. Replace R3 with 0 Ω or a ferrite bead.
2. **SAM-M8Q VCC_IO and V_BCKP are unconnected.** Tie both to VCC unless you add a backup battery for V_BCKP.
3. **The CAN transceiver (U5) is unpowered.** VCC, GND, the exposed pad and VIO are all unconnected. It also needs a **5 V** VCC, and the board has no 5 V rail.
4. **BMI088 wiring:**
   - CSB2, SDO1, SDO2 and INT1 are shorted together on one net, which also joins Teensy pins 8 and 34.
   - CSB1 is tied to GND, so the accelerometer is always selected.
   - The SDO lines don't reach SPI1's MISO pin, so hardware SPI1 reads won't work as wired.
   - INT2 shorts Teensy pins 9 and 33 together.
5. **Buzzer net is shared with interrupt outputs.** BZ1 (+) is on the same net as ADXL375 INT1, BMI088 INT3 and Teensy pin 5. A 12 mm buzzer probably also needs a transistor driver rather than a bare GPIO.
6. **ADXL375 GND pins 4 and 5 are unconnected.** Only pin 2 is grounded; all three should be.
7. **BNO055 protocol select.** PS1 = GND and PS0 = 3.3 V selects HID-over-I²C. For standard I²C, tie both low.
8. **INA226 is not wired up.** A0/A1 float, giving an undefined address, and VBUS, IN+ and IN− are unconnected with no shunt resistor.
9. **DS3231 VBAT is floating.** Add a coin cell, or tie VBAT to GND.

### Worth reviewing

- **Too many I²C pull-ups.** Each line has three 4.7 kΩ resistors (about 1.6 kΩ combined) plus two more footprints with no value. Populate one pair and mark the rest DNP.
- **Unset resistor values.** R5, R6, R9, R10 and R11 have the value "R". Set values or mark them DNP.
- **Teensy ON/OFF behavior.** Holding ON/OFF low for 5 s powers the Teensy off; it is not a reset. Confirm this is the intended behavior for the supervisor.
- **R18 (240 kΩ) in series with the TPS3890 SENSE pin.** Check this doesn't shift the trip threshold.
- **Missing decoupling capacitors** on the DS3231, INA226, TJA1051, SAM-M8Q and microSD.
- **Passive sizes.** The passives are 01005 and the LED is 0201, which needs machine assembly. Consider 0402 if you want to hand-assemble.

### Housekeeping

- **Mounting hole designators:** There are two footprints labelled **H2** and no **H1**. Re-annotate.
- **Stray via:** An unconnected via at (132.4, 5.8) sits outside the board outline. Delete it.
- **Duplicate via:** Two identical GND vias are stacked at (171, 44).
- **DRC:** Run it in KiCad before generating Gerbers.

---

## Firmware
<!-- TODO: link to firmware repo, toolchain (Arduino/PlatformIO + Teensyduino), required libraries -->

## Author
<!-- TODO -->

## License
<!-- TODO: e.g. CERN-OHL-S-2.0 for hardware, MIT for firmware -->
