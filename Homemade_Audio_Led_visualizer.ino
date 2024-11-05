



/**
 * 
 *  This is one of my earliest pieces of code ever! It creates a sound reactive effect
 *  for a set of addressable LEDs.    -Manny Batt
 * 
 */




// ***************************************
// ********** Global Variables ***********
// ***************************************


//FastLED
#include <FastLED.h>
#define ledPin D6
const int numLeds = 144;
CRGB leds[numLeds];

//Variables
#define soundPin A0

int lastValue = 0;
int effectCounter = 0;
int r = 0;
int g = 0;
int b = 20;
int effectChangeVar = 0;
int howManyLedsGetLit = 0;
int lastSpotL = 0;
int lastSpotR = 0;




// ***************************************
// *************** Setup *****************
// ***************************************


void setup() {
  Serial.begin(115200); //initialize serial
  FastLED.addLeds<NEOPIXEL, ledPin>(leds, numLeds);
  lastValue = analogRead(soundPin) * 2;
  randomSeed(lastValue);
  effectChangeVar = random(30, 50);
  lastSpotL = 72;
  lastSpotR = 73;
}




// ***************************************
// ************* Da Loop *****************
// ***************************************


void loop() {

  int value = analogRead(soundPin) * 1.5; //read the value of A0
  int threshhold = abs(lastValue - value);

  if (value == 0 || (threshhold < 4)) {
    return;
  }
  else {
    howManyLedsGetLit = value / 10; //Lower Peaks above LED Limits

    if (howManyLedsGetLit > 72) {
      howManyLedsGetLit = 72;
    }

    if (effectCounter > effectChangeVar) {
      r = random(0, 100);
      g = random(0, 100);
      b = random(0, 100);
      effectChangeVar = random(30, 50);
      effectCounter = 0;
    }

    if (lastValue == 0 && howManyLedsGetLit < 5) {
      return;
    }

    else if (howManyLedsGetLit == lastValue) {
      return;
    }

    else if (lastValue < howManyLedsGetLit) { //if theres a new bigger value [SLOW]
      int everyOther = 0;
      while (lastSpotR < (73 + howManyLedsGetLit)) {
        //Serial.println("  Up");
        leds[lastSpotL] = CRGB(r, g, b);
        leds[lastSpotR] = CRGB(r, g, b);
        lastSpotL--;
        lastSpotR++;
        if (everyOther == 2) {
          FastLED.show();
          everyOther = 0;
        }
        else {
          everyOther++;
        }
        FastLED.show();
      }
      FastLED.show();
      effectCounter++;
    }

    //if theres a new Smaller Value [SLOW]
    else if (lastValue > howManyLedsGetLit) {
      int everyOther = 0;
      while (lastSpotR > (73 + howManyLedsGetLit)) {
        //Serial.println("  Down");
        leds[lastSpotL] = CRGB(0, 0, 0);
        leds[lastSpotR] = CRGB(0, 0, 0);
        lastSpotL++;
        lastSpotR--;
        if (everyOther == 2) {
          FastLED.show();
          everyOther = 0;
        }
        else {
          everyOther++;
        }
        FastLED.show();
      }
      FastLED.show();
    }
    Serial.println(howManyLedsGetLit);
    lastValue = howManyLedsGetLit;
    delay(40);
  }
}
