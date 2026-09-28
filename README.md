Layers and stackup
TLB is a 4-layer, 50 × 50 mm PCB.
#	Layer	Material	Thickness	Use
—	Top silkscreen	—	—	Reference designators / labels
—	Top solder mask	—	—	—
1	F.Cu	Copper	0.035 mm	Components + signal routing
—	Prepreg	FR4	0.10 mm	Dielectric
2	In1.Cu	Copper	0.0175 mm	Split +3V3 / +5V power plane
—	Core	FR4	1.24 mm	Dielectric
3	In2.Cu	Copper	0.0175 mm	GND plane
—	Prepreg	FR4	0.10 mm	Dielectric
4	B.Cu	Copper	0.035 mm	Debugging / signal routing
—	Bottom solder mask	—	—	—
	Total		1.565 mm	


Layer strategy
- F.Cu: all components + primary signal routing
- In1.Cu: split power plane containing +3V3 and +5V
- In2.Cu: solid GND plane
- B.Cu: remaining/debug signal routing
Unlike the other board, your TLB deliberately has a power layer, because the design contains both 3.3 V and 5 V circuitry.
Your antenna keepout around the ESP32 must remain clear of routing/copper.
2. Routing rules
Your current PCB rules are:
Rule	TLB value
Minimum clearance	0.20 mm
Minimum track width	0.20 mm
Default signal track	0.25 mm
Power track	0.50 mm
Minimum via diameter	0.50 mm
Default via	0.60 / 0.30 mm
Power via	0.80 / 0.40 mm
Minimum drill	0.30 mm
Copper-edge clearance	0.50 mm
Hole-hole clearance	0.25 mm
Annular width	0.10 mm


Your net classes are:
Default
- Clearance: 0.20 mm
- Track: 0.25 mm
- Via: 0.60 / 0.30 mm
Power
- Clearance: 0.20 mm
- Track: 0.50 mm
- Via: 0.80 / 0.40 mm
- Assigned to +3V3 and +5V
3. Copper weights
Layer	Thickness	Weight
F.Cu	35 µm	1 oz
In1.Cu	17.5 µm	0.5 oz
In2.Cu	17.5 µm	0.5 oz
B.Cu	35 µm	1 oz


This matches the stackup specification you were given: 1 oz outer copper and 0.5 oz inner copper.
4. Connectors
Your TLB has these main external interfaces:
Ref	Connector	Purpose
J1	Power_In, 1×2	+3V3 power input + GND
J2	Light_APRS, 1×4	UART telemetry interface
J3	CAN connector, 1×2	CANH + CANL
J5	Micro-USB	USB interface / VBUS / shield
—	ESP32	Main controller / telemetry and logging processing


J1 — Power input
- Pin 1 → +3V3
- Pin 2 → GND
C2, the 33 µF bulk capacitor, is placed close to J1.
J2 — Light APRS
Pin	Signal
1	+3V3
2	GND
3	TX0
4	RX0


Used as the UART interface to the Light APRS system.
J3 — CAN
Pin	Signal
1	CANH
2	CANL


R4 = 120 Ω CAN termination resistor.
J5 — Micro-USB
- Pin 1 → VBUS
- Pin 5 → GND
- Pin 6 → Shield
- R6 = 0 Ω from shield to GND
- C13 associated with VBUS
- C12 associated with GND
5. Main components
MCU
U2 — ESP32-C3
Main microcontroller for the TLB.
Relevant interfaces:
ESP32 signal	Function
IO21 / TXD	I²C SCL
IO20 / RXD	I²C SDA
IO4	CAN-related
IO5	CAN-related
UART0 TX	TX0 → Light APRS
UART0 RX	RX0 ← Light APRS


The ESP32 operates from +3V3.
There is also an antenna keepout region around the ESP32 antenna.
EEPROM
U3 — 24LC256
256-kbit I²C EEPROM used for non-volatile data storage.
Connections:
- VCC → +3V3
- GND → GND
- SDA → ESP32 SDA
- SCL → ESP32 SCL
Pull-ups:
- R1 = 4.7 kΩ
- R3 = 4.7 kΩ
Decoupling:
- C5 = 0.1 µF
- C6 = 0.1 µF
CAN transceiver
U4 — TJA1051TK/3
Provides the physical-layer CAN interface.
Important power arrangement:
Pin	Function	TLB connection
1	TXD	CANTX
2	GND	GND
3	VCC	+5V
4	RXD	CANRX
5	VIO	+3V3
6	CANL	CANL
7	CANH	CANH
8	S	R2


This is important because the TJA1051TK/3 uses 5 V for VCC, while its logic interface can use 3.3 V through VIO.
Supporting components:
- C7 = 0.1 µF
- C8 = 0.1 µF
- C9 = 0.1 µF
- R2 = 10 kΩ
- R4 = 120 Ω termination
6. TLB component summary
Ref	Component	Function
U2	ESP32-C3	Main MCU
U3	24LC256	EEPROM / non-volatile storage
U4	TJA1051TK/3	CAN transceiver
J1	1×2 Power_In	+3V3 + GND input
J2	1×4 Light_APRS	UART telemetry
J3	1×2 CAN	CANH / CANL
J5	Micro-USB	USB interface
C2	33 µF	Bulk supply capacitor
C3/C4	0.1 µF	ESP32 decoupling
C5/C6	0.1 µF	EEPROM decoupling
C7/C8/C9	0.1 µF	CAN transceiver decoupling
C12/C13	0.1 µF	USB-related capacitors
R1/R3	4.7 kΩ	I²C pull-ups
R2	10 kΩ	CAN transceiver S
R4	120 Ω	CAN termination
R5	10 kΩ	ESP32 EN pull-up
R6	0 Ω	USB shield → GND


7. Interfaces / signal map
This is probably one of the most useful sections to have in your README.
I²C
ESP32 → 24LC256
- SCL → U3 pin 6
- SDA → U3 pin 5
- R1/R3 = 4.7 kΩ pull-ups to +3V3
CAN
ESP32 → TJA1051 → CAN connector
ESP32 CANTX
     ↓
TJA1051 TXD
     ↓
CAN transceiver
     ↓
CANH / CANL
     ↓
J3

and
J3 CANH / CANL
       ↓
TJA1051
       ↓
CANRX / CANTX
       ↓
ESP32

UART / Light APRS
ESP32 UART0 TX → J2 TX0
ESP32 UART0 RX ← J2 RX0

USB
J5 USB
 ├── VBUS
 ├── GND
 └── Shield

8. Power architecture
Your current PCB has:
+3V3
Used by:
- ESP32-C3
- 24LC256
- TJA1051 VIO
- I²C pull-ups
- J2
- various decoupling capacitors
+5V
Used by:
- TJA1051 VCC
GND
- Solid plane on In2.Cu
- GND vias connect component/connector GNDs into the plane.
⚠️ Important unresolved point
Your current PCB has a +5V isolated-copper warning, and we couldn't identify an obvious +5V source from the current connector arrangement:
- J1 supplies +3V3
- J5 VBUS is its own Net-(J5-VBUS) net
- U4 requires +5V at VCC
So this should remain explicitly documented as something to confirm with your team lead, rather than us inventing a power architecture.
9. Dimensions
Specification	TLB
Board width	50.0 mm
Board height	50.0 mm
Thickness	1.565 mm nominal stackup
Layers	4
Component side	Top
Mounting holes	2 × Ø5.0 mm NPTH


Your board outline is:
X = 100 → 150 mm
Y = 60 → 110 mm
giving exactly:
50 × 50 mm
10. Mounting holes
You added:
- 2 × 5 mm NPTH mounting holes
These were added as a team requirement.
The exact hole coordinates aren't in the information I have here, so I would not put coordinates into the README until we read them directly from your PCB file.
11. Protection / design features
Your TLB currently has:
Decoupling
Local 0.1 µF capacitors are placed around:
- ESP32
- EEPROM
- CAN transceiver
- USB section
CAN termination
R4 = 120 Ω between CANH and CANL.
Ground plane
In2.Cu = GND plane, with GND vias from components/connectors.
ESP32 antenna keepout
A dedicated keepout is maintained around the ESP32 antenna region.
USB shield grounding
R6 provides the shield-to-GND connection.
