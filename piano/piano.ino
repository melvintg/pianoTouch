int DOpin = 2;
int REpin = 3;
int MIpin = 4;
int FApin = 5;
int SOLpin = 6;
int LApin = 7;
int SIpin = 8;
int DO2pin = 9;
int RE2pin = 10;
int SPEAKPin = 11;

// This is how high the sensor needs to read in order
//  to trigger a touch.  You'll find this number
//  by trial and error, or you could take readings at 
//  the start of the program to dynamically calculate this.
int touchedCutoff = 60;

void setup(){
  Serial.begin(9600);
  pinMode(DOpin, OUTPUT);
  pinMode(REpin, OUTPUT);
  pinMode(MIpin, OUTPUT);
  pinMode(FApin, OUTPUT);
  pinMode(SOLpin, OUTPUT);
  pinMode(LApin, OUTPUT);
  pinMode(SIpin, OUTPUT);
  pinMode(DO2pin, OUTPUT);
  pinMode(RE2pin, OUTPUT);
  pinMode(SPEAKPin, OUTPUT);
}

void loop(){
  // If the capacitive sensor reads above a certain threshold,
  //  turn on the LED
  //DO
  if (readCapacitivePin(DOpin) > touchedCutoff && readCapacitivePin(REpin) < touchedCutoff) {
    tone(SPEAKPin, 261.63); 
  }
  //DO#
  else if(readCapacitivePin(DOpin) > touchedCutoff && readCapacitivePin(REpin) > touchedCutoff) {
    tone(SPEAKPin, 277.18);
  }
  //RE
  else if(readCapacitivePin(REpin) > touchedCutoff && readCapacitivePin(DOpin) < touchedCutoff && readCapacitivePin(MIpin) < touchedCutoff) {
    tone(SPEAKPin, 293.66);
  }
  //RE#
  else if(readCapacitivePin(REpin) > touchedCutoff && readCapacitivePin(MIpin) > touchedCutoff) {
    tone(SPEAKPin, 311.13);
  }
  //MI
  else if(readCapacitivePin(MIpin) > touchedCutoff && readCapacitivePin(REpin) < touchedCutoff && readCapacitivePin(FApin) < touchedCutoff) {
    tone(SPEAKPin, 329.63);
  }
  //FA
  else if(readCapacitivePin(FApin) > touchedCutoff && readCapacitivePin(SOLpin) < touchedCutoff && readCapacitivePin(MIpin) < touchedCutoff) {
    tone(SPEAKPin, 349.23);
  }
  //FA#
  else if(readCapacitivePin(FApin) > touchedCutoff && readCapacitivePin(SOLpin) > touchedCutoff) {
    tone(SPEAKPin, 369.99);
  }
  //SOL
  else if(readCapacitivePin(SOLpin) > touchedCutoff && readCapacitivePin(FApin) < touchedCutoff && readCapacitivePin(LApin) < touchedCutoff) {
    tone(SPEAKPin, 392);
  }
  //SOL#
   else if(readCapacitivePin(SOLpin) > touchedCutoff && readCapacitivePin(LApin) > touchedCutoff) {
    tone(SPEAKPin, 415.3);
  }  
  //LA
  else if(readCapacitivePin(LApin) > touchedCutoff && readCapacitivePin(SOLpin) < touchedCutoff && readCapacitivePin(SIpin) < touchedCutoff) {
    tone(SPEAKPin, 440);
  }
  //LA#
  else if(readCapacitivePin(LApin) > touchedCutoff && readCapacitivePin(SIpin) > touchedCutoff) {
    tone(SPEAKPin, 466.16);
  }
  //SI
  else if(readCapacitivePin(SIpin) > touchedCutoff && readCapacitivePin(LApin) < touchedCutoff && readCapacitivePin(DO2pin) < touchedCutoff) {
    tone(SPEAKPin, 493.88);
  }
  //DO'
  else if(readCapacitivePin(DO2pin) > touchedCutoff && readCapacitivePin(SIpin) < touchedCutoff && readCapacitivePin(RE2pin) < touchedCutoff) {
    tone(SPEAKPin, 523.25);
  }
  //DO'#
  else if(readCapacitivePin(RE2pin) > touchedCutoff && readCapacitivePin(DO2pin) > touchedCutoff) {
    tone(SPEAKPin, 554.37);
  }
  //RE'
  else if(readCapacitivePin(RE2pin) > touchedCutoff && readCapacitivePin(DO2pin) < touchedCutoff) {
    tone(SPEAKPin, 587.33);
  }
  else {
    noTone(SPEAKPin);
  }

  
  // Every 500 ms, print the value of the capacitive sensor
  
  if ( (millis() % 500) == 0){
    Serial.print("Capacitive Sensor on Pin 2 reads: ");
    Serial.println(readCapacitivePin(DOpin));
  }
  
  
}

// readCapacitivePin
//  Input: Arduino pin number
//  Output: A number, from 0 to 17 expressing
//          how much capacitance is on the pin
//  When you touch the pin, or whatever you have
//  attached to it, the number will get higher
//  In order for this to work now,
// The pin should have a 1+Megaohm resistor pulling
//  it up to +5v.
uint8_t readCapacitivePin(int pinToMeasure){
  // This is how you declare a variable which
  //  will hold the PORT, PIN, and DDR registers
  //  on an AVR
  volatile uint8_t* port;
  volatile uint8_t* ddr;
  volatile uint8_t* pin;
  // Here we translate the input pin number from
  //  Arduino pin number to the AVR PORT, PIN, DDR,
  //  and which bit of those registers we care about.
  byte bitmask;
  if ((pinToMeasure >= 0) && (pinToMeasure <= 7)){
    port = &PORTD;
    ddr = &DDRD;
    bitmask = 1 << pinToMeasure;
    pin = &PIND;
  }
  if ((pinToMeasure > 7) && (pinToMeasure <= 13)){
    port = &PORTB;
    ddr = &DDRB;
    bitmask = 1 << (pinToMeasure - 8);
    pin = &PINB;
  }
  if ((pinToMeasure > 13) && (pinToMeasure <= 19)){
    port = &PORTC;
    ddr = &DDRC;
    bitmask = 1 << (pinToMeasure - 13);
    pin = &PINC;
  }
  // Discharge the pin first by setting it low and output
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  delay(1);
  // Make the pin an input WITHOUT the internal pull-up on
  *ddr &= ~(bitmask);
  // Now see how long the pin to get pulled up
  int cycles = 16000;
  for(int i = 0; i < cycles; i++){
    if (*pin & bitmask){
      cycles = i;
      break;
    }
  }
  // Discharge the pin again by setting it low and output
  //  It's important to leave the pins low if you want to 
  //  be able to touch more than 1 sensor at a time - if
  //  the sensor is left pulled high, when you touch
  //  two sensors, your body will transfer the charge between
  //  sensors.
  *port &= ~(bitmask);
  *ddr  |= bitmask;
  
  return cycles;
}
