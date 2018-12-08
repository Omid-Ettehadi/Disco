/*
   Creation & Computation - Digital Futures, OCAD University
   Created 30 Nov 2018
   by: April De Zen, & Veda Adnani, & Omid Ettehadi
*/

#define PubNub_BASE_CLIENT WiFiClient

// Libraries
#include "esp_wpa2.h"
#include <WiFi.h>
#include <Wire.h>
#include <PubNub.h>
#include <wifiDetails.h>
#include <ArduinoJson.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Adafruit_NeoPixel.h>

// Definitions
#define LEDpin 21
#define NUM_LEDS 17

// Objects
Adafruit_NeoPixel LEDstrip = Adafruit_NeoPixel(NUM_LEDS, LEDpin, NEO_GRB + NEO_KHZ800);
Adafruit_BNO055 orientationSensor = Adafruit_BNO055();

// PubNub Details
const static char pubkey[] = "pub-c-0a3b6b6e-0ef0-4b11-b8eb-1a597dc2331f";
const static char subkey[] = "sub-c-c92fc548-f343-11e8-b085-b2b44c5b7fba";
const static char pubChannel[] = "musicChannel";

// Variables
String whoAmI = "theBallOne";
float xOrientation = 0, yOrientation = 0, zOrientation = 0;                 // X, Y & Z Orientation
float lastOrientationX = 0, lastOrientationY = 0, lastOrientationZ = 0;     // Save previous X, Y & Z Orientation                           
float velocityX = 0, velocityY = 0, velocityZ = 0;                          // X, Y & Z Velocity
float averageVelocity = 0;                                                  // Average of X & Y & Z Velocities
bool LEDTrack = false;                                                      // Keep track of LEDs

// Data
float message = 0;                                                          // Average of all the reading of AverageVelocity during one second
int countMessage = 0;                                                       // Counts how many values has been read over one minute

// Sampling
unsigned long lastRead;
int sampleRate = 1000;

void setup()
{
  // Begin serial connection
  Serial.begin(9600);

  // Connect to Wifi and PubNub Server
  connectStandardWifi(0);
  PubNub.begin(pubkey, subkey);
  Serial.println("PubNub Connected");

  // Initialise the Orientation sensor
  if(!orientationSensor.begin())             
  {
    Serial.println("Can't connect to the Sensor. Check the Wiring");
    while(1);
  }

  // Setup LED Strip
  LEDstrip.begin();
  LEDstrip.setBrightness(100);
  LEDstrip.show();

  // Wait to make sure everything is correctly connected
  delay(2000); 

  // Set on the External Crystal for the Orientation Sensor 
  orientationSensor.setExtCrystalUse(true);
}

void loop() 
{
  sensors_event_t event;                                                    // Create an event variable
  orientationSensor.getEvent(&event);                                       // Pass it to the BNO055 object
  
  // Get the X, Y & Z Orientation Values 
  xOrientation = round(event.orientation.x);
  yOrientation = round(event.orientation.y);
  zOrientation = round(event.orientation.z);

  // Make sure the orientation values are all positive and between 0 & 360  
  if (yOrientation < 0) { yOrientation = 360 + yOrientation; }
  if (zOrientation < 0) { zOrientation = 360 + zOrientation; }
    
  // Calculate the angular velocity between the previous angle, and the current angle
  velocityX = lastOrientationX - xOrientation;
  velocityY = lastOrientationY - yOrientation;
  velocityZ = lastOrientationZ - zOrientation;

  // Make sure the velocities are all positive
  if (velocityX < 0) { velocityX = abs(velocityX); }
  if (velocityY < 0) { velocityY = abs(velocityY); }
  if (velocityZ < 0) { velocityZ = abs(velocityZ); }
    
  // Calculate the average of X & Y & Z velocities
  averageVelocity = ((velocityX + velocityY + velocityZ) / 3);

  message += averageVelocity;
  countMessage++;

  // Update the histories
  lastOrientationX = xOrientation;
  lastOrientationY = yOrientation;
  lastOrientationZ = zOrientation;
    
  /*
  // Debugging
  Serial.print("X: ");
  Serial.print(xOrientation);
  Serial.print("  Y:  ");
  Serial.print(yOrientation);
  Serial.print("  Z:  ");
  Serial.println(zOrientation);
  Serial.print("vel:  ");
  Serial.println(averageVelocity);
  */

  // Turn the LED Strip on if the ball has averageVelocity higher than 5
  if ( averageVelocity <= 5 ) 
  {
    if ( LEDTrack == true )
    {
      for ( int i = 0; i < NUM_LEDS; i++ )
      {
        LEDstrip.setPixelColor(i, 0, 0, 0);
        LEDstrip.show();
        delay(10);
      }
      LEDTrack = false;
    }
  } 
  else 
  {
    if ( LEDTrack == false )
    {
      for ( int i = 0; i < NUM_LEDS; i++ )
      {
        LEDstrip.setPixelColor(i, 148, 0, 211);
        LEDstrip.show();
        delay(10);
      }
      LEDTrack = true;
    }
  }
  
  // Send the average of the last readings to PubNub every sampleRate milisecond
  if( millis()-lastRead >= sampleRate )
  {
    publishToPubNub();
    lastRead = millis();
  }
}

/* -------------------- PubNub -------------------- */
void publishToPubNub()
{
  WiFiClient *client;
  DynamicJsonBuffer messageBuffer(600);                        // Create a memory buffer to hold a JSON Object
  JsonObject& pMessage = messageBuffer.createObject();         // Create a new JSON object in that buffer

  // The messages
  pMessage["who"] = whoAmI;
  pMessage["averageVelocity"] = message;

  message = 0;
  countMessage = 0;

  int mSize = pMessage.measureLength() + 1;                   // Determine the size of the JSON Message
  char msg[mSize];                                            // Create a char array to hold the message
  pMessage.printTo(msg, mSize);                               // Convert the JSON object into simple text (needed for the PN Arduino client)

  client = PubNub.publish(pubChannel, msg);                   // Publish the message to PubNub

  if (!client)                                                // Error check the connection
  {
    Serial.println("client error");
    delay(1000);
    return;
  }

  if (PubNub.get_last_http_status_code_class() != PubNub::http_scc_success)  // Check that it worked
  {
    Serial.print("Got HTTP status code error from PubNub, class: ");
    Serial.print(PubNub.get_last_http_status_code_class(), DEC);
  }

  while (client->available())                                 // Get feedback from PubNub
  {
    Serial.write(client->read());
  }
  client->stop();                                             // Stop the connection
  Serial.println("Successful Publish");
}
