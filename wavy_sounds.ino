#include <NewPing.h>
#include <SoftwareSerial.h>


#define PIN_RESET_MIDI 4
#define PIN_ACTIVITY_LED 13
#define PIN_RX 2
#define PIN_TX 3
#define PIN_SONAR_ECHO 9
#define PIN_SONAR_TRIGGER 10

#define MAX_DISTANCE 100
#define NOTE_DURATION 500 //in ms
#define TRIGGER_DISTANCE 20

SoftwareSerial midiSerial(PIN_RX, PIN_TX);
NewPing sonar(PIN_SONAR_TRIGGER, PIN_SONAR_ECHO, MAX_DISTANCE);
    

byte note      = 60; //The MIDI note value to be played
int instrument = 94;

void setup() {
  Serial.begin(57600);
  midiSerial.begin(31250);
  resetMIDI();
  initMIDI();
}

void loop() {
  int distance = sonar.ping_cm();
  if (triggered(distance)) {
    noteOn(0, note, 60);
    while (triggered(distance)) {
      delay(10);
      distance = sonar.ping_cm();
    }
    noteOff(0, note, 60);
  }
}



