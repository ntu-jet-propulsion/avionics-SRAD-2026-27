TLB_Questions

Question 01:
+5V isolated copper: KiCad is still showing an “isolated copper fill” warning for the +5V zone on In1.Cu. Is this okay to leave, or should I modify/remove the isolated +5V region?

Question 02:
SDA connection: KiCad shows a “copper connection too narrow” warning on the SDA connection, with an effective width of ~0.041 mm even though the track itself is 0.25 mm. Is this something I need to fix, or can it be ignored?

Question 03:
GND vias near J1/J2: KiCad shows drilled-hole clearance warnings because some GND vias are extremely close to the J1/J2 connector PTH holes. Should I move these vias, or is the current placement acceptable?

Question 04:
ESP32 footprint: KiCad reports that the ESP32-C3-WROOM-02 footprint doesn't match the current library footprint. We used the existing/different footprint for the board — is this intentional and okay to keep?

Question 05:
Silkscreen warnings: There are several silkscreen clearance/clipping warnings. Do I need to clean these up before fabrication, or are they acceptable?

Question 06:
Mounting holes: I added two 5 mm NPTH mounting holes. Is their current placement acceptable?
