#include <PS2Keyboard.h>
#include <SPI.h>
#include <Ethernet.h>
#include <Adafruit_NeoPixel.h>

#include <RC.h>

#include "ps2.h"

// SET THESE
#define PUBLIC_KEY ""
#define PRIVATE_KEY ""
byte mac[] = { 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF };


// Whiteish for initialized
#define INITIALIZED_COLOR 50, 50, 50 

// Green for success
#define SUCCESS_COLOR 0, 30, 0

// Red for no such User
#define MAGNETIC_NOT_FOUND_COLOR 50, 0, 0

// Yellow for (network) error
#define ERROR_COLOR 10, 10, 0


#define MAGNETIC_IRQ_PIN 3
#define MAGNETIC_DATA_PIN 7

// magnetic is 9 numeric characters + null character 
#define MAGNETIC_LENGTH 10


PS2Keyboard rfidReader;

#define RFID_IRQ_PIN 2
#define RFID_DATA_PIN 5


#define NUM_PIXELS 40
#define PIXELS_PIN 6

// TODO: clean up PS/2 interfacing code
volatile boolean magneticReady;
volatile char magneticBuffer[MAGNETIC_LENGTH];
volatile uint8_t charCount;

String rfid;

void magneticReaderInterrupt();
void rfidReaderInterrupt();

/*
 * Sets the LEDs to the specified color.
 */
void setPixelsColor(uint8_t r, uint8_t g, uint8_t b);

/*
 * Return reverse endianness of val.
 */
uint32_t swap_uint32(uint32_t val);


EthernetClient ethernetClient;
APIClient api(ethernetClient, PUBLIC_KEY, PRIVATE_KEY);

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUM_PIXELS, PIXELS_PIN, NEO_GRB + NEO_KHZ800); 


void setup() {
  Serial.begin(9600);
  
  Serial.println(F("Welcome to CardReader"));
  
  if(!Ethernet.begin(mac)) {
    Serial.println(F("Failed to initialize ethernet shield"));
    return; 
  }
  
  // let ethernet shield initialize
  delay(1000);
  
  Serial.println(F("Connected to Internet"));
  
  pixels.begin();

  Serial.println(F("LEDs initialized"));
  
  // Magnetic always 9 characters, 10th null character
  magneticBuffer[MAGNETIC_LENGTH-1] = '\0';
  
  magneticReady = false;
  charCount = 0;
  
  // Flash LEDs to signal done initializing
  setPixelsColor(INITIALIZED_COLOR);
  delay(1000);
  setPixelsColor(0, 0, 0);
  
  attachInterrupt(1, magneticReaderInterrupt, FALLING);
  
  rfidReader.begin(RFID_DATA_PIN, RFID_IRQ_PIN);
}

void loop() {
  // Set to true if card swiped
  if(magneticReady) {
    Serial.print(F("Magnetic: "));
    String magnetic((const char*)magneticBuffer);
    Serial.println(magnetic);
    magneticReady = false;
    
    UserLookup userLookup;
    
    if(!api.magnetic(magnetic, "", userLookup)) {
      // network error
      setPixelsColor(ERROR_COLOR);

      Serial.println(F("Magnetic Lookup Failed"));
      Serial.println(api.lastResponseCode());
    } else {
      Serial.print(F("API Request ID: "));
      Serial.println(userLookup.api_request_id);
      
      if(userLookup.found) {
        Serial.print(F("User ID:"));
        Serial.println(userLookup.user_id);
        
        // give time for RFID to be sent
        delay(200);
    
        // read RFID decimal value
        while(rfidReader.available()) {
          char c = rfidReader.read();
          rfid += c;
        }
    
        // Convert to hex
        char rfidBuffer[rfid.length()];
        rfid.toCharArray(rfidBuffer, rfid.length());
    
        long rfidValue = atol((const char*)rfidBuffer);
    
        rfidValue = swap_uint32(rfidValue);

        String rfidHex = String(rfidValue, HEX);
    
        if(rfidHex.length() == 7) {
          rfidHex = "0" + rfidHex; 
        }
    
        rfidHex.toUpperCase();
        
        Serial.print(F("RFID: "));
        Serial.println(rfidHex);
        
        int apiRequestID;
        
        if(!api.userRFID(userLookup.user_id, rfidHex, "", apiRequestID)) {
          // network error
          setPixelsColor(ERROR_COLOR);
          
          Serial.println(F("User RFID Failed"));
        } else {
          Serial.print(F("API Request ID: "));
          Serial.println(apiRequestID);
          
          setPixelsColor(SUCCESS_COLOR);
          delay(3000);
          setPixelsColor(0, 0, 0);
        }
        
        rfid = "";
    
        while(rfidReader.available()) {
          rfidReader.read(); 
        }
      } else {
        Serial.println(F("User not found"));
        
        setPixelsColor(MAGNETIC_NOT_FOUND_COLOR);
        delay(3000);
        setPixelsColor(0, 0, 0);
      }
    }
  }
}

void magneticReaderInterrupt() {
  // bit shift count
  static uint8_t count = 0;
  
  // bit shift value
  static uint8_t temp = 0;
  
  // next keycode should be ignored
  // because current keycode a special keycode
  static boolean ignoreNext = false;
  
  // last time there was a new bit
  static uint32_t last_ms = 0;
  
  uint32_t now_ms = millis();
  
  // After 500ms reset bit shifter
  // (if not already 0)
  if(now_ms - last_ms > 500) {
    count = 0;
    temp = 0;
    ignoreNext = false;
    charCount = 0; 
  }
  
  uint8_t value = digitalRead(MAGNETIC_DATA_PIN);
  
  if(count == 0) {
    //start bit
  } else if(count <= 8) {
    temp |= (value << (count - 1)); 
  } else if(count == 9) {
    // parity bit
  } else if(count == 10) {
    //end bit
  }
  
  count++;
  
  // done reading keycode
  if(count == 11) {
    // ignore next keycode since
    // this keycode signals next one is
    // a special character
    if(temp == 0xF0) {
      ignoreNext = true;
    
    // still magnetic characters to read
    } else if(charCount <= 9) {
      if(ignoreNext) {
        ignoreNext = false;
      } else {
        char c = scanCodeToNumeric(temp);
        
        /*
         * if numeric character add to buffer
         * else ignore character
         */
        if(c != ' ') {
          // ignore first '%' character
          if(charCount >= 1 && charCount < MAGNETIC_LENGTH) {
            magneticBuffer[charCount-1] = c; 
          }
          
          charCount++;
          
          // done reading magnetic
          if(charCount == MAGNETIC_LENGTH) {
            magneticReady = true; 
          }
        }
      }
    }
    
    count = 0;
    temp = 0;
  }
  
  last_ms = now_ms;
}

void setPixelsColor(uint8_t r, uint8_t g, uint8_t b) {
  for(int i = 0; i < pixels.numPixels(); i++){
    pixels.setPixelColor(i, pixels.Color(r, g, b));
    pixels.show();
  } 
}

uint32_t swap_uint32(uint32_t val)
{
  val = ((val << 8) & 0xFF00FF00 ) | ((val >> 8) & 0xFF00FF ); 
  return (val << 16) | (val >> 16);
}

