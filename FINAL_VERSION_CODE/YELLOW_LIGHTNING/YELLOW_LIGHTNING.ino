// AUTHOR Eduardo Ortega
// Yellow - lightning

#include <FastLED.h>
#define NUM_LEDS 60

CRGBArray<NUM_LEDS> leds;

void setup() { 
  FastLED.addLeds<WS2812B,3, GRB>(leds, NUM_LEDS);
}

void loop(){ 
  //initial_prob_fade, and prob_delay_speed
  long prob_fade = random(40, 140);
  long prob_delay_speed = random(1, 5);
  while(1) {
    long lightning_strike_chance = random(1, 1000);
    long prob_value = 0002;
    // establish prob for strike not occuring
    if (lightning_strike_chance > prob_value) {
      leds.fadeToBlackBy(prob_fade);
      FastLED.delay(prob_delay_speed);
    } else {
      long prob_fade = random(40, 140);
      long prob_delay_speed = random(1, 5);
      long prob_saturation = random(225, 255);
      long prob_brightness = random(1, 100);
      for(int i = 0; i < NUM_LEDS; i++) {   
        leds.fadeToBlackBy(prob_fade);
        leds[i] = CHSV(64, prob_saturation, prob_brightness);
        FastLED.delay(prob_delay_speed);
      }
    }
  }
}

