// AUTHOR Eduardo Ortega
// White - snowfall

#include <FastLED.h>
#define NUM_LEDS 60
#define FADE_LOWER_BOUND 60
#define FADE_UPPER_BOUND 100
#define FADE_DELAY_LOWER_BOUND 90
#define FADE_DELAY_UPPER_BOUND 120

CRGBArray<NUM_LEDS> leds;

void setup() { 
  FastLED.addLeds<WS2812B,3, GRB>(leds, NUM_LEDS);
}

void loop(){ 
  //initial_prob_fade, and prob_delay_speed
  long prob_fade = random(FADE_LOWER_BOUND, FADE_UPPER_BOUND);
  long prob_delay_speed = random(FADE_DELAY_LOWER_BOUND, FADE_DELAY_UPPER_BOUND);
  while(1) {
    long snowfall_chance = random(1, 1000);
    long prob_value = 300;
    // establish prob for strike not occuring
    if (snowfall_chance > prob_value) {
      leds.fadeToBlackBy(prob_fade);
      FastLED.delay(prob_delay_speed);
    } else {
      long prob_fade = random(FADE_LOWER_BOUND, FADE_UPPER_BOUND);
      long prob_delay_speed = random(FADE_DELAY_LOWER_BOUND, FADE_DELAY_UPPER_BOUND);
      long prob_brightness = random(70, 150);
      long snow_fall_start = random(0, NUM_LEDS-10);
      long snow_fall_end = random(snow_fall_start, NUM_LEDS-2);
      for(int i = snow_fall_start; i < snow_fall_end; i++) {   
        leds.fadeToBlackBy(prob_fade);
        leds[i] = CHSV(100, 0, prob_brightness);
        FastLED.delay(prob_delay_speed);
      }
    }
  }
}

