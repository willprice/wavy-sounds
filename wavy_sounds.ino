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


// Check the VS1053 datasheet for instrument IDs
// 97 is pretty great
int instrument = 97;
// Each note has a numeric ID that can be found at
// http://www.phys.unsw.edu.au/jw/notes.html
int scale[] = {36, 38, 39, 
               41, 43, 45, 
               47, 48, 50, 
               51, 60, 62, 
               63, 65, 67, 
               69, 71, 72, 
               74, 75};


int scale_index = 0;
const int number_of_notes_in_scale = 20;

void setup() {
  midiSerial.begin(31250);
  resetMIDI();
  initMIDI();
}

void loop() {
  int distance = sonar.ping_cm();
  if (triggered(distance)) {
    noteOn(0, scale[scale_index], 60);
    while (triggered(distance)) {
      delay(10);
      distance = sonar.ping_cm();
    }
    noteOff(0, scale[scale_index], 60);
    updateScaleIndex();
  }
}

void updateScaleIndex() {
    scale_index = random(0, number_of_notes_in_scale);
}
