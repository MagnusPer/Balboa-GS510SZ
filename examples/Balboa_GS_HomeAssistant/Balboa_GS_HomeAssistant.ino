  /*
 *    
 *    Main board: Wemos D1 mini - esp8266
 *  
 *    SPA display controller for Balboa system GS510SZ 
 *    
 */
    

#include <WiFiClient.h>
#ifdef ESP32
#include <WebServer.h>
#include <WiFi.h>
#else
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>  
#endif
#include <ElegantOTA.h>                  // https://github.com/ayushsharma82/ElegantOTA
#include <ArduinoHA.h>
#include <Balboa_GS_Interface.h>        // https://github.com/MagnusPer/Balboa-GS510SZ    

#define setClockPin D1  
#define setReadPin  D2 
#define setWritePin D8  


//Constants
const char *wifi_ssid                    = "";          // WiFi SSID
const char *wifi_pwd                     = "";          // WiFi Password 
const char *wifi_hostname                = "SPA";
const char* mqtt_server                  = "";           // MQTT Boker IP, your home MQTT server eg Mosquitto on RPi, or some public MQTT
const int mqtt_port                      = 1883;        // MQTT Broker PORT, default is 1883 but can be anything.
const char *mqtt_user                    = "";          // MQTT Broker User Name
const char *mqtt_pwd                     = "";          // MQTT Broker Password 
String clientId                          = "SPA : " + String(ESP.getChipId(), HEX);

//Globals 
bool debug                               = true;    // If true activate debug values to write to serial port

const unsigned long ReportTimerMillis    = 30000;   // Timer in milliseconds to report mqtt topics 
unsigned long ReportTimerPrevMillis      = 0;       // Store previous millis

byte mac[] = {0x00, 0x10, 0xFA, 0x6E, 0x38, 0x4A};  // Leave this value, unless you own multiple hot tubs


// MQTT Constants
const char* mqtt_Display_topic              = "SPA/Display";
const char* mqtt_SetTemp_topic              = "SPA/SetTemp";
const char* mqtt_WaterTemp_topic            = "SPA/WaterTemp";
const char* mqtt_Heater_topic               = "SPA/Heater";
const char* mqtt_Pump1_topic                = "SPA/Pump1";
const char* mqtt_Pump2_topic                = "SPA/Pump2";
const char* mqtt_Lights_topic               = "SPA/Lights";
const char* mqtt_Subscribe_write_topic      = "SPA/Write"; 
const char* mqtt_Subscribe_updateTemp_topic = "SPA/UpdateTemp";

//Initialize components
WiFiClient espClient;                                           // Setup WiFi client definition WiFi
HADevice device(mac, sizeof(mac));
HAMqtt mqtt(espClient, device, 30);
BalboaInterface Balboa(setClockPin, setReadPin, setWritePin);   // Setup Balboa interface 
#ifdef ESP32
WebServer webserver(80);
#else
ESP8266WebServer webserver(80);
#endif

 
/**************************************************************************/
/* Setup                                                                  */
/**************************************************************************/

void setup() {
  
  if (debug) { Serial.begin(115200); Serial.println("Welcome to SPA - Balboa system GS510SZ");}
  setup_wifi();
  setup_HA();
  Serial.begin(115200);
  Balboa.begin();

  server.on("/", []() {
    server.send(200, "text/plain", "Hi! I am ESP8266.");
  });
  
  ElegantOTA.begin(&server);    // Start ElegantOTA
  server.begin();
  if (debug) { Serial.println("HTTP server started"); }

}


/**************************************************************************/
/* Setup WiFi connection                                                  */
/**************************************************************************/

void setup_wifi() {

    /*  WiFi status return values and meaning 
        WL_IDLE_STATUS      = 0,
        WL_NO_SSID_AVAIL    = 1,
        WL_SCAN_COMPLETED   = 2,
        WL_CONNECTED        = 3,
        WL_CONNECT_FAILED   = 4,
        WL_CONNECTION_LOST  = 5,
        WL_WRONG_PASSWORD   = 6,
        WL_DISCONNECTED     = 7 */
  
    if (debug){ Serial.print("WiFi.status(): "); Serial.println(WiFi.status()); }
    
    int WiFi_retry_counter = 0;
    WiFi.mode(WIFI_STA);
    WiFi.hostname(wifi_hostname);
    WiFi.setSleepMode(WIFI_NONE_SLEEP);
    WiFi.begin(wifi_ssid, wifi_pwd);
    
    // Loop until reconnected or max retry then restart
    while (WiFi.status() != WL_CONNECTED){
        WiFi_retry_counter ++;
        if (WiFi_retry_counter == 30) {ESP.restart();}  
        if (debug){ Serial.print("WiFi.status(): "); Serial.print(WiFi.status()); 
                    Serial.print("   WiFi retry: "); Serial.println(WiFi_retry_counter); } 
        delay(1000);
    }
    
    if (debug){ Serial.print("WiFi connected: ");Serial.println(WiFi.localIP());}

}


void setup_HA() {
    device.setName("Hottub");
    device.setSoftwareVersion("0.0.1");
    device.setManufacturer("Balboa");
    device.setModel("GS");

    mqtt.begin(BROKER_ADDR, BROKER_USERNAME, BROKER_PASSWORD);

}

/**************************************************************************/
/* Main loop                                                              */
/**************************************************************************/

void loop() {

	Balboa.loop();
  mqtt.loop();
  server.handleClient();
  
  if (WiFi.status() != WL_CONNECTED){ setup_wifi(); }             // Check WiFi connnection reconnect otherwise 
  if (!client.connected()) { reconnect(); }                       // Check MQTT connnection reconnect otherwise 


 
    if(millis() - ReportTimerPrevMillis  > ReportTimerMillis) {
    
          ReportTimerPrevMillis = millis();
          
          client.publish(mqtt_Display_topic, String(Balboa.LCD_display).c_str());
          client.publish(mqtt_SetTemp_topic, String(Balboa.setTemperature).c_str());
          client.publish(mqtt_WaterTemp_topic, String(Balboa.waterTemperature).c_str());
          client.publish(mqtt_Heater_topic, String(Balboa.displayHeater).c_str());
          client.publish(mqtt_Pump1_topic , String(Balboa.displayPump1).c_str());
          client.publish(mqtt_Pump2_topic , String(Balboa.displayPump2).c_str());
          client.publish(mqtt_Lights_topic , String(Balboa.displayLight).c_str());
   
    } 
     
}

/**************************************************************************/
/* Subscribe to MQTT topic                                                */
/**************************************************************************/

void callback(char* topic, byte* payload, unsigned int length) {

      char c_payload[length];
      memcpy(c_payload, payload, length);
      c_payload[length] = '\0';
  
      String s_topic = String(topic);
      String s_payload = String(c_payload);
  
    // Handling incoming messages

    Serial.println(s_topic);
    Serial.println(s_payload);

      if ( s_topic == mqtt_Subscribe_write_topic ) {
         
             if (s_payload == "TempUp") {
                  Balboa.writeDisplayData = true; 
                  Balboa.writeTempUp      = true;
             }
             else if (s_payload == "TempDown") {
                  Balboa.writeDisplayData = true;
                  Balboa.writeTempDown    = true;  
             }
             else if (s_payload == "Light") {
                  Balboa.writeDisplayData = true;
                  Balboa.writeLight       = true; 
             }
             else if (s_payload == "Pump1") {
                  Balboa.writeDisplayData = true;
                  Balboa.writePump1       = true;  
             }
             else if (s_payload == "Pump2") {
                  Balboa.writeDisplayData = true;
                  Balboa.writePump2       = true;  
             }
             else if (s_payload == "Pump3") {
                  Balboa.writeDisplayData = true;
                  Balboa.writePump3       = true;  
             }
             else if (s_payload == "Mode") {
                  Balboa.writeDisplayData = true;
                  Balboa.writeMode        = true;  
             }
             else if (s_payload == "Stop") {
                  Balboa.stop();
             }
             else if (s_payload == "Reset") {
                ESP.restart();
             }
             
           
      }

      if ( s_topic == mqtt_Subscribe_updateTemp_topic) {
        
            Balboa.updateTemperature(s_payload.toFloat());
      }

      
}
      