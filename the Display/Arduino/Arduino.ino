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
Adafruit_NeoPixel stripOne = Adafruit_NeoPixel(INSTRUMENT_1_NUM_LEDS, LED1pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripTwo = Adafruit_NeoPixel(INSTRUMENT_2_NUM_LEDS, LED2pin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel stripThree = Adafruit_NeoPixel(INSTRUMENT_3_NUM_LEDS, LED3pin, NEO_GRB + NEO_KHZ800);

// Variables
int LedOneStatus , LedTwoStatus, LedThreeStatus;

// Sampling
unsigned long lastSend;
int sendRate = 2000;

void setup() 
{
  // Begin serial connection
  Serial.begin(9600);

  stripOne.begin();
  stripOne.setBrightness(200);
  stripOne.show();

  delay(1000);

  stripTwo.begin();
  stripTwo.setBrightness(200);
  stripTwo.show();

  delay(1000);

  stripThree.begin();
  stripThree.setBrightness(200);
  stripThree.show();

  delay(1000);

  for ( int i = 0; i < 30; i++ )
  {
    stripOne.setPixelColor(i, 255, 255, 255);
    stripOne.show();
  }
  for ( int i = 0; i < 30; i++ )
  {
    stripOne.setPixelColor(i, 0, 0, 0);
    stripOne.show();
  }
  for ( int i = 0; i < 60; i++ )
  {
    stripTwo.setPixelColor(i, 255, 255, 255);
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

void loop() 
{
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
  DynamicJsonBuffer messageBuffer(200);               // Buffer for the JSON object        
  JsonObject& p5Read = messageBuffer.parse(Serial);   // JsonObject attaches to incoming serial message
  
  LedOneStatus = p5Read["LEDOneStatus"];              // Assign "PuzzleStatus" from the json object to LedOneStatus
  LedTwoStatus = p5Read["LEDTwoStatus"];              // Assign "BallOneStatus" from the json object to LedTwoStatus
  LedThreeStatus = p5Read["LEDThreeStatus"];          // Assign "BallTwoStatus" from the json object to LedThreeStatus
    
    
  /*if ( LedOneStatus == 1 )
  {
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
  }*/
    
  if ( LedTwoStatus == 0 && LedThreeStatus == 0 )
  {
    /*for ( int i = 0; i < 60; i++ )
    {
      stripTwo.setPixelColor(i, 255, 255, 255);
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
    }*/
  } else if ( LedTwoStatus == 0 && LedThreeStatus == 1 )
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
  } else if ( LedTwoStatus == 1 && LedThreeStatus == 0 )
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
  } else if ( LedTwoStatus == 1 && LedThreeStatus == 1 )
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
}
