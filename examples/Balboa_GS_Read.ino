 /*
 *    
 *    Main board: Wemos D1 mini - esp8266
 *  
 *    SPA display controller for Balboa system GS510SZ
 *    
 */


#include "Balboa_GS_Interface.h" 


char serialMonitorInput       = 0;                  // Serial monitor data
unsigned long printMillis     = 3000;               // Time in milliseconds the status are printed 
unsigned long printPrevMillis = 0;                  // Store previous millis


#define setClockPin D1  
#define setReadPin  D2 
#define setWritePin D8  

// Initialize components
BalboaInterface Balboa(setClockPin, setReadPin, setWritePin);


void setup() {
  
  Serial.begin(115200);
  Balboa.begin();

}

void loop() {
	
	Balboa.loop();

    if (Serial.available() > 0 ) {
          // read the incoming message fron serial monitor:
          serialMonitorInput = Serial.read();

         float temp_up = 38.5;
         float temp_down = 33.5;
    
              switch (serialMonitorInput) {
                  case '1':
                        Balboa.writeDisplayData = true; 
                        Balboa.writeTempUp      = true;
                        Serial.println("Write - Temp Up");
                        break; 
                  case '2':
                        Balboa.writeDisplayData = true;
                        Balboa.writeTempDown    = true;  
                        Serial.println("Write - Temp Down");
                        break; 
                  case '3':
                        Balboa.writeDisplayData = true;
                        Balboa.writeMode        = true; 
                        Serial.println("Write - Mode");
                        break;
                  case '4':
                        Balboa.writeDisplayData = true;
                        Balboa.writeLight       = true; 
                        Serial.println("Write - Light");
                        break;
                  case '5':
                        Balboa.writeDisplayData =  true;
                        Balboa.writePump1       = true; 
                        Serial.println("Write - Pump 1");
                        break;
                  case '6':
                        Balboa.writeDisplayData = true;
                        Balboa.writePump2       = true; 
                        Serial.println("Write - Pump 2");
                        break;
                  case '7':
                        Balboa.writeDisplayData = true;
                        Balboa.writePump3       = true; 
                        Serial.println("Write - Pump 3");
                        break;  
                   case '8':
                        Balboa.updateTemperature(temp_up);
                        Serial.print("update temp: ");
                        Serial.println(temp_up);
                        break;
                  case '9':
                        Balboa.updateTemperature(temp_down);
                        Serial.print("update temp: ");
                        Serial.println(temp_down);
                        break;
                
                  default:
                      break;
           }
     }

 
    if(millis() - printPrevMillis  > printMillis) {
    
          printPrevMillis = millis();
          
          Serial.println("");
          Serial.print("Water temperature): ");
          Serial.println(Balboa.waterTemperature); 
          Serial.print("Set temperature: ");
          Serial.println(Balboa.setTemperature); 
          Serial.print("LCD Display: ");
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
          Serial.println("");
         
    } 
     
}
