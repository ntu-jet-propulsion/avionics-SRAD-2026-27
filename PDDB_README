# PDDB — Power Distribution & Driver Board

## 1. Overview

The **Power Distribution & Driver Board (PDDB)** is a power-management and output-control PCB designed to distribute battery power, provide regulated low-voltage rails, communicate over CAN, and control external loads such as nichrome coils.

The board incorporates:

* Battery/power input connections
* High-current power distribution
* Fused input protection
* Buck-converter voltage regulation
* 3.3 V regulation
* CAN bus communication
* Current sensing
* MOSFET-based load switching
* Nichrome coil outputs
* External signal and ground connections

The design is implemented in **KiCad** and consists of both the schematic and PCB layout.

---

## 2. Project Files

| File                       | Description                     |
| -------------------------- | ------------------------------- |
| `PDDB Schematic.kicad_pro` | KiCad project configuration     |
| `PDDB Schematic.kicad_sch` | Electrical schematic            |
| `PDDB Schematic.kicad_pcb` | PCB layout                      |
| `PDDB Schematic-backups/`  | KiCad automatic project backups |
| `.history/`                | KiCad local history             |

### Current Design Version

**Version:** v0.x — Development

> The exact release/version number should be updated when this revision is formally pushed to the repository.

---

# 3. Functional Architecture

The PDDB can be broadly divided into the following functional blocks:

```text
                 ┌──────────────────────┐
Battery Input ──►│ Input Protection     │
                 │ Fuse / Protection    │
                 └──────────┬───────────┘
                            │
                            ▼
                 ┌──────────────────────┐
                 │ Power Distribution   │
                 │ & Current Sensing    │
                 └───────┬───────┬──────┘
                         │       │
              ┌──────────┘       └──────────────┐
              ▼                                 ▼
      ┌───────────────┐                 ┌────────────────┐
      │ Buck Converter│                 │ Load Switching │
      │ LM25085       │                 │ MOSFETs        │
      └───────┬───────┘                 └───────┬────────┘
              │                                 │
              ▼                                 ▼
         Regulated Rails                  Nichrome Coils

              │
              ▼
      ┌───────────────┐
      │ CAN Interface │
      │ TJA1051T      │
      └───────────────┘
```

---

# 4. Power Regulation

## 4.1 Main Buck Converter

The primary switching regulator is:

**U1 — LM25085MY**

The LM25085 is used as the main buck-converter controller.

Associated components include:

| Reference |     Value | Function                      |
| --------- | --------: | ----------------------------- |
| U1        | LM25085MY | Buck regulator controller     |
| L1        |    1.2 µH | Output inductor               |
| Rsen1     |     10 mΩ | Current-sense resistor        |
| C5        |     33 µF | Output filtering              |
| C2        |    100 µF | Bulk filtering                |
| R6        |   3.32 kΩ | Feedback/control network      |
| Radj1     |   2.55 kΩ | Adjustable regulation network |
| Radj2     |   2.55 kΩ | Adjustable regulation network |
| C3        |    470 nF | Filtering/compensation        |

The exact regulated output voltage and maximum current should be confirmed against the latest power-budget calculations before fabrication.

---

## 4.2 3.3 V Regulation

**VR1 — LM3940IMP-3.3_NOPB**

The LM3940 is used to provide a regulated **3.3 V rail** for low-voltage circuitry.

Associated capacitor:

* **C4 — 3.3 µF**

The 3.3 V rail is intended for the board's low-voltage electronics and control/interface circuitry.

---

# 5. Input and Protection

The PDDB includes input protection to reduce the risk of damage caused by electrical faults.

## 5.1 Fuse

**F1 — Fuse**

The fuse provides overcurrent protection on the power input.

The final fuse rating should be selected based on:

* Maximum expected operating current
* Normal startup/inrush current
* Nichrome load current
* PCB trace/current capacity
* Connector current rating
* Power supply/battery specifications

**Fuse rating: TBD / confirm before release**

---

## 5.2 Reverse/Transient Protection

The design includes:

**D1 — Schottky diode**

The Schottky diode provides protection within the power-input/control circuitry.

The exact protection behaviour should be verified against the final schematic and intended fault cases before the production release.

---

## 5.3 Bulk and Local Decoupling

The board contains multiple capacitors for:

* Input bulk energy storage
* Switching regulator filtering
* Local supply decoupling
* Noise reduction

Relevant components include:

* C1 — 1.6 nF
* C2 — 100 µF
* C3 — 470 nF
* C4 — 3.3 µF
* C5 — 33 µF
* Cadj1 — 1 nF
* Cadj2 — 1 nF
* Cadj3 — 1 nF

---

# 6. CAN Communication

## CAN Transceiver

**U2 — TJA1051T**

The TJA1051T provides the physical-layer interface for CAN communication.

The board exposes:

* **CANH**
* **CANL**

The schematic also includes:

**R4 — 120 Ω**

This resistor is associated with CAN bus termination.

### CAN Connector

The CAN interface is exposed through the board's connector/interface circuitry.

The final system configuration should determine whether the board is intended to provide a permanent 120 Ω termination or whether termination should be selectable.

---

# 7. Load / Nichrome Control

The PDDB provides switched outputs for nichrome-based loads.

Two dedicated connectors are present:

| Reference | Label           | Purpose                  |
| --------- | --------------- | ------------------------ |
| J7        | `nichrome coil` | Nichrome load connection |
| J8        | `nichrome coil` | Nichrome load connection |

The load switching circuitry uses MOSFETs including:

* **Q2 — AO3400A**
* **Q3 — AO3400A**
* **U4 — CSD25402Q3A**

These devices provide electronic switching of the connected loads.

The exact continuous and peak current limits must be verified from:

1. MOSFET SOA/current rating
2. PCB copper capacity
3. Connector rating
4. Thermal performance
5. Nichrome resistance
6. Supply voltage

---

# 8. Connectors

The current schematic contains the following external connectors/interfaces.

| Reference | Label / Value       | Purpose                   |
| --------- | ------------------- | ------------------------- |
| J1        | `Conn_01x02`        | 2-pin external connection |
| J2        | `Battery and input` | Battery/power input       |
| J3        | `Conn_01x01_Pin`    | Single-pin connection     |
| J4        | `Conn_01x02`        | 2-pin external connection |
| J5        | `Battery and input` | Battery/power input       |
| J6        | `Conn_01x01_Pin`    | Single-pin connection     |
| J7        | `nichrome coil`     | Nichrome load output      |
| J8        | `nichrome coil`     | Nichrome load output      |

### Connector Design Rationale

The connectors are provided to separate the major external interfaces of the PDDB:

* **Battery/input connectors** provide the primary power connection.
* **Nichrome connectors** provide dedicated high-current load connections.
* **2-pin connectors** provide signal/power connections where both signal and return are required.
* **Single-pin connectors** provide dedicated signal/test/auxiliary connections where a separate ground connection is not required at the connector.

> The exact mating connector part numbers should be added to the BOM before the PCB is released for manufacturing.

---

# 9. Current Sensing

The board contains:

**Rsen1 — 10 mΩ**

This low-value shunt resistor is used for current sensing within the power stage.

The shunt allows the voltage developed across the resistor to be used to determine current:

$$
I = \frac{V_{sense}}{R_{sense}}
$$

For the installed 10 mΩ resistor:

$$
I = \frac{V_{sense}}{0.010}
$$

Therefore, a 10 mV voltage drop corresponds to approximately:

$$
I = 1\,A
$$

and a 100 mV drop corresponds to:

$$
I = 10\,A
$$

The final measurable current range depends on the sensing circuitry and ADC/interface used by the overall system.

---

# 10. Sensor / Monitoring Interfaces

The current PDDB design should be treated as a **power and driver board rather than a dedicated sensor board**.

The following monitoring-related functionality is present:

| Function                  | Component / Interface                          | Status  |
| ------------------------- | ---------------------------------------------- | ------- |
| Current sensing           | Rsen1, 10 mΩ shunt                             | Present |
| CAN communication         | U2, TJA1051T                                   | Present |
| Supply voltage monitoring | External/system-dependent                      | Confirm |
| Temperature sensing       | Not explicitly identified in current schematic | TBD     |
| Current/voltage ADC       | External MCU/system-dependent                  | TBD     |

### Sensor List

Before the README is considered final, the project owner should confirm the complete system-level sensor list.

At minimum, document:

* Current sensor
* Voltage sensor
* Temperature sensor(s), if present
* Any external sensors connected through J1/J3/J4/J6
* CAN-connected sensors, if applicable

**Do not add a sensor to this list unless it is actually implemented or explicitly specified for the final system.**

---

# 11. PCB Stackup

The current KiCad PCB file does not contain a formal fabrication stackup definition.

The current board is therefore documented as:

**PCB type:** TBD
**Number of copper layers:** TBD
**Board thickness:** 1.6 mm nominal in the current KiCad board setup
**Copper weight:** TBD

### Recommended README entry after fabrication specification is confirmed

```text
Layer 1: Top Copper — TBD oz
Layer 2: Bottom Copper — TBD oz

Board thickness: 1.6 mm
Copper weight: TBD
Surface finish: TBD
Solder mask: TBD
Silkscreen: TBD
```

The final stackup should be confirmed with the PCB manufacturer before release.

---

# 12. Copper Weight

The copper weight has not been explicitly specified in the current KiCad PCB file.

**Current status: TBD**

Copper weight should be selected based on the maximum current carried by:

* Battery input traces
* Main power distribution paths
* Nichrome outputs
* Buck-converter power path

High-current traces should also be checked for:

* Trace width
* Temperature rise
* Via current capacity
* Connector current rating
* Copper thickness
* Continuous vs transient current

For the production release, the selected copper weight should be explicitly recorded here.

---

# 13. PCB Dimensions

The final mechanical dimensions should be recorded here once the Edge.Cuts outline has been finalized.

**Current board dimensions: TBD**

Required information:

| Parameter               | Value          |
| ----------------------- | -------------- |
| Length                  | TBD mm         |
| Width                   | TBD mm         |
| Thickness               | 1.6 mm nominal |
| Mounting-hole diameter  | TBD mm         |
| Mounting-hole locations | TBD            |
| Connector clearance     | TBD            |
| Component height limit  | TBD            |

The mechanical drawing should be checked before fabrication to ensure compatibility with the intended enclosure/system.


