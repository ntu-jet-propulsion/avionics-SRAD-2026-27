# TLB 

A **4-layer, 50 × 50 mm PCB** designed around an **ESP32-C3**. The TLB provides UART telemetry, CAN communication, non-volatile EEPROM storage, USB connectivity, and regulated 3.3 V / 5 V power domains.

> **Status:** Prototype / Pre-Fabrication
> Review the [Power Architecture](#8-power-architecture) section before fabrication, particularly the unresolved +5 V supply for the CAN transceiver.

---

# 1. Layers and Stackup

The TLB is a **4-layer PCB** with a nominal finished thickness of **1.565 mm**.

| Layer              | Material |    Thickness | Purpose                                |
| ------------------ | -------- | -----------: | -------------------------------------- |
| Top silkscreen     | —        |            — | Reference designators and labels       |
| Top solder mask    | —        |            — | Component-side protection              |
| **1 — F.Cu**       | Copper   |     0.035 mm | Components and primary signal routing  |
| Prepreg            | FR4      |      0.10 mm | Dielectric                             |
| **2 — In1.Cu**     | Copper   |    0.0175 mm | Split **+3V3 / +5V power plane**       |
| Core               | FR4      |      1.24 mm | Dielectric                             |
| **3 — In2.Cu**     | Copper   |    0.0175 mm | **Solid GND plane**                    |
| Prepreg            | FR4      |      0.10 mm | Dielectric                             |
| **4 — B.Cu**       | Copper   |     0.035 mm | Debugging and secondary signal routing |
| Bottom solder mask | —        |            — | Bottom-side protection                 |
| **Total**          |          | **1.565 mm** |                                        |

### Layer Strategy

* **F.Cu:** Components and primary signal routing
* **In1.Cu:** Split power plane containing **+3V3** and **+5V**
* **In2.Cu:** Solid **GND** plane
* **B.Cu:** Secondary/debug signal routing

Unlike a board using both inner layers as ground, the TLB deliberately uses **In1.Cu as a power plane** because the design contains both 3.3 V and 5 V circuitry.

### ESP32 Antenna Keepout

A dedicated antenna keepout is maintained around the ESP32.

> ⚠️ **Do not route signals or place copper/components inside the ESP32 antenna keepout region.** Maintaining this clearance is important for reliable RF performance.

---

# 2. Routing Rules

The current PCB design uses the following routing constraints:

| Rule                     |      TLB Value |
| ------------------------ | -------------: |
| Minimum clearance        |        0.20 mm |
| Minimum track width      |        0.20 mm |
| Default signal track     |        0.25 mm |
| Power track              |        0.50 mm |
| Minimum via diameter     |        0.50 mm |
| Default via              | 0.60 / 0.30 mm |
| Power via                | 0.80 / 0.40 mm |
| Minimum drill            |        0.30 mm |
| Copper-to-edge clearance |        0.50 mm |
| Hole-to-hole clearance   |        0.25 mm |
| Minimum annular width    |        0.10 mm |

## Net Classes

### Default

| Parameter   |          Value |
| ----------- | -------------: |
| Clearance   |        0.20 mm |
| Track width |        0.25 mm |
| Via         | 0.60 / 0.30 mm |

### Power

| Parameter     |          Value |
| ------------- | -------------: |
| Clearance     |        0.20 mm |
| Track width   |        0.50 mm |
| Via           | 0.80 / 0.40 mm |
| Assigned nets |      +3V3, +5V |

---

# 3. Copper Weights

The PCB uses **1 oz outer copper** and **0.5 oz inner copper**.

| Layer  | Thickness | Copper Weight |
| ------ | --------: | ------------: |
| F.Cu   |     35 µm |      **1 oz** |
| In1.Cu |   17.5 µm |    **0.5 oz** |
| In2.Cu |   17.5 µm |    **0.5 oz** |
| B.Cu   |     35 µm |      **1 oz** |

This matches the intended PCB fabrication stackup.

---

# 4. Connectors and External Interfaces

The TLB provides the following main external interfaces:

| Ref.   | Connector         | Purpose                                 |
| ------ | ----------------- | --------------------------------------- |
| **J1** | 1×2 `Power_In`    | +3V3 power input and GND                |
| **J2** | 1×4 `Light_APRS`  | UART telemetry interface                |
| **J3** | 1×2 CAN connector | CANH and CANL                           |
| **J5** | Micro-USB         | USB, VBUS and shield                    |
| —      | ESP32-C3          | Main controller and telemetry processor |

## J1 — Power Input

| Pin | Signal |
| --- | ------ |
| 1   | +3V3   |
| 2   | GND    |

**C2 = 33 µF** provides bulk capacitance close to the power input.

---

## J2 — Light APRS

| Pin | Signal |
| --- | ------ |
| 1   | +3V3   |
| 2   | GND    |
| 3   | TX0    |
| 4   | RX0    |

J2 provides the UART interface to the **Light APRS** system.

---

## J3 — CAN

| Pin | Signal |
| --- | ------ |
| 1   | CANH   |
| 2   | CANL   |

**R4 = 120 Ω** provides CAN bus termination.

---

## J5 — Micro-USB

The USB interface provides:

* **VBUS**
* **GND**
* **USB shield**

Current connections:

| USB Signal | Connection    |
| ---------- | ------------- |
| VBUS       | Net-(J5-VBUS) |
| GND        | GND           |
| Shield     | R6 → GND      |

Additional components:

* **R6 = 0 Ω** — USB shield to GND
* **C13 = 0.1 µF** — VBUS-related capacitor
* **C12 = 0.1 µF** — GND-related capacitor

---

# 5. Main Components

## 5.1 ESP32-C3 — U2

The **ESP32-C3** is the main microcontroller for the TLB.

It handles:

* Telemetry processing
* UART communication
* CAN communication
* EEPROM access
* Data processing and logging

The ESP32 operates from the **+3V3** rail.

### Relevant Interfaces

| ESP32 Signal | Function         |
| ------------ | ---------------- |
| IO21 / TXD   | I²C SCL          |
| IO20 / RXD   | I²C SDA          |
| IO4          | CAN-related      |
| IO5          | CAN-related      |
| UART0 TX     | TX0 → Light APRS |
| UART0 RX     | RX0 ← Light APRS |

> The ESP32 antenna keepout must remain free of copper, routing and components.

---

## 5.2 EEPROM — U3

**U3 — 24LC256**

The 24LC256 is a **256-kbit I²C EEPROM** used for non-volatile data storage.

### Connections

* VCC → +3V3
* GND → GND
* SDA → ESP32 SDA
* SCL → ESP32 SCL

### Supporting Components

| Component |  Value | Function    |
| --------- | -----: | ----------- |
| R1        | 4.7 kΩ | I²C pull-up |
| R3        | 4.7 kΩ | I²C pull-up |
| C5        | 0.1 µF | Decoupling  |
| C6        | 0.1 µF | Decoupling  |

---

## 5.3 CAN Transceiver — U4

**U4 — TJA1051TK/3**

The TJA1051TK/3 provides the physical-layer interface between the ESP32 and the CAN bus.

### Pin Connections

| Pin | Function | TLB Connection |
| --: | -------- | -------------- |
|   1 | TXD      | CANTX          |
|   2 | GND      | GND            |
|   3 | VCC      | +5V            |
|   4 | RXD      | CANRX          |
|   5 | VIO      | +3V3           |
|   6 | CANL     | CANL           |
|   7 | CANH     | CANH           |
|   8 | S        | R2             |

### Power Architecture

The TJA1051TK/3 uses:

* **+5V for VCC**
* **+3V3 for VIO**

This allows the CAN transceiver to interface with the 3.3 V ESP32 logic while operating its main supply from 5 V.

### Supporting Components

| Component |  Value | Function          |
| --------- | -----: | ----------------- |
| C7        | 0.1 µF | Decoupling        |
| C8        | 0.1 µF | Decoupling        |
| C9        | 0.1 µF | Decoupling        |
| R2        |  10 kΩ | CAN transceiver S |
| R4        |  120 Ω | CAN termination   |

---

# 6. Component Summary

| Ref.         | Component      | Function                   |
| ------------ | -------------- | -------------------------- |
| **U2**       | ESP32-C3       | Main MCU                   |
| **U3**       | 24LC256        | Non-volatile EEPROM        |
| **U4**       | TJA1051TK/3    | CAN transceiver            |
| **J1**       | 1×2 Power_In   | +3V3 + GND input           |
| **J2**       | 1×4 Light_APRS | UART telemetry             |
| **J3**       | 1×2 CAN        | CANH / CANL                |
| **J5**       | Micro-USB      | USB interface              |
| **C2**       | 33 µF          | Bulk supply capacitor      |
| **C3/C4**    | 0.1 µF         | ESP32 decoupling           |
| **C5/C6**    | 0.1 µF         | EEPROM decoupling          |
| **C7/C8/C9** | 0.1 µF         | CAN transceiver decoupling |
| **C12/C13**  | 0.1 µF         | USB-related capacitors     |
| **R1/R3**    | 4.7 kΩ         | I²C pull-ups               |
| **R2**       | 10 kΩ          | CAN transceiver S          |
| **R4**       | 120 Ω          | CAN termination            |
| **R5**       | 10 kΩ          | ESP32 EN pull-up           |
| **R6**       | 0 Ω            | USB shield → GND           |

---

# 7. Interfaces and Signal Map

## I²C

The ESP32 communicates with the 24LC256 over I²C.

```text
ESP32-C3
   │
   ├── SCL ───────► U3 SCL
   │                 │
   │              R1 = 4.7 kΩ
   │                 │
   │               +3V3
   │
   └── SDA ───────► U3 SDA
                     │
                  R3 = 4.7 kΩ
                     │
                   +3V3
```

---

## CAN

The ESP32 communicates with the external CAN bus through U4.

```text
ESP32 CANTX
     │
     ▼
TJA1051 TXD
     │
     ▼
 CAN Transceiver
     │
     ├────────► CANH ──────► J3
     │
     └────────► CANL ──────► J3
```

The receive path is:

```text
J3 CANH / CANL
      │
      ▼
TJA1051
      │
      ▼
ESP32 CANRX
```

**R4 = 120 Ω** provides CAN termination between CANH and CANL.

---

## UART / Light APRS

```text
ESP32 UART0 TX ─────► J2 TX0

ESP32 UART0 RX ◄───── J2 RX0
```

J2 also provides +3V3 and GND.

---

## USB

```text
J5 Micro-USB
   │
   ├── VBUS
   ├── GND
   └── Shield
          │
        R6 = 0 Ω
          │
         GND
```

---

# 8. Power Architecture

The TLB contains the following power domains:

## +3V3

The +3V3 rail supplies:

* ESP32-C3
* 24LC256
* TJA1051 VIO
* I²C pull-ups
* J2
* Local decoupling capacitors

## +5V

The +5V rail is intended to supply:

* **TJA1051 VCC**

## GND

Ground is implemented primarily through the **In2.Cu solid ground plane**.

Ground vias connect component and connector ground pins to the plane.

---

## ⚠️ +5V Supply — To Be Confirmed

The current PCB contains a **+5V copper region**, but the source of the +5V rail is not clearly defined in the current connector arrangement.

Currently:

* **J1** provides +3V3
* **J5 VBUS** is on `Net-(J5-VBUS)`
* **U4 VCC** requires +5V

Therefore, the source and regulation path for +5V must be confirmed before fabrication.

> **Action required:** Confirm with the team whether +5V is generated elsewhere, supplied through USB VBUS, or requires an onboard regulator.

Do **not** assume the +5V power architecture is complete until this has been verified.

---

# 9. Board Dimensions

| Specification     |                  TLB |
| ----------------- | -------------------: |
| Board width       |          **50.0 mm** |
| Board height      |          **50.0 mm** |
| Nominal thickness |         **1.565 mm** |
| Number of layers  |                **4** |
| Component side    |                  Top |
| Mounting holes    | **2 × Ø5.0 mm NPTH** |

### Board Outline

The PCB outline is:

```text
X = 100 → 150 mm
Y = 60 → 110 mm
```

Therefore:

**Board size = 50 × 50 mm**

---

# 10. Mounting

The TLB currently contains:

* **2 × Ø5.0 mm non-plated through holes (NPTH)**

Verify the final mounting-hole positions and mechanical clearances against the intended enclosure or mounting structure before fabrication.

---

# 11. Protection and Design Features

## Local Decoupling

Local **0.1 µF decoupling capacitors** are provided around:

* ESP32-C3
* 24LC256
* TJA1051
* USB interface

These capacitors provide local high-frequency supply filtering and reduce supply noise.

## CAN Termination

**R4 = 120 Ω** is connected between CANH and CANL.

This provides termination when the TLB is installed at an end of the CAN bus.

> If the TLB is used as an intermediate CAN node, the termination resistor should be made removable or otherwise disabled.

## Ground Plane

**In2.Cu** is configured as a solid GND plane.

Ground vias connect the component and connector ground connections to the plane, providing low-impedance return paths.

## ESP32 Antenna Keepout

A dedicated keepout region is maintained around the ESP32 antenna.

No routing, copper pours or components should be placed within this region.

## USB Shield Grounding

**R6 = 0 Ω** connects the USB shield to GND.

This provides a defined shield-ground connection while allowing the connection to be modified during hardware testing if required.

---

# 12. Pre-Fabrication Checklist

Before sending the TLB for fabrication:

### PCB

* [ ] Confirm 4-layer stackup with PCB manufacturer
* [ ] Confirm 1 oz outer / 0.5 oz inner copper
* [ ] Confirm 1.565 mm finished thickness is supported
* [ ] Verify 50 × 50 mm board outline
* [ ] Verify mounting-hole locations
* [ ] Verify ESP32 antenna keepout
* [ ] Check all connector footprints
* [ ] Run DRC
* [ ] Resolve all critical DRC violations

### Electrical

* [ ] Confirm +3V3 supply
* [ ] Confirm +5V supply source
* [ ] Verify TJA1051 VCC = +5V
* [ ] Verify TJA1051 VIO = +3V3
* [ ] Verify CAN termination
* [ ] Verify I²C pull-ups
* [ ] Verify UART TX/RX connections
* [ ] Verify USB VBUS routing
* [ ] Verify all decoupling capacitors

### Documentation

* [ ] Update schematic revision
* [ ] Update PCB revision
* [ ] Update BOM
* [ ] Confirm connector part numbers
* [ ] Confirm fabrication stackup
* [ ] Confirm unresolved issues are closed
* [ ] Generate and review Gerber files

---


