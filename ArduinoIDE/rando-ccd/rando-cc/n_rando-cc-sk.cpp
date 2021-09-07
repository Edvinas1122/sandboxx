#include "p_rando.h"
// swap these two to use 'cppcheck --enable=all ./thisfile.cpp'
#define N_CPPCHECK
#undef  N_CPPCHECK

#ifndef  N_CPPCHECK
#include <Arduino.h>
#endif

#define DDEBUG_LVL 0

// https://en.wikipedia.org/wiki/Icosahedron

const char icosahdrn_face_message[][26] { // exact count
    "It is certain            ",
    "Without a doubt          ",
    "You may rely on it       ",
    "Yes definitely           ",
    "It is decidedly so       ",
    "As I see it, yes         ",
    "Most likely              ",
    "Yes                      ",
    "Outlook good             ",
    "Signs point to yes       ",
    "Reply hazy try again     ",
    "Better not tell you now  ",
    "Ask again later          ",
    "Cannot predict now       ",
    "Concentrate and ask again",
    "Don’t count on it        ",
    "Outlook not so good      ",
    "My sources say no        ",
    "Very doubtful            ",
    "My reply is no           ",};

void versionPrint (void) {
    Serial.println (DATESTAMP);
}

int generateAnswer (void) {
    int genRnd = random (1, 20);
    Serial.print ("rando: ");
    Serial.println (genRnd);
    return genRnd;
}

void reportIcosaFaceMsg (int icos_idx) {
    if (DDEBUG_LVL == 2) {
        Serial.print ("bracket >> ");
    }

    Serial.print (icosahdrn_face_message[icos_idx]);

    if (DDEBUG_LVL == 0) {
        Serial.println ();
    }

    if (DDEBUG_LVL == 2) {
        Serial.println (" << bracket");
    }
}

void printAnswer (int answer) {
    reportIcosaFaceMsg (answer);
}

void answerQuestion (void) {
    int answ = generateAnswer ();
    printAnswer (answ);
}

extern void readword (void); // keyboard input via USB serial port

void waitForQuestion (void) {
    readword ();
}

void greet (void) {
    Serial.println ("Welcome to Magic 8 ball");
    Serial.println ("Please ask your question");
}

#define ANA_PIN 26

void randomize (void) {
    randomSeed (analogRead (ANA_PIN));
}

void hold_booting (void) {
    delay (1100);
}

extern void init_gpio (void);
extern void init_serial (void);

void setup (void) {
    hold_booting ();
    init_gpio ();
    init_serial ();
    versionPrint ();
    greet ();
    randomize ();
}

void loop (void) {
    waitForQuestion ();
    answerQuestion ();
}

#ifdef  N_CPPCHECK
int main (void) {
    setup ();
    loop ();
}
#endif

// $  cppcheck --enable=all ./n_rando-cc-sk.cpp
// Checking n_rando-cc-sk.cpp ...
// END
