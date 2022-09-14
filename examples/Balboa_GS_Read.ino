
#include "Balboa_GS_Interface.h" 


char serialMonitorInput                       = 0;              // for incoming serial monitor data

unsigned long printMillis                 = 3000;               // Time in milliseconds the status are printed 
unsigned long printPrevMillis             = 0;                  // Store previous millis

#define ClockPin D1  // esp8266: D1, D2, D8 (GPIO 5, 4, 15)
#define ReadPin  D2  // esp8266: D1, D2, D8 (GPIO 5, 4, 15)
#define WritePin D8  // esp8266: D1, D2, D8 (GPIO 5, 4, 15)

// Initialize components
BalboaInterface Balboa(ClockPin, ReadPin, WritePin);




void setup() {
  
  Serial.begin(115200);
  Balboa.begin();

}

void loop() {
	
	Balboa.loop();

    if (Serial.available() > 0 ) {
          // read the incoming message fron serial monitor:
          serialMonitorInput = Serial.read();
              
              switch (serialMonitorInput) {
                  case '1':
                        Balboa.writeButtonData = true; 
                        Balboa.writeTempUp     = true;
                        Serial.println("Write - Temp Up");
                        break; 
                  case '2':
                        Balboa.writeButtonData = true;
                        Balboa.writeTempDown   = true;  
                        Serial.println("Write - Temp Down");
                        break; 
                  case '3':
                        Balboa.writeButtonData = true;
                        Balboa.writeMode       = true; 
                        Serial.println("Write - Mode");
                        break;
                  case '4':
                        Balboa,writeButtonData = true;
                        Balboa.writeLight      = true; 
                        Serial.println("Write - Light");
                        break;
                  case '5':
                        Balboa.writeButtonData =  true;
                        Balboa.writePump1      = true; 
                        Serial.println("Write - Pump 1");
                        break;
                  case '6':
                        Balboa.writeButtonData = true;
                        Balboa.writePump2      = true; 
                        Serial.println("Write - Pump 2");
                        break;
                  case '7':
                        Balboa.writeButtonData = true;
                        Balboa.writePump3      = true; 
                        Serial.println("Write - Pump 3");
                        break;  
                    default:
                      break;
           }
     }

 
      if(millis() - printPrevMillis  > printMillis && displayDataBufferReady) {
      
         /*   printPrevMillis = millis();
            
            for (int x = 0; x <= displayDataBits; x++) {
                              
                  Serial.print(displayDataBuffer[x]);
                  
                  if (x == 6 || x == 13 || x == 20 || x == 27 || x == 34 ) {
                     Serial.print(" ");
                  }
            }*/
            
            Serial.println("");
            Serial.print("Water temperature (int): ");
            Serial.println(Balboa.waterTemperature); 
            Serial.print("Set temperature ");
            Serial.println(Balboa.setTemperature); 
            Serial.print("Water temperature (string): ");
            Serial.println(Balboa.LCD_display);
            Serial.print("Display button: ");
            Serial.println(Balboa.displayButton);
            Serial.print("Standard mode: ");
            Serial.println(Balboa.displayStandardMode);
            Serial.print("Heater: ");
            Serial.println(Balboa.displayHeater);
            Serial.print("Pump 1: ");
            Serial.println(Balboa.displayPump1);
            Serial.print("Pump 2: ");
            Serial.println(Balboa.displayPump2);
            Serial.print("Light: ");
            Serial.println(Balboa.displayLight);
           
      } 

   
       
}

