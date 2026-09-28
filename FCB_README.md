# Flight Computer

A **4-layer rocket/UAV flight computer** built around a **Teensy 4.0**. The board integrates redundant inertial and barometric sensing, GNSS, a real-time clock, microSD data logging, CAN communication, and onboard status indicators.

> **Status:** ⚠️ Prototype / Pre-Fabrication
> Review the [Known Issues](#known-issues--to-do-before-ordering) section before sending the design for fabrication.

---

## 1. Board Overview

### Key Features

* **Teensy 4.0** main flight controller
* Redundant IMU and barometric sensing
* High-G accelerometer for launch/deployment events
* GNSS positioning and recovery tracking
* Real-time clock for timestamped data
* microSD flight-data logging
* CAN bus communication
* Onboard buzzer and status LED
* 4-layer PCB with dedicated ground planes
* All components mounted on the top side

---

## 2. PCB Stackup

The PCB uses a **4-layer stackup** with a nominal finished thickness of **1.6 mm**.

| Layer   | Type   | Thickness | Purpose                                                |
| ------- | ------ | --------: | ------------------------------------------------------ |
| Top     | F.Cu   |     35 µm | Components, signal routing, 3.3 V routing, GPS keepout |
| Inner 1 | In1.Cu |     35 µm | **Solid GND plane**                                    |
| Inner 2 | In2.Cu |     35 µm | **Solid GND plane**                                    |
| Bottom  | B.Cu   |     35 µm | Signal routing                                         |

### Dielectric Structure

| Material               |   Thickness |
| ---------------------- | ----------: |
| Top solder mask        |     0.01 mm |
| Prepreg                |     0.10 mm |
| FR4 core               |     1.24 mm |
| Prepreg                |     0.10 mm |
| Bottom solder mask     |     0.01 mm |
| **Finished thickness** | **1.60 mm** |

FR4 dielectric assumptions:

* Relative permittivity, εr: **4.5**
* Loss tangent, tan δ: **0.02**

### Layer Strategy

Signal routing is primarily performed on the two outer copper layers, while **both inner layers are solid ground planes**.

This provides:

* Shorter return-current paths
* Lower loop area
* Improved signal integrity
* Reduced EMI
* Better shielding between signal layers

The 3.3 V rail is routed using traces and local copper pours rather than a dedicated power plane.

### Routing Rules

| Parameter            | Specification        |
| -------------------- | -------------------- |
| Standard trace width | 0.20 mm              |
| Via pad diameter     | 0.60 mm              |
| Via drill            | 0.30 mm              |
| Via type             | Through-hole         |
| Blind vias           | 1 currently present  |
| Surface finish       | **ENIG recommended** |

> **Fabrication note:** A blind via currently exists from Top → In1.Cu at approximately `(245, 29)`. Many low-cost PCB manufacturers do not support blind vias or charge additional fees. Consider converting this to a standard through-hole via before fabrication.

### Surface Finish

**ENIG (Electroless Nickel Immersion Gold)** is recommended because the board contains fine-pitch/LGA sensor packages and benefits from a flat, consistent surface.

---

## 3. Copper Weight

All four copper layers use **1 oz copper**.

| Layer  | Copper Thickness | Copper Weight |
| ------ | ---------------: | ------------: |
| F.Cu   |            35 µm |          1 oz |
| In1.Cu |            35 µm |          1 oz |
| In2.Cu |            35 µm |          1 oz |
| B.Cu   |            35 µm |          1 oz |

The flight computer carries relatively low current, primarily for the Teensy, sensors, GNSS, storage, and communication interfaces. Therefore, **1 oz copper is sufficient for the intended electrical loads**.

The standard 0.20 mm traces should nevertheless be reviewed against the actual current requirements of each power net before fabrication.

> **Fabrication note:** Some manufacturers default to 0.5 oz copper on inner layers. Specify **1 oz copper on all four layers** if the fabricated stackup is required to match the design.

---

# 4. Connectors and Interfaces

| Ref.   | Connector / Interface             | Purpose                          |
| ------ | --------------------------------- | -------------------------------- |
| **J1** | Würth 693072010801 microSD socket | Flight-data storage              |
| **J4** | 1×2, 2.54 mm header               | CANH / CANL connection           |
| —      | Teensy 4.0 micro-USB              | Power, programming and debugging |

### J1 — microSD

The microSD socket provides non-volatile storage for flight data.

The Teensy communicates with the card using **SPI**, allowing sensor measurements, GNSS data, system status, and other flight information to be logged during operation.

The card can be removed after recovery and read using a standard PC.

### J4 — CAN Bus

J4 provides the external **CANH/CANL** connection to other avionics modules.

Potential CAN-connected systems include:

* Recovery/pyro controller
* Power-management board
* Telemetry system
* Other flight computers or avionics modules

CAN uses differential signalling and provides good noise immunity, making it suitable for inter-board communication within a vehicle.

A **120 Ω termination resistor (R4)** is currently fitted, meaning this board is intended to operate at one end of the CAN bus.

> ⚠️ **Review:** J4 currently provides only CANH and CANL. A shared ground reference is recommended for the connected CAN nodes. Consider changing J4 to a **3-pin CANH/CANL/GND connector**.

### Teensy USB

The Teensy 4.0 micro-USB connection currently provides:

* Board power
* Firmware programming
* Serial debugging
* Development/testing access

> **Important:** USB is currently the **only power input to the board**.

### Deliberately Absent Interfaces

The board currently does not include:

* **External GPS antenna connector** — the SAM-M8Q includes an integrated patch antenna.
* **Dedicated battery connector** — the current design is powered through the Teensy USB connection.
* **Pyro/deployment outputs** — deployment circuitry is not implemented on this board.

For flight use, consider replacing the J4 2.54 mm header with a locking connector such as **JST-GH** or **Molex Pico-Lock**.

---

# 5. Sensor and Monitoring Devices

The flight computer uses multiple sensors to provide redundancy and cover different flight regimes.

| Ref.    | Device         | Measurement                       | Interface | Purpose                        |
| ------- | -------------- | --------------------------------- | --------- | ------------------------------ |
| **U9**  | Bosch BMI088   | 3-axis acceleration + 3-axis gyro | SPI       | Primary IMU                    |
| **U2**  | Bosch BNO055   | 9-axis IMU + sensor fusion        | I²C       | Secondary IMU and orientation  |
| **U10** | ADXL375        | ±200 g acceleration               | SPI       | High-G event detection         |
| **U6**  | Bosch BMP390   | Pressure + temperature            | I²C       | Primary altitude estimation    |
| **U12** | Bosch BMP390   | Pressure + temperature            | I²C       | Redundant altitude measurement |
| **U4**  | u-blox SAM-M8Q | Position, velocity, time          | UART      | GNSS positioning               |
| **U7**  | TI INA226-Q1   | Voltage, current, power           | I²C       | Power monitoring               |
| **IC1** | DS3231         | Real-time clock                   | I²C       | Timestamping                   |

### BMI088 — Primary IMU

The BMI088 provides:

* 3-axis acceleration
* 3-axis angular velocity
* High vibration tolerance

It is intended as the primary inertial measurement source during flight, particularly during high-vibration conditions.

### BNO055 — Secondary IMU

The BNO055 provides:

* 3-axis acceleration
* 3-axis angular velocity
* 3-axis magnetometer
* Onboard sensor fusion
* Quaternion/Euler orientation output

It provides an independent inertial measurement source and can be used to cross-check the BMI088.

### ADXL375 — High-G Accelerometer

The ADXL375 provides acceleration measurement up to approximately **±200 g**.

It complements the lower-range IMUs by capturing events such as:

* Launch acceleration
* High-G flight events
* Deployment shocks
* Landing impacts

### BMP390 — Barometric Altimeters

Two BMP390 devices are used for **redundant altitude measurement**.

| Device | I²C Address | Role      |
| ------ | ----------- | --------- |
| U6     | 0x76        | Primary   |
| U12    | 0x77        | Secondary |

Using two independent barometers provides an additional measurement source for altitude and apogee detection.

### SAM-M8Q — GNSS

The SAM-M8Q provides:

* Position
* Velocity
* Time

It is primarily used for:

* Recovery location
* Trajectory logging
* Flight-data analysis

> ⚠️ **High-altitude flight note:** Check the receiver's altitude and velocity limitations against the intended flight profile before use.

### INA226-Q1 — Power Monitoring

The INA226 is intended to measure:

* Bus voltage
* Current
* Power

> ⚠️ **Current status:** The INA226 is not currently connected to a shunt or bus. See [Known Issues](#known-issues--to-do-before-ordering).

### DS3231 — Real-Time Clock

The DS3231 provides an accurate real-time reference for timestamping logged flight data.

It is a clock rather than a flight sensor.

---

# 6. Protection and Reliability

## Implemented Protection

| Feature             | Device / Implementation | Purpose                           |
| ------------------- | ----------------------- | --------------------------------- |
| Voltage supervision | TPS389033-Q1            | Monitors 3.3 V rail               |
| CAN protection      | TJA1051                 | CAN bus fault tolerance           |
| Ground planes       | In1.Cu + In2.Cu         | Noise reduction and signal return |
| GPS keepout         | PCB rule area           | Protects antenna performance      |
| Local decoupling    | 100 nF–1 µF capacitors  | Supply filtering                  |
| Pull-ups / biasing  | R14–R17                 | Prevents floating control signals |

### Voltage Supervisor

**U8 — TPS389033-Q1**

The supervisor monitors the 3.3 V supply and controls the Teensy's ON/OFF line when the supply falls below the configured threshold.

> ⚠️ The Teensy 4.0 ON/OFF pin is not equivalent to a conventional hardware reset input. Verify that the intended supervisor behaviour is appropriate for the flight application.

### CAN Protection

**U5 — TJA1051**

The CAN transceiver provides built-in protection against bus faults and includes features such as thermal shutdown and TXD dominant-timeout protection.

> ⚠️ No external CAN ESD/TVS protection is currently fitted. Because J4 connects to an external cable, additional CAN protection should be considered.

### Ground Planes

In1.Cu and In2.Cu are maintained as solid ground planes to:

* Provide low-impedance return paths
* Reduce loop area
* Reduce EMI
* Improve signal integrity

### GPS Antenna Keepout

A dedicated keepout region is provided beneath the SAM-M8Q patch antenna to avoid copper, vias, and components interfering with GNSS performance.

---

## Protection Not Currently Implemented

The following protection features are absent or incomplete:

* ❌ Reverse-polarity protection
* ❌ Input fuse/PTC
* ❌ CAN external TVS/ESD protection
* ❌ microSD ESD protection
* ❌ Brown-out energy storage
* ❌ RTC backup battery
* ❌ GNSS backup supply
* ❌ Pyro/deployment protection circuitry

These should be reviewed according to the final system architecture and flight requirements.

---

# 7. Board Dimensions

| Parameter             | Specification       |
| --------------------- | ------------------- |
| Board size            | **110.0 × 56.0 mm** |
| Board thickness       | **1.6 mm**          |
| Board shape           | Rectangular         |
| Component placement   | Top side only       |
| Mounting holes        | 4 × Ø3.0 mm NPTH    |
| Mounting-hole keepout | Ø6.5 mm             |

### Mounting Hole Locations

Coordinates are measured from the **top-left corner** of the PCB:

| Hole |        X |       Y |
| ---- | -------: | ------: |
| H4   |   3.0 mm |  4.5 mm |
| H2   | 105.0 mm |  4.5 mm |
| H3   |   3.5 mm | 52.5 mm |
| H2   | 106.0 mm | 52.0 mm |

> ⚠️ The mounting holes are currently **not arranged on a perfectly rectangular pattern**. The bottom holes are offset by approximately 0.5–1.0 mm relative to the top holes. Consider standardising the mounting pattern before fabrication.

### Tall Components

| Component  |             Approx. Height |
| ---------- | -------------------------: |
| BZ1 buzzer |                     9.5 mm |
| U4 SAM-M8Q |                     6.3 mm |
| Teensy 4.0 | Depends on mounting method |

---

# 8. Teensy 4.0 Pin Map

| Teensy Pin | Signal         | Connected Device                     |
| ---------- | -------------- | ------------------------------------ |
| 5          | BUZZER         | BZ1 / ADXL375 INT1 / BMI088 INT3 ⚠️  |
| 6          | LED            | D1 via R11                           |
| 7          | ADXL_CS        | ADXL375 CS                           |
| 8, 34      | BMI088 signals | BMI088 ⚠️                            |
| 9, 33      | BMI088 INT2    | BMI088 ⚠️                            |
| 10         | SD_CS          | microSD                              |
| 11         | MOSI           | microSD / ADXL375                    |
| 12         | MISO           | microSD / ADXL375                    |
| 13         | SCK            | microSD / ADXL375                    |
| 14 (TX3)   | GPS_RX         | SAM-M8Q                              |
| 15 (RX3)   | GPS_TX         | SAM-M8Q                              |
| 16 (SCL1)  | I2C_SCL        | BNO055 / BMP390 ×2 / DS3231 / INA226 |
| 17 (SDA1)  | I2C_SDA        | BNO055 / BMP390 ×2 / DS3231 / INA226 |
| 22 (CTX1)  | CAN_TX         | TJA1051                              |
| 23 (CRX1)  | CAN_RX         | TJA1051                              |
| 26 (MOSI1) | BMI088_SDI     | BMI088                               |
| 27 (SCK1)  | BMI088_SCK     | BMI088                               |
| ON/OFF     | nRESET         | TPS3890                              |
| 3V3        | +3.3 V         | Board supply                         |
| GND        | GND            | Board ground                         |

⚠️ = requires review before fabrication.

---

# 9. I²C Address Map

| Device       | Address          | Configuration  |
| ------------ | ---------------- | -------------- |
| BNO055       | 0x29             | COM3 = 3.3 V   |
| BMP390 — U6  | 0x76             | SDO = GND      |
| BMP390 — U12 | 0x77             | SDO = 3.3 V    |
| DS3231       | 0x68             | Fixed          |
| INA226       | **Undefined ⚠️** | A0/A1 floating |

---

# 10. Known Issues / To-Do Before Ordering

> ⚠️ **The current PCB should not be sent for fabrication until the following issues have been reviewed and resolved.**

## 10.1 Critical Functional Issues

### 1. GPS Power

The SAM-M8Q is currently powered through **R3 = 4.7 kΩ**.

This is likely to prevent the GPS from receiving sufficient supply current.

**Action:** Replace R3 with **0 Ω** or an appropriate ferrite bead.

---

### 2. GPS VCC_IO and V_BCKP

The following SAM-M8Q pins are currently unconnected:

* VCC_IO
* V_BCKP

**Action:** Connect VCC_IO appropriately and either connect V_BCKP to the intended backup supply or explicitly terminate it according to the application requirements.

---

### 3. CAN Transceiver Power

The TJA1051 currently has unconnected:

* VCC
* VIO
* GND / exposed pad

The design also does not currently provide the required CAN transceiver supply rail.

**Action:** Resolve the CAN power architecture before fabrication.

---

### 4. BMI088 Interface

The BMI088 connections require significant review.

Current concerns include:

* CSB2, SDO1, SDO2 and INT1 sharing a net
* CSB1 tied directly to GND
* SDO lines not correctly connected to the intended SPI MISO path
* INT2 shorting Teensy pins 9 and 33

**Action:** Re-check the complete BMI088 SPI and interrupt wiring against the datasheet and intended Teensy SPI peripheral.

---

### 5. Buzzer / Interrupt Conflict

The buzzer output currently shares a net with:

* ADXL375 INT1
* BMI088 INT3
* Teensy pin 5

This can cause electrical conflicts between the buzzer driver and sensor interrupt outputs.

**Action:** Assign the buzzer and sensor interrupts to independent nets. A transistor driver should also be considered for the buzzer.

---

### 6. ADXL375 Ground Connections

ADXL375 ground pins 4 and 5 are currently unconnected.

**Action:** Connect all required GND pins according to the datasheet.

---

### 7. BNO055 Interface Configuration

The current PS0/PS1 configuration may select an unintended communication protocol.

**Action:** Verify the PS0/PS1 configuration against the desired standard I²C operating mode.

---

### 8. INA226

The INA226 is currently incomplete:

* A0/A1 are floating
* VBUS is unconnected
* IN+ is unconnected
* IN− is unconnected
* No current-sense shunt is implemented

**Action:** Either complete the power-monitoring circuit or remove the INA226 from the design.

---

### 9. DS3231 Backup Supply

The DS3231 VBAT pin is currently floating.

**Action:** Either provide a backup battery or connect VBAT appropriately according to the intended operating mode.

---

## 10.2 Design Review Items

### I²C Pull-Ups

Multiple 4.7 kΩ pull-ups appear on the I²C bus.

The resulting parallel resistance may be unnecessarily low.

**Action:** Populate a single appropriate pull-up pair and mark redundant footprints as **DNP**.

### Unspecified Resistor Values

The following components have incomplete values:

* R5
* R6
* R9
* R10
* R11

**Action:** Specify the required resistance or mark the component as DNP.

### Teensy ON/OFF Behaviour

The TPS3890 drives the Teensy ON/OFF pin.

**Action:** Confirm that the resulting power-off behaviour is acceptable, as the ON/OFF input does not function as a conventional reset pin.

### TPS3890 SENSE Network

R18 = 240 kΩ is connected to the TPS3890 SENSE network.

**Action:** Verify that this resistor does not unintentionally shift the intended voltage threshold.

### Decoupling

Review local decoupling for:

* DS3231
* INA226
* TJA1051
* SAM-M8Q
* microSD

### Passive Package Sizes

The current design uses very small passive packages, including **01005** components and a **0201 LED**.

**Action:** Consider 0402 packages if manual assembly is expected.

---

# 11. PCB Housekeeping

Before generating manufacturing files:

* [ ] Re-annotate mounting holes — duplicate `H2` designators exist.
* [ ] Add/fix missing `H1` designator.
* [ ] Remove stray via around `(132.4, 5.8)`.
* [ ] Remove duplicate GND vias around `(171, 44)`.
* [ ] Verify all mounting-hole locations.
* [ ] Verify all silkscreen labels.
* [ ] Check component courtyard clearance.
* [ ] Run **Electrical Rules Check (ERC)**.
* [ ] Run **Design Rules Check (DRC)**.
* [ ] Resolve all critical DRC/ERC errors.
* [ ] Generate and inspect Gerbers before fabrication.

---

# 12. Pre-Fabrication Checklist

### Electrical

* [ ] Power architecture verified
* [ ] GPS power verified
* [ ] CAN power verified
* [ ] CAN termination verified
* [ ] BMI088 SPI verified
* [ ] BMI088 interrupts verified
* [ ] BNO055 protocol configuration verified
* [ ] ADXL375 grounding verified
* [ ] INA226 completed or removed
* [ ] DS3231 backup supply resolved
* [ ] Buzzer/interrupt conflict resolved
* [ ] Protection requirements reviewed

### PCB

* [ ] Stackup confirmed with manufacturer
* [ ] 1 oz copper specified on all layers
* [ ] Surface finish specified
* [ ] Blind via removed or fabrication capability confirmed
* [ ] Board dimensions verified
* [ ] Mounting pattern finalised
* [ ] Connector footprints verified
* [ ] Component clearances checked
* [ ] DRC passed
* [ ] ERC passed

### Documentation

* [ ] BOM updated
* [ ] Part numbers verified
* [ ] Connector specifications confirmed
* [ ] Sensor list updated
* [ ] Revision number updated
* [ ] Schematic revision updated
* [ ] PCB revision updated
* [ ] README updated
* [ ] Gerbers generated
* [ ] Gerbers reviewed

---

