// works with keyboards with matrix 7x8
#define matrix1 5
#define matrix2 8

#include "MIDIUSB.h"

int octave = 0; // add or subtract 8 for one octave
bool currentState[matrix1 * matrix2];
bool requestState[matrix1 * matrix2];
void setup() 
{
  //-----matrix1 number of pins-------
  pinMode(A0,INPUT); //1
  pinMode(A1,INPUT); //2
  pinMode(A2,INPUT); //3
  pinMode(A3,INPUT); //4
  pinMode(15,INPUT); //5

  //-----matrix2 number of pins-------
  //each matrix2 pin must be one above the previous one
  pinMode(2,INPUT); //1
  pinMode(3,INPUT); //2
  pinMode(4,INPUT); //3
  pinMode(5,INPUT); //4
  pinMode(6,INPUT); //5
  pinMode(7,INPUT); //6
  pinMode(8,INPUT); //7
  pinMode(9,INPUT); //8

  
}

void loop() 
{
  readKeys();
  writeKeys();
  delay(1);
}

void readKeys()
{
  for(int i=0; i<matrix2; i++)
  {
    //---------matrix2 pins-----------
    pinMode(2,INPUT);
    pinMode(3,INPUT);
    pinMode(4,INPUT);
    pinMode(5,INPUT);
    pinMode(6,INPUT);
    pinMode(7,INPUT);
    pinMode(8,INPUT);
    pinMode(9,INPUT);
    pinMode(i+2,OUTPUT); // <--i+2 (this is your first pin number)
    
    digitalWrite(i+2,LOW);
    delayMicroseconds(1000);

    //you have to repeat this finction matrix1 times
    requestState[i*matrix1+0] = !digitalRead(A3);
    requestState[i*matrix1+1] = !digitalRead(A2);
    requestState[i*matrix1+2] = !digitalRead(A1);
    requestState[i*matrix1+3] = !digitalRead(A0);
    requestState[i*matrix1+4] = !digitalRead(15);
    
  }
}


void writeKeys()
{
  for(int i=0; i<matrix1 * matrix2; i++)
  {
    if(requestState[i]==true && currentState[i] == false)
    {
      noteOn(0, 36+octave+(i*matrix2)%matrix1 * matrix2+i/matrix1, 64);
      currentState[i] = requestState[i];
    }
    
    if(requestState[i]==false && currentState[i] == true)
    {
      noteOff(0, 36+octave+(i*matrix2)%matrix1 * matrix2+i/matrix1, 64);
      currentState[i] = requestState[i];
    }
  }
  MidiUSB.flush();
}


void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}
