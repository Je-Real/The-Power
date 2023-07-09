#include <MIDI.h>
#include <Keypad.h>

// This is required to set up the MIDI library.
// The default MIDI setup uses the Arduino built-in serial port
// which is pin 1 for transmitting on the Arduino Uno.
MIDI_CREATE_DEFAULT_INSTANCE();

// Set up the MIDI channel to send on
#define MIDI_CHANNEL 1

#define MIDI_LED LED_BUILTIN

const byte ROWS = 5;
const byte COLS = 8;
char keys[ROWS][COLS] = {
  {36,37,38,39,40,41,42,43},
  {44,45,46,47,48,49,50,51},
  {52,53,54,55,56,57,58,59},
  {60,61,62,63,64,65,66,67},
  {68,69,70,71,72,73,74,75}
};
byte rowPins[ROWS] = {2,3,4,5,6}; //connect to the row pinouts of the kpd
byte colPins[COLS] = {A3,A2,A1,A0,15,14,16,10}; //connect to the column pinouts of the kpd

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  MIDI.begin(MIDI_CHANNEL_OFF);
  Serial.begin(9600);
  pinMode(MIDI_LED, OUTPUT);
}


void loop() {
    // Fills kpd.key[ ] array with up-to 10 active keys.
    // Returns true if there are ANY active keys.
    if (kpd.getKeys())
    {
        for (int i=0; i<LIST_MAX; i++)   // Scan the whole key list.
        {
            if ( kpd.key[i].stateChanged )   // Only find keys that have changed state.
            {
                switch (kpd.key[i].kstate) {
                    case PRESSED:
                    digitalWrite(MIDI_LED, HIGH);
                    //Serial.print(kpd.key[i].kchar,DEC);
                    //Serial.println(" Pressed");
                    MIDI.sendNoteOn(kpd.key[i].kchar, 127, MIDI_CHANNEL);
                break;
                    case HOLD:
                break;
                    case RELEASED:
                    MIDI.sendNoteOff(kpd.key[i].kchar, 0, MIDI_CHANNEL);
                    digitalWrite(MIDI_LED, LOW);
                break;
                    case IDLE:
                break;
                }
            }
        }
    }
}  // End loop
