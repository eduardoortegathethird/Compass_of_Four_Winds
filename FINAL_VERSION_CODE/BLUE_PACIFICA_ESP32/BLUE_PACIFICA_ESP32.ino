// Eduardo Ortega
#include <FastLED_NeoPixel.h>
#define LED_PIN_2           2
#define LED_PIN_4           4
#define LED_PIN_5           5
#define NUM_LEDS            195
#define LED_TYPE            WS2812B
#define BRIGHTNESS          100
CRGB leds[NUM_LEDS];
FastLED_NeoPixel<NUM_LEDS, LED_PIN_2, NEO_GRB> leds_2;
FastLED_NeoPixel<NUM_LEDS, LED_PIN_4, NEO_GRB> leds_4;
FastLED_NeoPixel<NUM_LEDS, LED_PIN_5, NEO_GRB> leds_5;

CRGBPalette16 pacifica_palette_1 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x14554B, 0x28AA50 };

CRGBPalette16 pacifica_palette_2 = 
    { 0x000507, 0x000409, 0x00030B, 0x00030D, 0x000210, 0x000212, 0x000114, 0x000117, 
      0x000019, 0x00001C, 0x000026, 0x000031, 0x00003B, 0x000046, 0x0C5F52, 0x19BE5F };
CRGBPalette16 pacifica_palette_3 = 
    { 0x000208, 0x00030E, 0x000514, 0x00061A, 0x000820, 0x000927, 0x000B2D, 0x000C33, 
      0x000E39, 0x001040, 0x001450, 0x001860, 0x001C70, 0x002080, 0x1040BF, 0x2060FF };

void pacifica_loop() {
  static uint16_t sCIStart1, sCIStart2, sCIStart3, sCIStart4;
  static uint32_t sLastms = 0;
  uint32_t ms = GET_MILLIS();
  uint32_t deltams = ms - sLastms;
  sLastms = ms;
  uint16_t speedfactor1 = beatsin16(3, 179, 269);
  uint16_t speedfactor2 = beatsin16(4, 179, 269);
  uint32_t deltams1 = (deltams * speedfactor1) / 256;
  uint32_t deltams2 = (deltams * speedfactor2) / 256;
  uint32_t deltams21 = (deltams1 + deltams2) / 2;
  sCIStart1 += (deltams1 * beatsin88(1011,10,13));
  sCIStart2 -= (deltams21 * beatsin88(777,8,11));
  sCIStart3 -= (deltams1 * beatsin88(501,5,7));
  sCIStart4 -= (deltams2 * beatsin88(257,4,6));

  CRGB c = CRGB(0, 6, 10);
  uint32_t color = leds_2.Color(c.blue, c.green, c.red);
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds_2.setPixelColor(i, color);
    leds_4.setPixelColor(i, color);
    leds_5.setPixelColor(i, color);
  }

  pacifica_one_layer( pacifica_palette_1, sCIStart1, beatsin16( 3, 11 * 256, 14 * 256), beatsin8( 10, 70, 130), 0-beat16( 301) );
  pacifica_one_layer( pacifica_palette_2, sCIStart2, beatsin16( 4,  6 * 256,  9 * 256), beatsin8( 17, 40,  80), beat16( 401) );
  pacifica_one_layer( pacifica_palette_3, sCIStart3, 6 * 256, beatsin8( 9, 10,38), 0-beat16(503));
  pacifica_one_layer( pacifica_palette_3, sCIStart4, 5 * 256, beatsin8( 8, 10,28), beat16(601));

  pacifica_deepen_colors();
}

void pacifica_one_layer( CRGBPalette16& p, uint16_t cistart, uint16_t wavescale, uint8_t bri, uint16_t ioff) {
  uint16_t ci = cistart;
  uint16_t waveangle = ioff;
  uint16_t wavescale_half = (wavescale / 2) + 20;
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    waveangle += 250;
    uint16_t s16 = sin16( waveangle ) + 32768;
    uint16_t cs = scale16( s16 , wavescale_half ) + wavescale_half;
    ci += cs;
    uint16_t sindex16 = sin16( ci) + 32768;
    uint8_t sindex8 = scale16( sindex16, 240);
    CRGB c = ColorFromPalette( p, sindex8, bri, LINEARBLEND);
    leds[i] += c;
    uint32_t color = leds_2.Color(leds[i].blue, leds[i].green, leds[i].red);
    leds_2.setPixelColor(i, color);
    leds_4.setPixelColor(i, color);
    leds_5.setPixelColor(i, color);
  }
}

void pacifica_deepen_colors() {
  for( uint16_t i = 0; i < NUM_LEDS; i++) {
    leds[i].blue = scale8( leds[i].blue,  145); 
    leds[i].green= scale8( leds[i].green, 200); 
    leds[i] |= CRGB( 2, 5, 7);
    uint32_t color = leds_2.Color(leds[i].blue, leds[i].green, leds[i].red);
    leds_2.setPixelColor(i, color);
    leds_4.setPixelColor(i, color);
    leds_5.setPixelColor(i, color);
  }
}

////////////////////////////////////////////////////////////////

void setup() {
  delay(3000);
  leds_2.begin();
  leds_2.setBrightness( BRIGHTNESS );
  leds_4.begin();
  leds_4.setBrightness( BRIGHTNESS );
  leds_5.begin();
  leds_5.setBrightness( BRIGHTNESS );
}

void blue_animation() {
  EVERY_N_MILLISECONDS( 10 ) {
    pacifica_loop();
    leds_2.show();
    leds_4.show();
    leds_5.show();
  }
}

void loop() {
  blue_animation();
}
