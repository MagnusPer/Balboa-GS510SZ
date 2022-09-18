
#ifndef Balboa_GS_Interface_h
#define Balboa_GS_Interface_h

#include <Arduino.h>


const byte displayDataBufferSize       		= 40;		// Size of display data buffer
const byte displayDataBits             		= 38;       // 0-38 bits length of display data within a cycle
const byte buttonDataBits              		= 2;        // 0-2 bits length of button data within a cycle
const byte totalDataBits               		= 41;       // 0-41 total number of pulses within a cycle
const unsigned int durationNewCycle    		= 5000;		// How many microsecounds to detect new cycle if no interrupt occurs 
const unsigned long buttonPressTimerMillis  = 500;  	// Timer in milliseconds between update temperature button presses 


class BalboaInterface {

  public:
	
	BalboaInterface(byte setClockPin, byte setReadPin, byte setWritePin);
	
	// Interface control
	void begin();						           	// Initializes the stream output to Serial by default
    bool loop();                                    // Returns true if valid data is available
    void stop();                                    // Disables the clock hardware interrupt 
    void resetStatus();                             // Resets the state of all status components as changed for sketches to get the current status	
	void updateTemperature(float Temperature);		// Function to set the water temperature 	

	// Status tracking
	float waterTemperature;                			// Water temperatur 
	float setTemperature;                			// The wanted set temperature   
	String LCD_display;								// The text shown on display 
	bool displayButton;        						// Temp up/down button pressed
	bool displayBit29;        						// Still unknown functionality, if at all used!
	bool displayBit30;        						// Still unknown functionality, if at all used!
	bool displayStandardMode;        				// Standard Mode activated or not
	bool displayBit32;        						// Still unknown functionality, if at all used!
	bool displayBit33;        						// Still unknown functionality, if at all used!  
	bool displayBit34;        						// Still unknown functionality, if at all used!
	bool displayHeater;        						// Heater running or not
	bool displayPump1;        						// Pump 1 running or not 
	bool displayPump2;        						// Pump 2 running or not
	bool displayLight;        						// Hot tube lights activated or not
	static bool displayDataBufferOverflow;
	
	// Write button data to control unit  
	static bool writeDisplayData;            		// If something should be written to button data line  
	static bool writeMode;
	static bool writeTempUp;
	static bool writeTempDown;
	static bool writeLight;
	static bool writePump1;
	static bool writePump2;		
	static bool writePump3;

  private:
	
	static void clockPinInterrupt();
	void decodeDisplayData();
	String lockup_LCD_character(int LCD_character);
	int LCD_segment_1;
	int LCD_segment_2;
	int LCD_segment_3;
	int LCD_segment_4;
	String LCD_display_1;
	String LCD_display_2;
	String LCD_display_3;
	String LCD_display_4;  
	static byte displayDataBuffer[displayDataBufferSize]; 	// Array of display data measurements 
	static unsigned long clockInterruptTime;
	static int clockBitCounter;               		 		// Counter of pulses within a cycle
	static byte dataIndex; 									
	static bool displayDataBufferReady;            			// Is buffer available to be decoded
	static byte clockPin;
    static byte displayPin;
    static byte buttonPin;

	int updateTempDirection;
	int updateTempButtonPresses;
	unsigned long buttonPressTimerPrevMillis; 
};

  
#endif  // Balboa_GS_Interface_h