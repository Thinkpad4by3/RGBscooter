// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET
#include <EEPROM.h>

#define NUMPIXELS 28 // Number of LEDs in strip
#define NUMPIXELS2 23 // Number of LEDs in strip
// Here's how to control the LEDs from any two pins:
#define DATAPIN    4
#define CLOCKPIN   5
#define DATAPIN2    6
#define CLOCKPIN2   7

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BGR);
Adafruit_DotStar strip2(NUMPIXELS2, DATAPIN2, CLOCKPIN2, DOTSTAR_BGR);

#define INTERRUPTPIN   2

// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BRG);

int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0xFF0000;      // 'On' color (starts red)
int hue = 0;
int orghue = 0;
int brightness = 0;
int prevRead = 0;
volatile int programSelection;

int index = 0;


void setup() {
  pinMode(INTERRUPTPIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPTPIN), changeProgram, RISING);


#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  strip2.begin(); // Initialize pins for output
  strip2.show();  // Turn all LEDs off ASAP
  programSelection = EEPROM.read(1);
}

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.


void loop() {
  brightness = (analogRead(A1))/(4);
  if(abs(brightness-prevRead) < 4) {
    brightness = prevRead;
  } else {
    prevRead = brightness;
  }
  switch(programSelection) {
    case 0: redWhiteBlue(); break;
    case 1: red(); break;
    case 2: blueYellow(); break; 
    case 3: orangeWhiteGreen(); break;
    case 4: linasTerribleSuggestion(); break;
  }
  delay(20);
  redStrip2();
  //orangeWhiteGreen();
  //color = strip.ColorHSV(orghue, 255, brightness);
  //orghue = orghue + 50;
  //if(orghue > 65520) {
  //  orghue = 0;
  //}
  //hue = orghue;
  //for(int i = 0; i < NUMPIXELS; i++) {
  //  hue = hue + 100;
  //  color = strip.ColorHSV(hue, 255, brightness);
  //  strip.setPixelColor(i, color); // 'On' pixel at head
  //  strip2.setPixelColor(i, color); // 'On' pixel at head
  //}
  //strip.show();                     // Refresh strip
}

void changeProgram() {
  programSelection++;
  if(programSelection > 4) {
    programSelection = 0;
  }
  EEPROM.write(1,programSelection);
}

void redWhiteBlue() {
  color = strip.ColorHSV(0,255, brightness);
  int flag = 0;
  int counter = 0;
   for(int i = 0; i < NUMPIXELS; i++) {
    counter++;
    if(counter > 3) {
      flag++;
      if(flag > 2) {
        flag = 0;
      }
      counter = 0;
    }
    switch(flag) {
      case 0: color = strip.ColorHSV(0,255, brightness); break;
      case 1: color = strip.ColorHSV(43690,255, brightness); break;
      case 2: color = strip.ColorHSV(10922,0, brightness); break;
    }
    //color = strip.ColorHSV(hue, 255, brightness);
    strip.setPixelColor(i, color); // 'On' pixel at head
    //strip2.setPixelColor(i, color); // 'On' pixel at head
  }
  strip.show();
}

void red() {
  color = strip.ColorHSV(0,255, brightness);
   for(int i = 0; i < NUMPIXELS; i++) {
    strip.setPixelColor(i, color); // 'On' pixel at head
    //strip2.setPixelColor(i, color); // 'On' pixel at head
  }
  strip.show();
}

void redStrip2() {
  color = strip.ColorHSV(0,255, brightness);
   for(int i = 0; i < NUMPIXELS; i++) {
    strip2.setPixelColor(i, color); // 'On' pixel at head
    //strip2.setPixelColor(i, color); // 'On' pixel at head
  }
  strip2.show();
}

void blueYellow() {
  color = strip.ColorHSV(0,255, brightness);
  int flag = 0;
  int counter = 0;
   for(int i = 0; i < NUMPIXELS; i++) {
    counter++;
    if(counter > 3) {
      flag++;
      if(flag > 1) {
        flag = 0;
      }
      counter = 0;
    }
    switch(flag) {
      case 0: color = strip.ColorHSV(10922,255, brightness); break;
      case 1: color = strip.ColorHSV(43690,255, brightness); break;
    }
    //color = strip.ColorHSV(hue, 255, brightness);
    strip.setPixelColor(i, color); // 'On' pixel at head
    //strip2.setPixelColor(i, color); // 'On' pixel at head
  }
  strip.show();
}

void orangeWhiteGreen() {
  color = strip.ColorHSV(0,255, brightness);
  int flag = 0;
  int counter = 0;
   for(int i = 0; i < NUMPIXELS; i++) {
    counter++;
    if(counter > 3) {
      flag++;
      if(flag > 2) {
        flag = 0;
      }
      counter = 0;
    }
    switch(flag) {
      case 0: color = strip.ColorHSV(5000,255, brightness); break;
      case 1: color = strip.ColorHSV(43690,0, brightness); break;
      case 2: color = strip.ColorHSV(21500,255, brightness); break;
    }
    //color = strip.ColorHSV(hue, 255, brightness);
    strip.setPixelColor(i, color); // 'On' pixel at head
    //strip2.setPixelColor(i, color); // 'On' pixel at head
  }
  strip.show();
}

void linasTerribleSuggestion() {
  color = strip.ColorHSV(0,255, brightness);
  int flag = 0;
  int counter = 0;
   for(int i = 0; i < NUMPIXELS; i++) {
    counter++;
    if(counter > 3) {
      flag++;
      if(flag > 1) {
        flag = 0;
      }
      counter = 0;
    }
    switch(flag) {
      case 0: color = strip.ColorHSV(43690,0, brightness); break;
      case 1: color = strip.ColorHSV(5000,255, brightness); break;
    }
    //color = strip.ColorHSV(hue, 255, brightness);
    strip.setPixelColor(i, color); // 'On' pixel at head
    //strip2.setPixelColor(i, color); // 'On' pixel at head
  }
  strip.show();
}

