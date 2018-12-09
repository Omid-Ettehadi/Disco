/*
   Creation & Computation - Digital Futures, OCAD University
   Created 30 Nov 2018
   by: April De Zen, & Veda Adnani, & Omid Ettehadi
*/

// Libraries
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

// Definitions
#define LED1pin 12
#define LED2pin 11
#define LED3pin 10
#define INSTRUMENT_1_NUM_LEDS 30
#define INSTRUMENT_2_NUM_LEDS 60
#define INSTRUMENT_3_NUM_LEDS 60

// Objects
// stripOne -> the Puzzle
// stripTwo -> the Ball 1
// stripThree -> the Ball 2
Adafruit_NeoPixel stripOne = Adafruit_NeoPixel(INSTRUMENT_1_NUM_LEDS, LED1pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTwo = Adafruit_NeoPixel(INSTRUMENT_2_NUM_LEDS, LED2pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripThree = Adafruit_NeoPixel(INSTRUMENT_3_NUM_LEDS, LED3pin, NEO_GRB + NEO_KHZ800);

// Variables
int LedOneStatus, LedTwoStatus, LedThreeStatus;

// Sampling
unsigned long lastSend;
int sendRate = 2000;

void setup() 
{
  // Begin serial connection
  Serial.begin(9600);

  // Initialize Strip One (the Puzzle status)
  stripOne.begin();
  stripOne.setBrightness(200);
  stripOne.show();

  delay(1000);

  // Initialize Strip Two (the Ball 1 status)
  stripTwo.begin();
  stripTwo.setBrightness(200);
  stripTwo.show();

  delay(1000);

  // Initialize Strip Three (the Ball 2 status)
  stripThree.begin();
  stripThree.setBrightness(200);
  stripThree.show();

  delay(1000);

  // One trial to test all the lights
  for ( int i = 0; i < 60; i++ )
  {
    if ( i < 30 ) 
    {
      stripOne.setPixelColor(i, 255, 255, 255);
      stripOne.show();
    }
    stripTwo.setPixelColor(i, 255, 255, 255);
    stripTwo.show();
    stripThree.setPixelColor(i, 255, 255, 255);
    stripThree.show();
  }
  for ( int i = 0; i < 60; i++ )
  {
    if ( i < 30 ) 
    {
      stripOne.setPixelColor(i, 0, 0, 0);
      stripOne.show();
    }
    stripTwo.setPixelColor(i, 0, 0, 0);
    stripTwo.show();
    stripThree.setPixelColor(i, 0, 0, 0);
    stripThree.show();
  }
}

void loop() 
{
  DynamicJsonBuffer messageBuffer(200);               // Buffer for the JSON object        
  JsonObject& p5Read = messageBuffer.parse(Serial);   // JsonObject attaches to incoming serial message
  
  LedOneStatus = p5Read["PuzzleStatus"];              // Assign "PuzzleStatus" from the json object to LedOneStatus
  LedTwoStatus = p5Read["BallOneStatus"];              // Assign "BallOneStatus" from the json object to LedTwoStatus
  LedThreeStatus = p5Read["BallTwoStatus"];          // Assign "BallTwoStatus" from the json object to LedThreeStatus


  // if both balls are not moving, do nothing
  if ( LedTwoStatus == 0 && LedThreeStatus == 0 )
  {
    // Do nothing
  }
  // if only ball one is moving, set strip two to purple and strip three to white 
  else if ( LedTwoStatus == 1 && LedThreeStatus == 0 )
  {
    for ( int i = 0; i < 60; i++ )
    {
      stripTwo.setPixelColor(i, 148, 0, 211);
      stripTwo.show();
      stripThree.setPixelColor(i, 255, 255, 255);
      stripThree.show();
    }
    for ( int i = 0; i < 60; i++ )
    { 
      stripTwo.setPixelColor(i, 0, 0, 0);
      stripTwo.show();
      stripThree.setPixelColor(i, 0, 0, 0);
      stripThree.show();
    }
  } 
  // if only ball two is moving, set strip two to white and strip three to blue 
  else if ( LedTwoStatus == 0 && LedThreeStatus == 1 )
  {
    for ( int i = 0; i < 60; i++ )
    {
      stripTwo.setPixelColor(i, 255, 255, 255);
      stripTwo.show();
      stripThree.setPixelColor(i, 0, 128, 128);
      stripThree.show();
    }
    for ( int i = 0; i < 60; i++ )
    {
      stripTwo.setPixelColor(i, 0, 0, 0);
      stripTwo.show();
      stripThree.setPixelColor(i, 0, 0, 0);
      stripThree.show();
    }
  }
  // if both ball one & two are moving, set strip two to purple and strip three to blue 
  else if ( LedTwoStatus == 1 && LedThreeStatus == 1 )
  {
    for ( int i = 0; i < 60; i++ )
    {
      stripTwo.setPixelColor(i, 148, 0, 211);
      stripTwo.show();
      stripThree.setPixelColor(i, 0, 128, 128);
      stripThree.show();
    }
    for ( int i = 0; i < 60; i++ )
    {
      stripTwo.setPixelColor(i, 0, 0, 0);
      stripTwo.show();
      stripThree.setPixelColor(i, 0, 0, 0);
      stripThree.show();
    }
  }

  
  // Every second run this
  // If no puzzle key is pressed, run a white flash light
  // else run a green flash light
  if( millis()-lastSend >= sendRate )
  {
    if ( LedOneStatus == 1 )
    {
      for ( int i = 0; i < 30; i++ )
      {
        stripOne.setPixelColor(i, 0, 255, 0);
        stripTwo.show();
      }
      for ( int i = 0; i < 30; i++ )
      {
        stripOne.setPixelColor(i, 0, 0, 0);
        stripOne.show();
      }
    } else {
      for ( int i = 0; i < 30; i++ )
      {
        stripOne.setPixelColor(i, 255, 255, 255);
        stripTwo.show();
      }
      for ( int i = 0; i < 30; i++ )
      {
        stripOne.setPixelColor(i, 0, 0, 0);
        stripOne.show();
      }
    }
    lastSend = millis();
  }
}
