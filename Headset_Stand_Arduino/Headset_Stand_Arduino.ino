// Libraries //

#include <FastLED.h>
#include <GParser.h>

// Variables //

#define DATA_PIN    2
#define LED_TYPE    WS2812B // Enter the type of your LED strip
#define COLOR_ORDER GRB // Order of color Green Red Blue
#define NUM_LEDS    8 // Enter amount of LEDs on your strip

float BRIGHTNESS = 100; // Default Brigntness
float FRAMES_PER_SECOND = 120;
int serialSpeed = 9600;
uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current
uint8_t gHue = 0; // rotating "base color" used by many of the patterns
int hue_speed = 100;
int loop_speed = 10;
int loop_patterns = 0;
CRGB leds[NUM_LEDS];

void setup() {
  //  delay(3000);
  Serial.begin(serialSpeed);
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setMaxPowerInVoltsAndMilliamps(5, 500);
  FastLED.setBrightness(BRIGHTNESS);
}

typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = { rainbow, confetti, sinelon, juggle, bpm, colorScroll };

/*
   PC - Arduino
   0: Pattern number (int)
   1: Brightness control (float)
   2: Speed number (int)
   3: FPS number (float)
   4: Loop_patterns (bool)
*/

/*
   Arduino - PC
   0: Brightness
*/
void loop() {

  gPatterns[gCurrentPatternNumber]();
  if (Serial.available()) {
    char buf[50];
    int num = Serial.readBytesUntil(';', buf, 50);
    buf[num] = NULL;
    GParser data(buf, ',');
    int ints[10];
    data.parseInts(ints);
    switch (ints[0]) {
      case 0:  gCurrentPatternNumber = int(ints[1]);
        break;
      case 1:
        BRIGHTNESS = float(ints[1]);
        FastLED.setBrightness(BRIGHTNESS);
        break;
      case 2:
        hue_speed = int(ints[1]);
        break;
      case 3:
        FRAMES_PER_SECOND = int(ints[1]);
        break;
      case 4:
        loop_patterns = int(ints[1]);
        break;
      case 5:
        loop_speed = int(ints[1]);
        break;
    }
  }
  static uint32_t tmr_speed = 0;
  if (millis() - tmr_speed > hue_speed) {
    tmr_speed = millis();
    gHue++;
  }

  static uint32_t tmr = 0;
  if (millis() - tmr > 100) {
    tmr = millis();
    Serial.print(0);
    Serial.print(',');
    Serial.println(BRIGHTNESS);
    Serial.print(1);
    Serial.print(',');
    Serial.println(hue_speed);
    Serial.print(2);
    Serial.print(',');
    Serial.println(FRAMES_PER_SECOND);
    Serial.print(3);
    Serial.print(',');
    Serial.println(loop_patterns);
    Serial.print(4);
    Serial.print(',');
    Serial.println(loop_speed);
  }
  if (loop_patterns == 1) {
    static uint32_t tmr_loop_speed = 0;
    if (millis() - tmr_loop_speed > loop_speed  * 1000) {
      tmr_loop_speed = millis();
      nextPattern();
    }
  }
  FastLED.show();
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}



#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

void nextPattern()
{
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

/*
   Patterns
*/



void rainbow()
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 7);
}

void rainbowWithGlitter()
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}

void addGlitter( fract8 chanceOfGlitter)
{
  if ( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::White;
  }
}

void confetti()
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 200, 255);
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 20);
  int pos = beatsin16( 13, 0, NUM_LEDS - 1 );
  leds[pos] += CHSV( gHue, 255, 192);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = PartyColors_p;
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255);
  for ( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue + (i * 2), beat - gHue + (i * 10));
  }
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 20);
  uint8_t dothue = 0;
  for ( int i = 0; i < 8; i++) {
    leds[beatsin16( i + 7, 0, NUM_LEDS - 1 )] |= CHSV(dothue, 200, 255);
    dothue += 32;
  }
}
void colorScroll() {
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CHSV(gHue + (i * 10), 255, 255);
  }
}
