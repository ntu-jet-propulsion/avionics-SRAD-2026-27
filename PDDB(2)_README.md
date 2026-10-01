[PDDB.md](https://github.com/user-attachments/files/32892513/PDDB.md)
# PDDB — Power Distribution & Driver Board

A **4-layer, 33.1 × 27.0 mm power board** combining a battery input with fuse, an **LM25085 P-FET buck controller**, an **LM3940 3.3 V LDO**, a **TJA1051 CAN transceiver**, and **two low-side AO3400A MOSFET drivers for nichrome coils**.

> **Status:** ⚠️ Prototype / Pre-Fabrication
> The layout is complete and passes KiCad DRC with **0 errors, 0 warnings and 0 unconnected items**. The **schematic still has functional errors**, though. Read the [Known Issues](#10-known-issues--to-do-before-ordering) section before sending the design for fabrication.

---

## 1. Board Overview

### Key Features

* Battery / power input through a screw terminal (J1) and two 1×4 headers (J2, J5)
* Fused battery path (F1)
* LM25085 constant-on-time P-FET buck controller with CSD25402Q3A P-MOSFET and 10 mΩ current sense
* LM3940 3.3 V low-dropout regulator
* TJA1051T CAN transceiver with on-board 120 Ω termination
* Two AO3400A low-side switches for nichrome coils (J7, J8)
* 4-layer PCB with a dedicated solid GND plane and a VCC power plane
* All components mounted on the top side (single-sided SMT assembly)

### Layout Statistics

| Parameter                 | Value                         |
| ------------------------- | ----------------------------- |
| Board size                | **33.1 × 27.0 mm** (894 mm²)  |
| Copper layers             | 4                             |
| Components                | 35                            |
| Routable nets             | 19 (100 % routed)             |
| Unrouted nets             | 0                             |
| Total vias                | 63 (6 signal/power, 57 GND)   |
| Total track length        | ≈ 320 mm                      |
| DRC errors / warnings     | **0 / 0**                     |
| Schematic parity issues   | 0                             |
| EDA tool                  | KiCad 10.0                    |

---

## 2. PCB Stackup

The PCB uses a **4-layer stackup** with a nominal finished thickness of **1.6 mm**.

| Layer   | Type   | Thickness | Purpose                                                          |
| ------- | ------ | --------: | ---------------------------------------------------------------- |
| Top     | F.Cu   |     35 µm | Components, signal and power routing, GND pour                   |
| Inner 1 | In1.Cu |     35 µm | **Solid GND plane**                                              |
| Inner 2 | In2.Cu |     35 µm | **VCC power plane** (nichrome coil supply) + secondary routing   |
| Bottom  | B.Cu   |     35 µm | Secondary signal routing, GND pour                               |

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

> **Note:** These are KiCad's default 4-layer stackup values. The board file does not store an explicit stackup. Define it in *Board Setup → Physical Stackup* and confirm it with the manufacturer.

### Layer Strategy

Components and most routing are on F.Cu. **In1.Cu is an unbroken GND plane** directly under the top layer, so every top-layer signal has a reference plane immediately beneath it.

This provides:

* Short, low-impedance return paths
* Low loop area in the buck converter's switching path
* Reduced EMI
* Shielding between the top layer and the inner power plane

**In2.Cu is a VCC plane.** VCC is the nichrome coil supply. It reaches J2, J5, J7 and J8 through the plane and through 1.0 mm traces. A single Vbat trace and a single U1-VCC trace also run on In2.

**F.Cu and B.Cu carry GND pours** with island removal. Stitching vias on a ≥ 3.5 mm grid tie the pours to the inner GND plane.

### Routing Rules

| Net class | Nets                                                                 | Track width | Via (pad / drill) |
| --------- | -------------------------------------------------------------------- | ----------: | ----------------: |
| Power     | VCC, J7-Pin2, J8-Pin2 (coil supply and returns)                      |     1.00 mm |   0.80 / 0.40 mm |
| Supply    | Vbat, SW node (D1-K), Vout (C1-Pad1), +3.3 V, J2-Pin1, sense+        |     0.50 mm |   0.60 / 0.30 mm |
| Sense     | U1-ISEN                                                              |     0.40 mm |   0.60 / 0.30 mm |
| Gnd       | GND fan-outs (SMD pad → plane)                                       |     0.40 mm |   0.60 / 0.30 mm |
| Default   | CANH, CANL, gate drive, ADJ, feedback                                |     0.25 mm |   0.60 / 0.30 mm |

| Parameter            | Specification                                         |
| -------------------- | ----------------------------------------------------- |
| Minimum clearance    | 0.20 mm (0.12 mm local override on U4 pads 1–4)       |
| Copper-to-edge       | ≥ 0.50 mm                                             |
| Standard via         | 0.60 mm pad / 0.30 mm drill                           |
| Small via            | 0.50 mm pad / 0.25 mm drill (3 vias, U1–U4 channel)   |
| Minimum drill        | **0.20 mm** (U1 footprint thermal vias)               |
| Minimum annular ring | 0.125 mm                                              |
| Via type             | Through-hole only (no blind / buried vias)            |
| Surface finish       | **ENIG recommended**                                  |

> **Fabrication note:** U1's library footprint (MSOP-8-1EP with thermal vias) has **0.20 mm drills**, so the board minimum hole size is set to 0.20 mm. Most 4-layer fab services support this, but check before ordering.
>
> **Fabrication note:** The U4 (VSON-8 NexFET) library footprint has a 0.14 mm pad-to-pad gap at 0.65 mm pitch. A 0.12 mm local clearance is applied to those pads only. Confirm that the fab's minimum spacing is ≤ 0.12 mm.

### Surface Finish

**ENIG (Electroless Nickel Immersion Gold)** is recommended. The board has fine-pitch leadless packages (VSON-8, MSOP-8 with exposed pad) that benefit from a flat, consistent surface.

---

## 3. Copper Weight

All four copper layers use **1 oz copper (35 µm)**.

| Layer  | Copper Thickness | Copper Weight |
| ------ | ---------------: | ------------: |
| F.Cu   |            35 µm |          1 oz |
| In1.Cu |            35 µm |          1 oz |
| In2.Cu |            35 µm |          1 oz |
| B.Cu   |            35 µm |          1 oz |

Most of the circuitry carries low current: the controller, LDO, CAN and gate drive.

The **nichrome coil paths** are different. They run VCC → J7/J8 → coil → Q3/Q2 → GND and can carry several amps.

* VCC is carried by the In2 plane plus 1.0 mm traces.
* Drain traces Q3 → J7.2 and Q2 → J8.2 are **1.0 mm wide and only ≈ 3 mm long**.
* A 1.0 mm, 1 oz external trace handles roughly **2.5–3 A at a 10 °C rise** (IPC-2221 estimate).

> **Fabrication note:** If the coil firing current exceeds ≈ 3 A continuous, specify **2 oz outer copper**. Firing pulses are short, which also helps. Some manufacturers default to 0.5 oz on inner layers, so specify **1 oz on all four layers** to match the design.

---

## 4. Connectors and Interfaces

| Ref.   | Connector / Interface                           | Purpose                                   |
| ------ | ----------------------------------------------- | ----------------------------------------- |
| **J1** | Phoenix PT 1,5/2-5.0-H, 2-pin screw terminal    | Battery input (GND / VBAT)                |
| **J2** | 1×4, 2.54 mm header                             | Battery and input (fused path)            |
| **J5** | 1×4, 2.54 mm header                             | Battery and input (parallel to J2)        |
| **J4** | 1×2, 2.54 mm header                             | CANH / CANL                               |
| **J7** | 1×2, 2.54 mm header                             | Nichrome coil 1 (driven by Q3)            |
| **J8** | 1×2, 2.54 mm header                             | Nichrome coil 2 (driven by Q2)            |
| **J3** | 1×1, 2.54 mm pin                                | Unconnected in schematic                  |
| **J6** | 1×1, 2.54 mm pin                                | Unconnected in schematic                  |

### J1 — Battery Terminal

The screw terminal takes the main battery input. Its wire-entry face is flush with the **bottom board edge**, and silkscreen labels **GND** (pin 1) and **VBAT** (pin 2) mark the polarity.

> ⚠️ J1 pin 2 connects to Vbat **directly, without going through F1**. Only the J2/J5 path is fused.

### J2 / J5 — Battery and Input Headers

J2 and J5 are wired in parallel:

* Pin 1 is the battery input. It passes through **F1** to Vbat and also biases the coil-driver gates through R1/R5.
* Pin 2 is the VCC coil supply.
* Pins 3–4 are GND.

### J4 — CAN Bus

J4 provides the external **CANH/CANL** connection to other avionics modules.

A **120 Ω termination resistor (R4)** is fitted, so this board is intended to sit at one end of the CAN bus.

> ⚠️ **Review:** J4 only provides CANH and CANL. A shared ground reference is recommended between CAN nodes. Consider a **3-pin CANH/CANL/GND connector**.

### J7 / J8 — Nichrome Coils

Each coil connects between **VCC (pin 1)** and a **low-side AO3400A drain (pin 2)**. Each MOSFET sits directly beside its connector, keeping the switched loop short.

For flight use, consider replacing the 2.54 mm headers with locking connectors such as **JST-GH** or **Molex Pico-Lock**.

### Deliberately Absent Interfaces

The board currently does not include:

* **Microcontroller interface:** the coil drivers have no logic-level control input (see Known Issues).
* **CAN TXD/RXD connection:** the transceiver's logic pins are not brought out.
* **Mounting holes.**

---

## 5. Functional Blocks and Key Devices

| Ref.      | Device             | Function                                   | Package                 |
| --------- | ------------------ | ------------------------------------------ | ----------------------- |
| **U1**    | TI LM25085MY       | P-FET buck controller                      | MSOP-8-EP (PowerPAD)    |
| **U4**    | TI CSD25402Q3A     | P-channel MOSFET (buck high-side switch)   | VSON-8 3.3 × 3.3 NexFET |
| **Rsen1** | 10 mΩ              | Buck current-sense resistor                | 2512                    |
| **L1**    | 1.2 µH             | Buck inductor                              | 7.3 × 7.3 × 4.5 mm      |
| **D1**    | Schottky           | Buck freewheel diode                       | SOD-523                 |
| **C2**    | 100 µF             | Buck output bulk capacitor                 | Elec. 6.3 × 5.4 mm      |
| **VR1**   | TI LM3940IMP-3.3   | 3.3 V LDO                                  | SOT-223                 |
| **U2**    | NXP TJA1051T       | High-speed CAN transceiver                 | SOIC-8                  |
| **Q2/Q3** | AO3400A            | N-channel low-side coil switches           | SOT-23                  |
| **F1**    | Fuse               | Input protection for the J2/J5 path        | 1206                    |

### Buck Converter (U1, U4, Rsen1, L1, D1, C2)

The LM25085 drives the CSD25402Q3A P-FET through **PGATE**. Current is sensed across **Rsen1 (10 mΩ)**: the sense+ node goes to ADJ via Radj1/Cadj1, and ISEN connects at Rsen1 pad 1.

Layout notes:

* The gate-drive trace (U1.6 → U4.4) is **3.0 mm** long.
* The ADJ trace is **3.6 mm** long. Radj1/Cadj1 sit right beside U1 pin 1.
* The switch node (U4 drain → D1 → L1) is a short **1.0 mm** copper path.
* C2 sits directly under L1's output pad, and C3 is close by.
* U1's VIN, VCC and ISEN pins escape through three 0.5 mm vias in a 1.6 mm channel between U1 and U4.

> ⚠️ The buck converter is **not functional as drawn** (see Known Issues 1 and 3).

### 3.3 V LDO (VR1, C5, C4)

The LM3940 provides a 3.3 V rail, with **C5 (33 µF)** as output capacitor.

> ⚠️ VR1's input pin is tied to GND in the schematic (see Known Issues 2).

### CAN Transceiver (U2, R4)

U2 sits directly above J4. Its CANL and CANH pins (6 and 7) line up with J4 pins 2 and 1, so the bus traces need no crossings. **R4 = 120 Ω** terminates the bus.

### Nichrome Coil Drivers (Q2, Q3, R1–R3, R5)

| Coil | Connector | MOSFET | Gate resistor | Pulled-to-GND resistor |
| ---- | --------- | ------ | ------------- | ---------------------- |
| 1    | J7        | Q3     | R5 (10 kΩ)    | —                      |
| 2    | J8        | Q2     | R1 (10 kΩ)    | —                      |

R2 and R3 (10 kΩ) connect **J2-Pin1 directly to GND**, not to the gates (see Known Issues 6).

---

## 6. Protection and Reliability

## Implemented Protection

| Feature             | Device / Implementation         | Purpose                                  |
| ------------------- | ------------------------------- | ---------------------------------------- |
| Input fuse          | F1 (1206)                       | Protects the J2/J5 battery path          |
| Current sensing     | Rsen1 + LM25085 ADJ             | Buck cycle-by-cycle current limit        |
| CAN protection      | TJA1051                         | Bus-fault tolerance, thermal shutdown    |
| CAN termination     | R4 = 120 Ω                      | Bus termination                          |
| Ground plane        | In1.Cu (solid) + F/B GND pours  | Low-impedance return, EMI reduction      |
| Power plane         | In2.Cu VCC                      | Low-impedance coil supply                |
| Local decoupling    | Cadj2/Cadj3, C3, C5             | Supply filtering                         |

### Fuse

**F1** sits between J2/J5 pin 1 and the Vbat net.

> ⚠️ The J1 screw terminal feeds Vbat **directly and bypasses F1**. Consider moving F1 so it protects all inputs.

### CAN Protection

**U2 — TJA1051T** provides built-in protection against bus faults.

> ⚠️ No external CAN ESD/TVS protection is fitted. Because J4 connects to an external cable, consider adding a CAN TVS (e.g. PESD2CAN).

### Ground and Power Planes

* **In1.Cu** is a solid GND plane. The only openings are via and pad clearances.
* Every SMD ground pad has its **own fan-out via** to In1.
* **F.Cu and B.Cu** carry GND pours, stitched to In1 with vias.
* **In2.Cu** is the VCC (coil supply) plane.

---

## Protection Not Currently Implemented

The following protection features are absent or incomplete:

* ❌ Reverse-polarity protection
* ❌ Fuse on the J1 input path
* ❌ CAN external TVS/ESD protection
* ❌ Gate pull-down resistors on Q2/Q3 (coils could turn on from a floating gate)
* ❌ Gate-voltage clamp on Q2/Q3: AO3400A V<sub>GS</sub> max is ±12 V, and the gates are fed from the battery input
* ❌ Flyback / snubber across the coil outputs
* ❌ Input TVS / transient protection

Review these against the final battery voltage and system requirements.

---

## 7. Board Dimensions

| Parameter             | Specification            |
| --------------------- | ------------------------ |
| Board size            | **33.1 × 27.0 mm**       |
| Board thickness       | **1.6 mm**               |
| Board shape           | Rectangular              |
| Component placement   | Top side only            |
| Mounting holes        | **None** (not in schematic) |

### Connector Locations

Coordinates are pin-1 positions, measured from the **top-left corner** of the PCB:

| Connector | Edge   |       X |       Y |
| --------- | ------ | ------: | ------: |
| J7        | Right  | 31.3 mm |  1.8 mm |
| J8        | Right  | 31.3 mm |  8.0 mm |
| J5        | Right  | 31.3 mm | 17.6 mm |
| J1        | Bottom |  3.0 mm | 21.0 mm |
| J2        | Bottom | 12.9 mm | 25.2 mm |
| J4        | Bottom | 26.7 mm | 25.2 mm |
| J6        | Inner  | 11.0 mm | 12.6 mm |
| J3        | Inner  | 17.6 mm | 21.5 mm |

> ⚠️ The board has **no mounting holes**. If it has to be mechanically secured, add M2/M2.5 holes. That would make the board larger, roughly +6 mm in each direction for corner holes.

### Tall Components

| Component             |                    Approx. Height |
| --------------------- | --------------------------------: |
| J2, J4, J5, J7, J8, J3, J6 (2.54 mm headers) | ≈ 8.5 mm (standard 6 mm mating pin) |
| J1 screw terminal     | Refer to Phoenix PT 1,5/2-5.0-H datasheet |
| C2 electrolytic       |                            5.4 mm |
| L1 inductor           |                            4.5 mm |

---

## 8. Connector Pin Map

| Connector | Pin | Net         | Connected To                                  |
| --------- | --- | ----------- | --------------------------------------------- |
| J1        | 1   | GND         | Ground plane                                  |
| J1        | 2   | Vbat        | U1 VIN, Cadj2, Cadj3, Radj2, F1.1 ⚠️ unfused  |
| J2 / J5   | 1   | J2-Pin_1    | F1 → Vbat; R1, R5 (gates); R2, R3 (to GND) ⚠️ |
| J2 / J5   | 2   | VCC         | J7.1, J8.1 (coil supply, In2 plane)           |
| J2 / J5   | 3   | GND         | Ground plane                                  |
| J2 / J5   | 4   | GND         | Ground plane                                  |
| J4        | 1   | CANH        | U2 pin 7, R4                                  |
| J4        | 2   | CANL        | U2 pin 6, R4                                  |
| J7        | 1   | VCC         | Coil 1 supply                                 |
| J7        | 2   | J7-Pin_2    | Q3 drain                                      |
| J8        | 1   | VCC         | Coil 2 supply                                 |
| J8        | 2   | J8-Pin_2    | Q2 drain                                      |
| J3        | 1   | —           | Unconnected ⚠️                                |
| J6        | 1   | —           | Unconnected ⚠️                                |

⚠️ = requires review before fabrication.

---

## 9. Footprint Library

Every footprint comes from the **official KiCad footprint library** (`kicad-footprints`, KiCad 10). DRC reports no library mismatches, so the board copies are identical to the library versions.

| Ref(s)                     | Footprint | Library Link |
| -------------------------- | --------- | ------------ |
| U1                         | `Package_SO:MSOP-8-1EP_3x3mm_P0.65mm_EP1.73x1.85mm_ThermalVias` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Package_SO.pretty/MSOP-8-1EP_3x3mm_P0.65mm_EP1.73x1.85mm_ThermalVias.kicad_mod) |
| U2                         | `Package_SO:SOIC-8_3.9x4.9mm_P1.27mm` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Package_SO.pretty/SOIC-8_3.9x4.9mm_P1.27mm.kicad_mod) |
| U4                         | `Package_SON:VSON-8_3.3x3.3mm_P0.65mm_NexFET` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Package_SON.pretty/VSON-8_3.3x3.3mm_P0.65mm_NexFET.kicad_mod) |
| VR1                        | `Package_TO_SOT_SMD:SOT-223` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Package_TO_SOT_SMD.pretty/SOT-223.kicad_mod) |
| Q2, Q3                     | `Package_TO_SOT_SMD:SOT-23` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Package_TO_SOT_SMD.pretty/SOT-23.kicad_mod) |
| D1                         | `Diode_SMD:D_SOD-523` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Diode_SMD.pretty/D_SOD-523.kicad_mod) |
| L1                         | `Inductor_SMD:L_7.3x7.3_H4.5` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Inductor_SMD.pretty/L_7.3x7.3_H4.5.kicad_mod) |
| C2                         | `Capacitor_SMD:CP_Elec_6.3x5.4` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Capacitor_SMD.pretty/CP_Elec_6.3x5.4.kicad_mod) |
| C5                         | `Capacitor_SMD:C_1206_3216Metric` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Capacitor_SMD.pretty/C_1206_3216Metric.kicad_mod) |
| C1, C3, C4                 | `Capacitor_SMD:C_0805_2012Metric` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Capacitor_SMD.pretty/C_0805_2012Metric.kicad_mod) |
| Cadj1, Cadj2, Cadj3        | `Capacitor_SMD:C_0603_1608Metric` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Capacitor_SMD.pretty/C_0603_1608Metric.kicad_mod) |
| Rsen1                      | `Resistor_SMD:R_2512_6332Metric` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Resistor_SMD.pretty/R_2512_6332Metric.kicad_mod) |
| R1, R2, R3, R5, R6, R7     | `Resistor_SMD:R_0805_2012Metric_Pad1.20x1.40mm_HandSolder` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Resistor_SMD.pretty/R_0805_2012Metric_Pad1.20x1.40mm_HandSolder.kicad_mod) |
| R4, Radj1, Radj2           | `Resistor_SMD:R_0603_1608Metric` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Resistor_SMD.pretty/R_0603_1608Metric.kicad_mod) |
| F1                         | `Fuse:Fuse_1206_3216Metric` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Fuse.pretty/Fuse_1206_3216Metric.kicad_mod) |
| J1                         | `TerminalBlock_Phoenix:TerminalBlock_Phoenix_PT-1,5-2-5.0-H_1x02_P5.00mm_Horizontal` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/TerminalBlock_Phoenix.pretty/TerminalBlock_Phoenix_PT-1,5-2-5.0-H_1x02_P5.00mm_Horizontal.kicad_mod) |
| J2, J5                     | `Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Connector_PinHeader_2.54mm.pretty/PinHeader_1x04_P2.54mm_Vertical.kicad_mod) |
| J4, J7, J8                 | `Connector_PinHeader_2.54mm:PinHeader_1x02_P2.54mm_Vertical` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Connector_PinHeader_2.54mm.pretty/PinHeader_1x02_P2.54mm_Vertical.kicad_mod) |
| J3, J6                     | `Connector_PinHeader_2.54mm:PinHeader_1x01_P2.54mm_Vertical` | [kicad_mod](https://gitlab.com/kicad/libraries/kicad-footprints/-/blob/master/Connector_PinHeader_2.54mm.pretty/PinHeader_1x01_P2.54mm_Vertical.kicad_mod) |

Library home: [gitlab.com/kicad/libraries/kicad-footprints](https://gitlab.com/kicad/libraries/kicad-footprints)

> **Board-level modification:** U4 pads 1–4 have a **0.12 mm local clearance** set on the board. The footprint geometry itself is unchanged.

---

# 10. Known Issues / To-Do Before Ordering

> ⚠️ **Do not send the PCB for fabrication until the following issues have been reviewed and resolved.**
> All of them are **schematic** issues. The netlist was deliberately left unchanged during layout so the board stays in sync with the schematic. Fix the schematic, update the PCB from it, and re-route the affected nets.

## 10.1 Critical Functional Issues

### 1. Buck Converter Input Path Is Open

The sense+ node is not connected to **Vbat**. This node is Rsen1 pin 2 / Radj1 pin 2 / Cadj1 pin 1 (`Net-(Cadj1-Pad1)`), so U4's source has no supply and the converter cannot run.

**Action:** Connect the Rsen1 / Radj1 / Cadj1 node to Vbat (the LM25085 VIN side), as in the datasheet's typical application.

---

### 2. LM3940 Input Grounded

VR1 pins 1 and 2 are both on GND, and the tab (pin 4) is unconnected. On the SOT-223 LM3940, **pin 1 is IN**. The +3.3 V rail can never be powered.

**Action:** Connect VR1 pin 1 to the intended input rail (5 V or the buck output) and the tab to GND.

---

### 3. LM25085 FB Unconnected

U1 pin 3 (FB) is unconnected, so the R7/R6/C1 feedback divider does not reach the controller.

**Action:** Connect the R7/R6 junction (`Net-(C1-Pad2)`) to U1 FB.

---

### 4. LM25085 Exposed Pad Unconnected

U1's exposed pad (pin 9) and its thermal vias are not on GND.

**Action:** Tie EP to GND per the datasheet.

---

### 5. CAN Transceiver Supply and Logic

U2 VCC (pin 3) is fed from `Net-(U1-VCC)`. That is the LM25085's VCC gate-drive bias pin, **not a regulated 5 V rail**. The TJA1051 needs **4.5–5.5 V**.

U2's other pins are also open:

* TXD (pin 1) is unconnected
* RXD (pin 4) is unconnected
* S (pin 8) is unconnected

**Action:** Provide a proper 5 V supply with local decoupling. Bring TXD/RXD to a controller. Tie S to GND for normal mode, or drive it.

---

### 6. Coil Driver Gate Network

R2 and R3 are connected **from J2-Pin1 to GND**, not from the gates to GND. As drawn:

* Q2/Q3 gates are pulled to the battery input through R1/R5, with no pull-down, so both coils energise whenever J2/J5 pin 1 is live.
* The AO3400A gate rating (±12 V) can be exceeded on a higher-voltage battery.

**Action:** Confirm the intended control scheme. Most likely R2/R3 should be gate pull-downs, and the gates should be driven by a logic-level control signal. Add gate clamping if the battery voltage exceeds 12 V.

---

### 7. Unfused J1 Input

J1 pin 2 feeds Vbat directly, bypassing F1.

**Action:** Route all battery inputs through the fuse.

---

## 10.2 Design Review Items

### C4 Shorted to Ground

C4 has both pads on GND, so it does nothing.

**Action:** Fix its connection or mark it **DNP**.

### Freewheel Diode Rating

D1 is in a **SOD-523** package, which is only rated for low current.

**Action:** Check D1's current rating against the buck converter's load current. A SOD-123F/SMA Schottky may be needed.

### Unconnected Pins J3 / J6

J3 and J6 have no net.

**Action:** Assign their intended function or remove them.

### Unspecified Values

The following values need confirming:

* R7: value shown as `R_Small_US`
* F1: rating not specified
* D1: part number not specified

### Mounting

No mounting holes are present.

**Action:** Add them if the board must be mechanically fixed.

---

# 11. PCB Housekeeping

Before generating manufacturing files:

* [ ] Define the physical stackup explicitly in *Board Setup* (currently KiCad defaults).
* [ ] Add mounting holes if required.
* [ ] Re-run placement and routing for the nets that change after the schematic fixes.
* [ ] Verify silkscreen labels. Refdes for U1, R3, Radj1/2 and Cadj1/2 are on F.Fab only (no room on silk).
* [ ] Check component courtyard clearance (currently 0 violations).
* [ ] Run **Electrical Rules Check (ERC)**.
* [ ] Run **Design Rules Check (DRC)** (currently 0 errors / 0 warnings).
* [ ] Refill all zones before plotting.
* [ ] Generate and inspect Gerbers and drill files before fabrication.

---

# 12. Pre-Fabrication Checklist

### Electrical

* [ ] Buck input path (sense+ → Vbat) connected
* [ ] Buck FB connected and output voltage verified
* [ ] LM25085 exposed pad tied to GND
* [ ] LM3940 input / tab corrected
* [ ] CAN transceiver 5 V supply provided
* [ ] CAN TXD/RXD/S connected
* [ ] CAN termination verified
* [ ] Coil gate drive and pull-downs corrected
* [ ] Gate-voltage rating checked against battery voltage
* [ ] All inputs fused
* [ ] D1 current rating verified
* [ ] Protection requirements reviewed

### PCB

* [ ] Stackup confirmed with manufacturer
* [ ] 1 oz copper specified on all layers (2 oz outer if coil current > ~3 A)
* [ ] 0.20 mm minimum drill supported by fab
* [ ] 0.12 mm minimum spacing (U4 pads) supported by fab
* [ ] Surface finish specified (ENIG)
* [ ] Board dimensions verified (33.1 × 27.0 mm)
* [ ] Mounting holes added or confirmed not required
* [ ] Connector footprints verified
* [ ] Component clearances checked
* [ ] DRC passed
* [ ] ERC passed

### Documentation

* [ ] BOM updated
* [ ] Part numbers verified
* [ ] Connector specifications confirmed
* [ ] Revision number updated
* [ ] Schematic revision updated
* [ ] PCB revision updated
* [ ] README updated
* [ ] Gerbers generated
* [ ] Gerbers reviewed

---
