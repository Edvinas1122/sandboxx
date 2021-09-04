#define DATESTAMP "                  Wed Aug 25 21:33:39 UTC 2021"

/* Includes Charley Shattuck's Tiny interpreter,
   similar to myforth's Standalone Interpreter
   Charley's example code is in the public domain */

// swap these two to use 'cppcheck --enable=all ./thisfile.cpp'
#define N_CPPCHECK
#undef  N_CPPCHECK

#ifndef  N_CPPCHECK
#include <Arduino.h>
#endif

#define DDEBUG_LVL 0

// verbatim:

/* Tiny interpreter,
   similar to myforth's Standalone Interpreter
   This example code is in the public domain */

/* Structure of a dictionary entry */
typedef struct {
    const char *name;
    void (*function) ();
} entry;

/* Data stack for parameter passing
   This "stack" is circular,
   like a Green Arrays F18A data stack,
   so overflow and underflow are not possible
   Number of items must be a power of 2 */
const int STKSIZE = 8;
const int STKMASK = 7;
int stack[STKSIZE];
int p = 0;

/* TOS is Top Of Stack */
#define TOS stack[p]
/* NAMED creates a string in flash */
#define NAMED(x, y) const char x[]=y

/* Terminal Input Buffer for interpreter */
const byte maxtib = 64; // 16 may be more appropriate
char tib[maxtib];
/* buffer required for strings read from flash */
char namebuf[maxtib];
byte pos;
char ch;

#define EOL_CHAR '\n'
/* Incrementally read command line from serial port */
byte reading () {
    if (!Serial.available ())
        return 1;
    ch = Serial.read ();
    Serial.print (ch); // keystroke echo.  OPTIONAL.

    if (ch == EOL_CHAR) {
        Serial.print ('\r');
    } // try to do CR without LF here
    // if ((ch == EOL_CHAR) || (ch == ' ')) {
    if (ch == EOL_CHAR) { // not parsing using spaces now
        return 0;
    }
    if ((ch == '\010') || (ch == '\177')) { // backspace or rubout
        if (ch == '\177') {
            Serial.print ("\010"); // 0x08
            Serial.print (" mutt and jeff "); // 0x08
        }
        Serial.print (' ');
        Serial.print ("\010"); // 0x08
        if (pos == 0) {
            return 1;
        }
        tib[pos--] = 0;
        tib[pos] = 0;
        return 1; // continue reading keystrokes for this word's name &c.
    } // backspace or rubout

    if (pos < maxtib) {
        tib[pos++] = ch;
        tib[pos] = 0;
    }
    // individual, non-special compositional keystrokes
    // fall through to here:
    return 1;
}

/* Block on reading the command line from serial port */
/* then echo each word */
void readword () {
    pos = 0;
    tib[0] = 0;
    while (reading ());
    if (DDEBUG_LVL == 2) {
        Serial.print (tib);
        Serial.print (" ");
        Serial.println ("  that was \'tib\'  for you.");
    }
}

/* Run a word via its name */
void runword (void) {
}

bool pin_state; // is this really a bool or just an int that is equal to zero, or to one

// int wasted;

void cpl (int pin) {
    // wasted = pin;
    pin_state = digitalRead (LED_BUILTIN);
    pin_state = !pin_state; // how does this interface to typing as bool or as int
    digitalWrite (LED_BUILTIN, pin_state);
}

void delayed (void) {
    // if (pin_state != 0) {
    if (pin_state != false) { // obeyed bool typing - made explicit.  mistake?
        delay (10);
        return;
    }
    delay (3000); // harmless - waiting for serial
}

void blink (void) {
    cpl (LED_BUILTIN);
    delayed ();
}

void init_gpio (void) {
    pinMode (LED_BUILTIN, 1);
    digitalWrite (LED_BUILTIN, 0);
}

void init_serial (void) {
    Serial.begin (115200);
    while (!Serial) {
        blink ();
    }
}

// int BASE;
/*
void unused_setup (void) {
    init_gpio ();
    init_serial ();
    delay (30); // vestigal - may be removed
    Serial.println ("here.");
    Serial.println (DATESTAMP);
    Serial.print ("__LED_BUILTIN__ ");
    Serial.println (wasted); // temporary - delete me soon
 // BASE = -99;
}
*/

void interpret_loop (void) {
    readword ();
//  runword ();
    // runword_old();
}

#ifdef  N_CPPCHECK
int main (void) {
    setup ();
    loop ();
}
#endif

// ENiD,
//  $ cppcheck --enable=style n_forth-avr-d.cpp
// Checking n_forth-avr-d.cpp ...
// [n_forth-avr-d.cpp:229]: (style) Non-boolean value returned from function returning bool
// [n_forth-avr-d.cpp:233]: (style) Non-boolean value returned from function returning bool

// END.
