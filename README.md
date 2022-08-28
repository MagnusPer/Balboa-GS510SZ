# Balboa-GS510SZ

## PIN

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

## Measurenments 

YELLOW = PIN 6 - CLK <br />
BLUE  = PIN 5 - Display data <br /> 
RED    = PIN 8 - Button data <br />

<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/measurements/Cycles.JPG" width="400">

<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/measurements/PIN6-PIN5-PIN8.JPG" width="400">
 
<img src="https://github.com/MagnusPer/Balboa-GS510SZ/blob/main/measurements/Complete%20Sequence.JPG" width="1000">

Display shows *37.5C* - How does that translates into the bitstream??  

### Conclusion
- The clock is pulsing 7 times for each byte  <br />
- Totaly of 6 bytes in each cycle <br />
- Each cycle is repeted every 20 ms <br />
- One cycle duration 1.88 ms
- Bit duration of the clock is 10 microseconds <br />

### references
- https://github.com/NickB1/OpenSpa/blob/master/documents/Balboa/Balboa_Display_Measurements.pdf
