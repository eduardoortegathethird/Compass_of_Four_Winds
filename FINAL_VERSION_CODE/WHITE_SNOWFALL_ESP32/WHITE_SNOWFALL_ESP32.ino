// AUTHOR Eduardo Ortega
// White - snowfall

#include <FastLED_NeoPixel.h>
#define LED_PIN_2               2
#define LED_PIN_4               4
#define LED_PIN_5               5
#define LED_TYPE                WS2812B
#define NUM_LED_PER_STRIP       19
#define NUM_STRIP               10
#define NUM_LEDS                190
#define BRIGHTNESS              100
#define FADE_LOWER_BOUND        30
#define FADE_UPPER_BOUND        50
#define FADE_DELAY_LOWER_BOUND  125
#define FADE_DELAY_UPPER_BOUND  175
CRGB leds[NUM_LED_PER_STRIP][NUM_STRIP];
FastLED_NeoPixel<NUM_LEDS, LED_PIN_2, NEO_GRB> leds_2;
FastLED_NeoPixel<NUM_LEDS, LED_PIN_4, NEO_GRB> leds_4;
FastLED_NeoPixel<NUM_LEDS, LED_PIN_5, NEO_GRB> leds_5;


void setup() { 
  delay(3000);
  leds_2.begin();
  leds_2.setBrightness( BRIGHTNESS );
  leds_4.begin();
  leds_4.setBrightness( BRIGHTNESS );  
  leds_5.begin();
  leds_5.setBrightness( BRIGHTNESS );
}

void assign_WHITE_LEDS(int index, int strip) {
  CRGB col_w = leds[index][strip];
  uint32_t color = leds_2.Color(col_w.red, col_w.green, col_w.blue);
  if( strip == 0 ) {
    leds_2.setPixelColor(NUM_LED_PER_STRIP-index-1, color);
    leds_4.setPixelColor(NUM_LED_PER_STRIP-index-1, color);
    leds_5.setPixelColor(NUM_LED_PER_STRIP-index-1, color);
  } else {
    leds_2.setPixelColor((strip*2*NUM_LED_PER_STRIP) + (NUM_LED_PER_STRIP-index), color);
    leds_2.setPixelColor((((strip*2)-1)*NUM_LED_PER_STRIP)+index, color);
    leds_4.setPixelColor((strip*2*NUM_LED_PER_STRIP) + (NUM_LED_PER_STRIP-index), color);
    leds_4.setPixelColor((((strip*2)-1)*NUM_LED_PER_STRIP)+index, color);
    leds_5.setPixelColor((strip*2*NUM_LED_PER_STRIP) + (NUM_LED_PER_STRIP-index), color);
    leds_5.setPixelColor((((strip*2)-1)*NUM_LED_PER_STRIP)+index, color);
  }
}

void fade_pixel(int i, int strip, long fade) {
  int red = int(leds[i][strip].red - fade);
  int blue = int(leds[i][strip].blue - fade);
  int green = int(leds[i][strip].green - fade);
  if(red < 0){ red = 0; }
  if(blue < 0){ blue = 0; }
  if(green < 0){ green = 0; }
  leds[i][strip] = CRGB(red, green, blue);
}

void fade_black(long fade){
  for( int strip=0; strip < NUM_STRIP; strip++) {
    for(int i=0; i < NUM_LED_PER_STRIP; i++){
        fade_pixel(i, strip, fade);
    }
  }
}

void white_update(){ 
  //snow fall chance
  for ( int j = 0; j < NUM_STRIP; j++) {
    long SPARKING = random(80, 100);
    if( random8() < SPARKING ) {
      int y = random8(3);
      leds[y][j] = CRGB(255, 255, 255);
    }
  }

  // increment snow fall
  for( int strip=0; strip < NUM_STRIP; strip++){
    CRGB led_tmp[NUM_LED_PER_STRIP];
    for( int pixel=0; pixel < NUM_LED_PER_STRIP-1; pixel++) {
      led_tmp[pixel] = leds[pixel-1][strip];
    }
    for( int pixel=1; pixel < NUM_LED_PER_STRIP-1; pixel++) {
      leds[pixel][strip] = led_tmp[pixel];
    }
  }

  // fade snow fall (all pixels)
  fade_black(20);

  for ( int strip = 0; strip < NUM_STRIP; strip++) {
    for( int cell_index = 0; cell_index < NUM_LED_PER_STRIP; cell_index++) {
      assign_WHITE_LEDS(cell_index, strip);
    }
  }
}

void WHITE_ANIMATION() {
  white_update();
  leds_2.show();
  leds_4.show();
  leds_5.show();
  delay(150);
}

void loop() {
  WHITE_ANIMATION();
}
