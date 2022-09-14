# Balboa-GS510SZ with panel VL700S

<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/extras/docs/balboa_gs501sz.jpg" width="400"> <img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/extras/docs/VL700S.jpg" width="400">




## RJ45 pins
<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/extras/docs/RJ45.jpg" width="200">

| PIN           | Description             | 
| ------------- | ------------------------|
| PIN 1         | Functionality unclear!  |
| PIN 2         | Functionality unclear!  |
| PIN 3         | Button data             |  
| PIN 4         | GND                     |
| PIN 5         | Display data            | 
| PIN 6         | Clock                   |
| PIN 7         | 5 VDC                   |  
| PIN 8         | Floating output         |

## Measurements 

### PIN 1 and 2 
PWM output from control system, same on both pins. Unclear functionality for these PINs still! Maybe some internal light supply? <br />  
50Hz, pk-pk=18v, mean 5V

<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/extras/measurements/PIN_1%262.JPG" width="300">

### PIN 6 (Yellow - clock), PIN 5 (Blue - display data), PIN 3 (Pink - button data) 

<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/extras/measurements/Multiple_sequences.jpg" width="350">   

<p align="center">
<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/extras/measurements/Sequence.jpg" width="1000">
</p>

#### Conclusion
- The clock is pulsing 7 times for each chunck of display data, totaly 39 bits och 3 times for button data. A complete cycle 42 pulses. <br />
- Totaly of 6 display data chunks in each cycle <br />
- Each cycle is repeted every 20 ms <br />
- One total cycle duration 1.88 ms
- Bit duration for display data is 10 microseconds for display data <br />
- Bit duration for button data is 60 microseconds <br />

#### Display Data
Display shows **37.5C** - How does that translates into the bitstream? The first 4 chunks is coded in BCD to 7 segment LCD (see reference) but only with 7 bits sinse the LSB always is (0). <br />
3 = 1111001(0) = 0x79  (Chunk 1) <br />
7 = 1110000(0) = 0x70  (Chunk 2) <br />
5 = 1011011(0) = 0x5B  (Chunk 3) <br />
C = 1001110(0) = 0x4E  (Chunk 4) <br />

| Chunk 1 - bit(0-6)| Chunk 2 - bit(7-13) | Chunk 3 - bit(14-20) | Chunk 4 - bit(21-27) | Chunk 5 - bit(28-34) | Chunk 6 - bit(35-38) | 
| ----------------- | ------------------- | ------------------   | -------------------- | -------------------- | ------------------   |                    
| LCD segment 1     |  LCD segment 2      |   LCD Segment 3      |   LCD segment 4      | 28 - Button down/up  | 35 - Heater          |                    
|                   |                     |                      |                      | 29 - ?               | 36 - Pump 1          |        
|                   |                     |                      |                      | 30 - ?               | 37 - Pump 2          |
|                   |                     |                      |                      | 31 - Standard mode   | 38 - Light           |
|                   |                     |                      |                      | 32 - ?               |                      |
|                   |                     |                      |                      | 33 - ?               |                      |
|                   |                     |                      |                      | 34 - ?               |                      |
   
#### Button data 

|  Button   |  Decoding bit(0-2) | 
| --------- | ------------------ |
| Mode      |   0 0 1            |
| Temp up   |   1 0 0            |
| Temp down |   1 0 1            |  
| Light     |   0 1 1            |
| Pump 1    |   1 1 0            | 
| Pump 2    |   0 1 0            |
| Pump 3    |   1 1 1            |  

## Wiring

Since the display and controler unit hardware design is unknown an OR-gate is used on button data line to protect simultaneous HIGH/LOW levels from display and Arduino.

ToDo: voltage adjust the Arduino input to max 3.3V

<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/extras/docs/Balboa_GS_Interface.jpg" width="1000">

## Software

**version 1.0**
Basic functionality to read and set status using a **Balboa_GS** developed library. Two examples are provided, first with simple read and set functionality and the second using MQTT for remote access.     

## References of great help!
- https://github.com/NickB1/OpenSpa/blob/master/documents/Balboa/Balboa_Display_Measurements.pdf
- https://www.olivierhill.ca/archives/72-The-Internet-of-Spas.html
- https://create.arduino.cc/projecthub/meljr/7-segment-led-displays-102-using-a-shift-register-6b6976
