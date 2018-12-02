/*
   Creation & Computation - Digital Futures, OCAD University
   Created 30 Nov 2018
   by: April De Zen, & Veda Adnani, & Omid Ettehadi
*/

// Libraries
#include <ArduinoJson.h>

// Definitions
#define SWITCH1pin 2
#define SWITCH2pin 3
#define SWITCH3pin 4
#define SWITCH4pin 5
#define SWITCH5pin 6
#define SWITCH6pin 7

#define LED1pin 8
#define LED2pin 9
#define LED3pin 10
#define LED4pin 11
#define LED5pin 12
#define LED6pin 13

// Variables
int switch1val;
int switch2val;
int switch3val;
int switch4val;
int switch5val;
int switch6val;

// Sampling
unsigned long lastSend;
int sendRate = 500;

void setup() 
{
  Serial.begin(9600);
  
  // Assign pins
  pinMode(SWITCH1pin, INPUT_PULLUP);
  pinMode(SWITCH2pin, INPUT_PULLUP);
  pinMode(SWITCH3pin, INPUT_PULLUP);
  pinMode(SWITCH4pin, INPUT_PULLUP);
  pinMode(SWITCH5pin, INPUT_PULLUP);
  pinMode(SWITCH6pin, INPUT_PULLUP);
  pinMode(LED1pin, OUTPUT);
  pinMode(LED2pin, OUTPUT);
  pinMode(LED3pin, OUTPUT);
  pinMode(LED4pin, OUTPUT);
  pinMode(LED5pin, OUTPUT);
  pinMode(LED6pin, OUTPUT);
}

void loop() 
{
  // Set switch values to zero if they are open, and one if they are one
  if ( digitalRead(SWITCH1pin) == HIGH ) { switch1val = 0; } else { switch1val = 1; }
  if ( digitalRead(SWITCH2pin) == HIGH ) { switch2val = 0; } else { switch2val = 1; }
  if ( digitalRead(SWITCH3pin) == HIGH ) { switch3val = 0; } else { switch3val = 1; }
  if ( digitalRead(SWITCH4pin) == HIGH ) { switch4val = 0; } else { switch4val = 1; }
  if ( digitalRead(SWITCH5pin) == HIGH ) { switch5val = 0; } else { switch5val = 1; }
  if ( digitalRead(SWITCH6pin) == HIGH ) { switch6val = 0; } else { switch6val = 1; }

  // Check how many switches are on, turn that many LEDs on
  // See how many switches are connected
  int sum = switch1val + switch2val + switch3val + switch4val + switch5val + switch6val;
    
  // If no switch is connected turn all LEDs off
  if (switch1val == LOW && switch2val == LOW && switch3val == LOW 
  && switch4val == LOW && switch5val == LOW && switch6val == LOW)
  {
    digitalWrite(LED1pin, LOW);
    digitalWrite(LED2pin, LOW);
    digitalWrite(LED3pin, LOW);
    digitalWrite(LED4pin, LOW);
    digitalWrite(LED5pin, LOW);
    digitalWrite(LED6pin, LOW);

  } 
  else if ( sum == 1 ) 
  {
    digitalWrite(LED1pin, HIGH);
    digitalWrite(LED2pin, LOW);
    digitalWrite(LED3pin, LOW);
    digitalWrite(LED4pin, LOW);
    digitalWrite(LED5pin, LOW);
    digitalWrite(LED6pin, LOW); 
  }
  else if ( sum == 2 ) 
  {
    digitalWrite(LED1pin, HIGH);
    digitalWrite(LED2pin, HIGH);
    digitalWrite(LED3pin, LOW);
    digitalWrite(LED4pin, LOW);
    digitalWrite(LED5pin, LOW);
    digitalWrite(LED6pin, LOW);
  }
  else if ( sum == 3 ) 
  {
    digitalWrite(LED1pin, HIGH);
    digitalWrite(LED2pin, HIGH);
    digitalWrite(LED3pin, HIGH);
    digitalWrite(LED4pin, LOW);
    digitalWrite(LED5pin, LOW);
    digitalWrite(LED6pin, LOW); 
  }
  else if ( sum == 4 ) 
  {
    digitalWrite(LED1pin, HIGH);
    digitalWrite(LED2pin, HIGH);
    digitalWrite(LED3pin, HIGH);
    digitalWrite(LED4pin, HIGH);
    digitalWrite(LED5pin, LOW);
    digitalWrite(LED6pin, LOW); 
  }
  else if ( sum == 5 ) 
  {
    digitalWrite(LED1pin, HIGH);
    digitalWrite(LED2pin, HIGH);
    digitalWrite(LED3pin, HIGH);
    digitalWrite(LED4pin, HIGH);
    digitalWrite(LED5pin, HIGH);
    digitalWrite(LED6pin, LOW);
  }
  else if (sum == 6 ) 
  {
    digitalWrite(LED1pin, HIGH);
    digitalWrite(LED2pin, HIGH);
    digitalWrite(LED3pin, HIGH);
    digitalWrite(LED4pin, HIGH);
    digitalWrite(LED5pin, HIGH);
    digitalWrite(LED6pin, HIGH);  
  }

  // Send data through Serial Connection to the P5
  if( millis()-lastSend >= sendRate )
  {
    DynamicJsonBuffer messageBuffer(512);                   // Create the Buffer for the JSON object        
    JsonObject& p5Send = messageBuffer.createObject();      // Create a JsonObject variable in that buffer       
    
    p5Send["switch1"] = switch1val;                         // Assign switch1val to the key "switch1" in the json object
    p5Send["switch2"] = switch2val;                         // Assign switch2val to the key "switch2" in the json object
    p5Send["switch3"] = switch3val;                         // Assign switch3val to the key "switch3" in the json object
    p5Send["switch4"] = switch4val;                         // Assign switch4val to the key "switch4" in the json object
    p5Send["switch5"] = switch5val;                         // Assign switch5val to the key "switch5" in the json object
    p5Send["switch6"] = switch6val;                         // Assign switch6val to the key "switch6" in the json object
    
    p5Send.printTo(Serial);                                 // Print JSON object as a string
    Serial.println();                                       // Print a \n character to the serial port to distinguish between objects

    lastSend = millis();
  }
}
