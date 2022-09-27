
#include "Balboa_GS_Interface.h" 


byte BalboaInterface::clockPin;
byte BalboaInterface::displayPin;
byte BalboaInterface::buttonPin;
bool BalboaInterface::displayDataBufferOverflow;
bool BalboaInterface::writeDisplayData;       
bool BalboaInterface::writeMode;
bool BalboaInterface::writeTempUp;
bool BalboaInterface::writeTempDown;
bool BalboaInterface::writeLight;
bool BalboaInterface::writePump1;
bool BalboaInterface::writePump2;
bool BalboaInterface::writePump3;
unsigned long BalboaInterface::clockInterruptTime;
int  BalboaInterface::clockBitCounter;  
byte BalboaInterface::displayDataBuffer[displayDataBufferSize];
byte BalboaInterface::dataIndex; 
bool BalboaInterface::displayDataBufferReady;  


BalboaInterface::BalboaInterface(byte setClockPin, byte setReadPin, byte setWritePin) {
  clockPin = setClockPin;
  displayPin = setReadPin;
  buttonPin = setWritePin;
     
}

void BalboaInterface::begin() { 
  pinMode(clockPin, INPUT);
  pinMode(displayPin, INPUT);
  pinMode(buttonPin, OUTPUT);
  digitalWrite(buttonPin,LOW);   
   
  attachInterrupt(clockPin, clockPinInterrupt, CHANGE);
  
 }

void BalboaInterface::stop() {
  
  detachInterrupt(digitalPinToInterrupt(clockPin));    

}

bool BalboaInterface::loop() {

	if (displayDataBufferReady) { 
		
		// Decode data onces available 
		decodeDisplayData(); 
		
		// Get setTemperature if not known 
		if (setTemperature == 0) {
			writeDisplayData = true;
			writeTempUp = true;
		}
				
		
		// Update temperature 
		if (updateTempButtonPresses > 0) {
			
			if(millis() - buttonPressTimerPrevMillis  > buttonPressTimerMillis) {
				
				buttonPressTimerPrevMillis = millis();

				if (updateTempDirection == 1) {
					writeDisplayData = true;
					writeTempDown = true;
				}
				else if (updateTempDirection == 2) {
					writeDisplayData = true;
					writeTempUp = true;
				}
			
				updateTempButtonPresses--;
			}
		}
	}
	   	
  return true;	
}


void BalboaInterface::updateTemperature(float Temperature){
	
	float updateTempDifference = Temperature - setTemperature;
	
	if (updateTempDifference < 0 ) 		 { updateTempDirection = 1; }			// Temp down
	else if (updateTempDifference > 0 )  { updateTempDirection = 2; }			// Temp up
	else if (updateTempDifference == 0 ) { updateTempDirection = 0; }			// no change
	
	updateTempButtonPresses = 1 + (abs(updateTempDifference) * 2);				// calculate how many times the "button" should be pressed 
																				// every button press = 0.5 and the first is to enter the menu																			
}
	
	
void BalboaInterface::decodeDisplayData() {

      LCD_segment_1 = 0;
      LCD_segment_2 = 0;
      LCD_segment_3 = 0;
      LCD_segment_4 = 0;
      
      LCD_display = "";
            
      for (int x = 0; x <= displayDataBits; x++) {
      
                  if ( x <= 6 ) {
                        LCD_segment_1 <<= 1;
                        if (displayDataBuffer[x] == 1){
                            LCD_segment_1 |= 1;
                        }
                        else LCD_segment_1 |= 0;
                  }
                  else if (x > 6 && x <= 13) {
                        LCD_segment_2 <<= 1;
                        if ( displayDataBuffer[x] == 1){
                            LCD_segment_2 |= 1;
                        }
                        else LCD_segment_2 |= 0;
                  }   
                  else if (x > 13 && x <= 20) {
                        LCD_segment_3 <<= 1;
                        if ( displayDataBuffer[x] == 1){
                            LCD_segment_3 |= 1;
                        }
                        else LCD_segment_3 |= 0;
                  }   
                  else if (x > 20 && x <= 27) {
                        LCD_segment_4 <<= 1;
                        if ( displayDataBuffer[x] == 1){
                            LCD_segment_4 |= 1;
                        }
                        else LCD_segment_4 |= 0;
                  }       
                  else if (x == 28) {
                        if ( displayDataBuffer[x] == 1){
                            displayButton = true;
                        }
                        else displayButton = false;
                 }
                 else if (x == 29) {
                        if ( displayDataBuffer[x] == 1){
                            displayBit29 = true;
                        }
                        else displayBit29 = false;
                 } 
                  else if (x == 30) {
                        if ( displayDataBuffer[x] == 1){
                            displayBit30 = true;
                        }
                        else displayBit30 = false;
                  }  
                  else if (x == 31) {
                        if ( displayDataBuffer[x] == 1){
                            displayStandardMode = true;
                        }
                        else displayStandardMode = false;
                  } 
                  else if (x == 31) {
                        if ( displayDataBuffer[x] == 1){
                            displayStandardMode = true;
                        }
                        else displayStandardMode = false;
                  } 
                  else if (x == 32) {
                        if ( displayDataBuffer[x] == 1){
                            displayBit32 = true;
                        }
                        else displayBit32 = false;
                  } 
                  else if (x == 33) {
                        if ( displayDataBuffer[x] == 1){
                            displayBit33 = true;
                        }
                        else displayBit33 = false;
                  } 
                  else if (x == 34) {
                        if ( displayDataBuffer[x] == 1){
                            displayBit34 = true;
                        }
                        else displayBit34 = false;
                  } 
                  else if (x == 35) {
                        if ( displayDataBuffer[x] == 1){
                            displayHeater = true;
                        }
                        else displayHeater = false;
                  } 
                  else if (x == 36) {
                        if ( displayDataBuffer[x] == 1){
                            displayPump1 = true;
                        }
                        else displayPump1 = false;
                  } 
                  else if (x == 37) {
                        if ( displayDataBuffer[x] == 1){
                            displayPump2 = true;
                        }
                        else displayPump2 = false;
                  }   
                  else if (x == 38) {
                        if ( displayDataBuffer[x] == 1){
                            displayLight = true;
                        }
                        else displayLight = false;
                  }   
                 
            } 
        
           LCD_display_1 = lockup_LCD_character(LCD_segment_1);
           LCD_display_2 = lockup_LCD_character(LCD_segment_2); 
           LCD_display_3 = lockup_LCD_character(LCD_segment_3);
           LCD_display_4 = lockup_LCD_character(LCD_segment_4);  
      
           
             // check if temperature or something else is shown on LCD display
           
             // No temperature is shown
			 if(LCD_segment_1 == 0) {   
                  LCD_display = LCD_display_1 + LCD_display_2 + LCD_display_3 + LCD_display_4; 
             } 
             
			 // Temperature is shown
			 else {
                 
				float Temperature = (10 * LCD_display_1.toInt() + LCD_display_2.toInt() + 0.1 * LCD_display_3.toInt());
				 
				if (displayButton) {setTemperature = Temperature;}
				else {waterTemperature = Temperature;}  
				                 
				LCD_display = LCD_display_1 + LCD_display_2 + "." + LCD_display_3 + LCD_display_4; 
             }

                          
            displayDataBufferReady = false;
            attachInterrupt(clockPin, clockPinInterrupt, CHANGE);
}

 ICACHE_RAM_ATTR void BalboaInterface::clockPinInterrupt() {
	  
        
     if (!displayDataBufferReady) {
            
            if ((micros() - clockInterruptTime) >= durationNewCycle ) {             // New cycle detected  
                    dataIndex = 0;
                    clockBitCounter = 0;
                    displayDataBufferReady = false;
            }
            
            clockInterruptTime = micros();
            
            if (digitalRead(clockPin) == LOW) { digitalWrite(buttonPin,LOW); }
                     
            if (digitalRead(clockPin) == HIGH) {

                                         
                  // Write button data if requested
                  
                  if (writeDisplayData == true && clockBitCounter >= 39 && clockBitCounter <= 41){
                            
                          if (clockBitCounter == 39) {
                                 
                                  if (writeMode)           { digitalWrite(buttonPin,LOW);  }
                                  else if (writeTempUp)    { digitalWrite(buttonPin,HIGH); }
                                  else if (writeTempDown)  { digitalWrite(buttonPin,HIGH); }
                                  else if (writeLight)     { digitalWrite(buttonPin,LOW);  }
                                  else if (writePump1)     { digitalWrite(buttonPin,HIGH); }
                                  else if (writePump2)     { digitalWrite(buttonPin,LOW);  }
                                  else if (writePump3)     { digitalWrite(buttonPin,HIGH); }
                          }

                          else if (clockBitCounter == 40) {
                                 
                                  if (writeMode)           { digitalWrite(buttonPin,LOW); }
                                  else if (writeTempUp)    { digitalWrite(buttonPin,LOW); }
                                  else if (writeTempDown)  { digitalWrite(buttonPin,LOW); }
                                  else if (writeLight)     { digitalWrite(buttonPin,HIGH);}
                                  else if (writePump1)     { digitalWrite(buttonPin,HIGH);}
                                  else if (writePump2)     { digitalWrite(buttonPin,HIGH);}
                                  else if (writePump3)     { digitalWrite(buttonPin,HIGH);}
                          }

                          else if (clockBitCounter == 41) {
                                  
                                  if (writeMode)           { digitalWrite(buttonPin,HIGH); }
                                  else if (writeTempUp)    { digitalWrite(buttonPin,LOW);  }
                                  else if (writeTempDown)  { digitalWrite(buttonPin,HIGH); }
                                  else if (writeLight)     { digitalWrite(buttonPin,HIGH); }
                                  else if (writePump1)     { digitalWrite(buttonPin,LOW);  }
                                  else if (writePump2)     { digitalWrite(buttonPin,LOW);  }
                                  else if (writePump3)     { digitalWrite(buttonPin,HIGH); }

                                  writeMode = false;
                                  writeTempUp = false;
                                  writeTempDown = false;
                                  writeLight = false;
                                  writePump1 = false;
                                  writePump2 = false;
                                  writePump3 = false;
                        }
                  }
                  
                  // Read display data   
                                 
                  if ( clockBitCounter <= displayDataBits ) {
                            displayDataBuffer[dataIndex] = digitalRead(displayPin);
                            dataIndex++;
                  }
                  else if ( clockBitCounter == totalDataBits ){          // Total cycle has passed  
                           displayDataBufferReady = true;
                           detachInterrupt(digitalPinToInterrupt(clockPin));                         
                  }
                  else if ( clockBitCounter > totalDataBits ){
                          displayDataBufferOverflow  = true;
                  }
                  
                 clockBitCounter++; 
				 
           }
      }
} 

String BalboaInterface::lockup_LCD_character(int LCD_character) {

      
      switch (LCD_character) {
          case B0000000: return " ";  break;
          case B1111110: return "0";  break;
          case B0110000: return "1";  break;
          case B1101101: return "2";  break;
          case B1111001: return "3";  break;
          case B0110011: return "4";  break;
          case B1011011: return "5";  break;
          case B1011111: return "6";  break;
          case B1110000: return "7";  break;
          case B1111111: return "8";  break;
		  case B1110011: return "9";  break;    
          case B1110111: return "A";  break;
       // case B0011111: return "B";  break;
          case B1001110: return "C";  break;
       // case B0111101: return "D";  break;
          case B1001111: return "E";  break;
       // case B1000111: return "F";  break;  
          case B1011110: return "G";  break; 
          case B0110111: return "H";  break;
       // case B0000110: return "I";  break;
          case B0111100: return "J";  break;
       // case B1010111: return "K";  break;
          case B0001110: return "L";  break;
          case B1010100: return "M";  break;
          case B1110110: return "N";  break;
       // case B1111110: return "O";  break;
       // case B1100111: return "P";  break;
          case B1101011: return "Q";  break;
          case B1100110: return "R";  break;
       // case B1011011: return "S";  break;
       // case B0001111: return "T";  break;
          case B0111110: return "U";  break;
       // case B0111110: return "V";  break;
          case B0101010: return "W";  break;
       // case B0110111: return "X";  break;
       // case B0111011: return "Y";  break;
       // case B1101101: return "Z";  break;      
          case B1111101: return "a";  break;
          case B0011111: return "b";  break;
          case B0001101: return "c";  break; 
          case B0111101: return "d";  break;
          case B1101111: return "e";  break;
          case B1000111: return "f";  break;
       // case B1111011: return "g";  break;
          case B0010111: return "h";  break;
          case B0000100: return "i";  break;
          case B0000001: return "j";  break;
          case B1010111: return "k";  break;
          case B0000110: return "l";  break;
          case B0010100: return "m";  break;
          case B0010101: return "n";  break;
          case B0011101: return "o";  break;
          case B1100111: return "p";  break;
	  //  case B1110011: return "q";  break;
          case B0000101: return "r";  break;
       // case B1011011: return "s";  break;
          case B0001111: return "t";  break;
          case B0011100: return "u";  break;
       // case B0011100: return "v";  break;
       // case B0010100: return "w";  break;
       // case B0110111: return "x";  break;
          case B0111011: return "y";  break;
          default:       return "-";  break; // Error condition, displays vertical bars
             
      }
}

