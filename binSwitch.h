#include "esphome.h"

//  global variable to store state of bits
uint64_t output_state = 0;
// pins of the Output shift register (74HC595)
const byte outputLatch_pin = 18;
const byte clock_pin = 17;
const byte mosi_pin = 16;

class binSwitch : public Component, public Switch
{
 byte pinNum;
 bool SetBit(bool pinValue, uint64_t number)
 {
   if (pinValue)
     bitSet(output_state, number);
   else
     bitClear(output_state, number);
   SetOutputs();
   return pinValue;
 }
 void SetOutputs()
 {
   digitalWrite(clock_pin, LOW);
   digitalWrite(outputLatch_pin, LOW);
   shiftOut(mosi_pin, clock_pin, LSBFIRST, output_state);
   shiftOut(mosi_pin, clock_pin, LSBFIRST, output_state >> 8);
   shiftOut(mosi_pin, clock_pin, LSBFIRST, output_state >> 16);
   shiftOut(mosi_pin, clock_pin, LSBFIRST, output_state >> 24);
   digitalWrite(outputLatch_pin, HIGH);
 }
public:
 // pin - output pin of the shift register (0..31)
 binSwitch(byte pin) { pinNum = pin; publish_state(false);}
 void setup() override
 {
   publish_state(false);
   pinMode(outputLatch_pin, OUTPUT);
   pinMode(clock_pin, OUTPUT);
   pinMode(mosi_pin, OUTPUT);
   digitalWrite(outputLatch_pin, HIGH);
   SetOutputs();
 }
 void write_state(bool state) override
 {
   publish_state(SetBit(state, pinNum));
 }
};