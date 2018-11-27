// 27 NOV 2018 14:23z

#define DATENOW "Tue 27 Nov Mar 2018  14:22z"
#include "config.h"
#include "src/periph/dotstar.h"
extern void setup_dotstar();
extern void loop_dotstar();

/*
Adafruit Arduino - Lesson 4. 8 LEDs and a Shift Register
Simon Monk
mods: Christopher W Hafey, wa1tnr  27 November 2018
*/

// Trinket M0
int latchPin = 2;
int clockPin = 3;
int dataPin  = 4;

byte leds = 0;
byte uleds = 0;
byte pos = 15; // rightmost

byte slew = 5;

void setup(void) 
{
  setup_dotstar();

  Serial.begin(9600);

  // while(!Serial) {

  for (int i = 4 ; i > 0 ; i--) {
      loop_dotstar();
  }

  Serial.print(DATENOW);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin,  OUTPUT);  
  pinMode(clockPin, OUTPUT);

}

void _digitSelect(void) {
   uleds = pos;
   shiftOut(dataPin, clockPin, MSBFIRST, uleds);
}

void _updateSR(void) {
   digitalWrite(latchPin, LOW);

   _digitSelect();
   uleds = leds; // sequential digit
   shiftOut(dataPin, clockPin, MSBFIRST, uleds);

   digitalWrite(latchPin, HIGH);
}

void updateShiftRegister(void) {
   // uleds = 0;     _updateSR();
   uleds = leds;  _updateSR();
}

uint8_t ledval = 0;
uint8_t oldval = 0;

void unfunc(uint8_t ledval) {
    int8_t i;
    for (i=7; i>=0; i--) {
        if (ledval &  (1 <<  (i)) ) {
            // digitalWrite(sid, HIGH);
            Serial.print("1");
        } else {
            // digitalWrite(sid, LOW);
            Serial.print("0");
        }
    }
    Serial.print(" ~stanza~ ");
}


void blankleds(void) {
    leds = 0;
    updateShiftRegister();
}


void setleds(void) {
    leds = ledval; updateShiftRegister();
    delay(1); // CRITICAL - must be a finite, non-zero delay here
}

void _puteye(void) {
  if (EXPOSE_DIGIT_PAINTING) {
      delay(122); // to expose digit change
  }
  setleds();
  blankleds();
}

void _outeye(void) {
  pos = 15 ; _puteye();
  pos = 22 ; _puteye();
  pos = 27 ; _puteye();
  pos = 29 ; _puteye();
}

void outeye(void) {
    int duration = 322;
    for (int i = duration; i>0; i--) {
        _outeye();
    }
}

void dig_zero(void) {
  int i = 0;

  // 0 
  i = 1 + 2 + 4 + 8 + 16 + 32 +  0 +   0;
      ledval = i; outeye();
}

void dig_one(void) {
  int i = 0;

  // 1
  i = 0 + 2 + 4 + 0 +  0 +  0 +  0 +   0;
      ledval = i; outeye();
}


void loop(void) 
{
  blankleds();
  delay(40);

  ledval = 0;
  oldval = 0;

  int i = 0;

  dig_zero(); dig_one();
  dig_zero(); dig_one();
  dig_zero(); dig_one();
  dig_zero(); dig_one();

  // 2     // 1 2 8 16 64
  i = 1 + 2 + 0 + 8 + 16 +  0 + 64 +   0;
      ledval = i; outeye();

  // 3
  i = 1 + 2 + 4 + 8 +  0 +  0 + 64 +   0;
      ledval = i; outeye();

  // 4   //  2 4 32 64
  i = 0 + 2 + 4 + 0 +  0 + 32 + 64 +   0;
      ledval = i; outeye();

  // 5   // like 2
  i = 1 + 0 + 4 + 8 +  0 + 32 + 64 +   0;
      ledval = i; outeye();

  // 6
  i = 1 + 0 + 4 + 8 + 16 + 32 + 64 +   0;
      ledval = i; outeye();

  // 7
  i = 1 + 2 + 4 + 0 +  0 +  0 +  0 +   0;
      ledval = i; outeye();

  // 8
  i = 1 + 2 + 4 + 8 + 16 + 32 + 64 +   0;
      ledval = i; outeye();

  // 9
  i = 1 + 2 + 4 + 0 +  0 + 32 + 64 +   0;
      ledval = i; outeye();

  // A
  i = 1 + 2 + 4 + 16 + 32 + 64;
      ledval = i; outeye();

  // b
  i = 4 + 8 + 16 + 32 + 64;
      ledval = i; outeye();

  // C
  i = 1 + 8 + 16 + 32;
      ledval = i; outeye();

  // d
  i = 2 + 4 + 8 + 16 + 64;
      ledval = i; outeye();

  // E
  i = 1 + 8 + 16 + 32 + 64;
      ledval = i; outeye();

  // F
  i = 1 + 0 + 16 + 32 + 64;
      ledval = i; outeye();


  i = 1 + 2 + 4 + 8 + 16 + 32 + 64 + 128;
  i = 0;

  // hold display blank for a while:
  i = 128; ledval = i; outeye(); blankleds();
      delay(111 + slew);

  // hold display blank for even a bit longer:
  i = 128; ledval = i; outeye(); blankleds();
      delay(111 + slew);

}


/*
#define EXPOSE_DIGIT_PAINTING -1
*/
#define EXPOSE_DIGIT_PAINTING  0