/* MIDI information
   - channels range from 0-15
 */
 

typedef enum {
  CHANNEL_MESSAGE = 0xB0,
  SET_INSTRUMENT  = 0xC0
} midiCommands;


void resetMIDI() {
  pinMode(PIN_RESET_MIDI, OUTPUT);
  digitalWrite(PIN_RESET_MIDI, LOW);
  delay(100);
  digitalWrite(PIN_RESET_MIDI, HIGH);
  delay(100);
  talkMIDI(CHANNEL_MESSAGE, 0x07, 120); //0xB0 is channel message, set channel volume to near max (127)
}

void initMIDI() {
  talkMIDI(CHANNEL_MESSAGE, 0, 0x00); //Default bank GM1
  talkMIDI(SET_INSTRUMENT, instrument, 0); //Set instrument number. 0xC0 is a 1 data byte command
}

void playNote(int note) {
  int note_velocity = 60;
  noteOn(0, note, note_velocity);
  delay(NOTE_DURATION);
  noteOff(0, note, note_velocity);
}

void noteOn(byte channel, byte note, byte attack_velocity) {
  //Like pressing a piano key
  talkMIDI( (0x90 | channel), note, attack_velocity);
}

//Send a MIDI note-off message.  Like releasing a piano key
void noteOff(byte channel, byte note, byte release_velocity) {
  talkMIDI( (0x80 | channel), note, release_velocity);
}

//Plays a MIDI note. Doesn't check to see that cmd is greater than 127, or that data values are less than 127
void talkMIDI(byte cmd, byte data1, byte data2) {
  digitalWrite(PIN_ACTIVITY_LED, HIGH);
  midiSerial.write(cmd);
  midiSerial.write(data1);

  //Some commands only have one data byte. All cmds less than 0xBn have 2 data bytes 
  //(sort of: http://253.ccarh.org/handout/midiprotocol/)
  if( (cmd & 0xF0) <= 0xB0)
    midiSerial.write(data2);

  digitalWrite(PIN_ACTIVITY_LED, LOW);
}
