# Balboa-GS510SZ

## RJ45 pins
<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/docs/RJ45.png" width="200">

| PIN           | Type            | Description   |
| ------------- | --------------  |-------------- |
| PIN 1         | Power           |  5 VDC        |
| PIN 2         | ?               |               |
| PIN 3         | Digital Input   |  Pump 1       |
| PIN 4         | GND             |  Ground       |
| PIN 5         | Serial Tx       |  Display Data |
| PIN 6         | CLK             |  Clock        |
| PIN 7         | Digital Input   |  Pump 2       |
| PIN 8         | Serial Tx       |  Buttons data |

## Measurements 

YELLOW = PIN 6 - CLK <br />
BLUE  = PIN 5 - Display data <br /> 
RED    = PIN 8 - Button data <br />

<p align="center">
<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/measurements/Cycles.JPG" width="350">   <img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/measurements/PIN6-PIN5-PIN8.JPG" width="400">
</p>
<p align="center">
<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/measurements/Complete%20Sequence.JPG" width="1000">
</p>

### Conclusion
- The clock is pulsing 7 times for each chunck (not byte!) - really strange behaviour, why not 8 !! <br />
- Totaly of 6 chunks in each cycle <br />
- Each cycle is repeted every 20 ms <br />
- One cycle duration 1.88 ms
- Bit duration of the clock is 10 microseconds <br />

Display shows **37.5C** - How does that translates into the bitstream? It seems the first 4 chunks is coded in BCD to 7 segment LCD (see reference) but only with 7 bits sinse the LSB always is (0). Again why try to optimize that hard when there is not much data anyway! <br />
3 = 1111001(0) = 0x79  (Chunk 1) <br />
7 = 1110000(0) = 0x70  (Chunk 2) <br />
5 = 1011011(0) = 0x5B  (Chunk 3) <br />
C = 1001110(0) = 0x4E  (chunk 4) <br />

### References
- https://github.com/NickB1/OpenSpa/blob/master/documents/Balboa/Balboa_Display_Measurements.pdf
- https://www.olivierhill.ca/archives/72-The-Internet-of-Spas.html
- https://create.arduino.cc/projecthub/meljr/7-segment-led-displays-102-using-a-shift-register-6b6976


### First implementation running to decode data is progressing...

<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/measurements/Capture.JPG" width="400">

### Remember
Compile ESP8266 with 160Mhz otherwise samples may be lost - still digging in why! 
