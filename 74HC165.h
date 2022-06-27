#include "esphome.h"

class SN74HC165Component : public PollingComponent, public BinarySensor 
{
  byte pinNum;
  private:
    const byte LATCH = 15;  // ESP pin number that should be connected to pin 1 of 74hc165s
    const byte DATA  = 14;  // ESP pin number that should be connected to pin 9 of first 74hc165
    const byte CLOCK = 12; // ESP pin number that should be connected to pin 2 of 74hc165s

  bool getBit(unsigned long x, byte n) 
  {
    bool value = bitRead(x, n);
    return value;
  }
  
  unsigned long shiftinput= 1;
  unsigned long oldshiftinput= 0;

 public:
  SN74HC165Component (byte pin) : PollingComponent(15000) { pinNum = pin; }   // Change the number inside "PollingComponent(100)" to change update interval in miliseconds 
  float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }

  void setup() override 
  {
    pinMode(LATCH, OUTPUT); 
    pinMode(CLOCK, OUTPUT); 
    pinMode(DATA, INPUT); 
    digitalWrite(CLOCK, LOW); 
    digitalWrite(CLOCK, HIGH); 
    digitalWrite(LATCH, HIGH);
  }

  void update() override 
  {
    digitalWrite(CLOCK, LOW); 
    digitalWrite(CLOCK, HIGH); 
    digitalWrite(LATCH, LOW); 
    digitalWrite(LATCH, HIGH); 

    shiftinput = shiftIn(DATA, CLOCK, LSBFIRST);
    shiftinput |= shiftIn(DATA, CLOCK, LSBFIRST) << 8;
    shiftinput |= shiftIn(DATA, CLOCK, LSBFIRST) << 16;
    shiftinput |= shiftIn(DATA, CLOCK, LSBFIRST) << 24;
    
    if (shiftinput != oldshiftinput) 
    {
      publish_state(getBit(shiftinput, pinNum));
      oldshiftinput = shiftinput;
    }  
  }
};