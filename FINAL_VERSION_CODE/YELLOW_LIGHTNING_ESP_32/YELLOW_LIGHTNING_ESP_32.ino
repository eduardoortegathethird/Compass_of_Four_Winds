// AUTHOR Eduardo Ortega
// Yellow - lightning

#include <FastLED_NeoPixel.h>
#define NUM_LEDS    195
#define LED_PIN_2   2
#define LED_PIN_4   4
#define LED_PIN_5   5
#define LED_TYPE    WS2812B
#define BRIGHTNESS  100
CRGB leds[NUM_LEDS];
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

void fade_black(long fade){
  float fade_factor = fade / 255.0;
  for(uint16_t i=0; i < NUM_LEDS; i++){
    int red = int(leds[i].red - fade);
    int blue = int(leds[i].blue - fade);
    int green = int(leds[i].green - fade);
    if(red < 0){ red = 0; }
    if(blue < 0){ blue = 0; }
    if(green < 0){ green = 0; }
    leds[i] = CRGB(red, green, blue);
    uint32_t color = leds_2.Color(leds[i].red, leds[i].green, leds[i].blue);
    //leds_2.setPixelColor(i, color);
    //leds_4.setPixelColor(i, color);
    //leds_5.setPixelColor(i, color);
  }
}

void yellow_flash_animation(){ 
  //initial_prob_fade, and prob_delay_speed
  long prob_fade = random(40, 140);
  long prob_delay_speed = random(1, 5);
  while(1) {
    long lightning_strike_chance = random(1, 1000);
    long prob_value = 50;
    // establish prob for strike not occuring
    if (lightning_strike_chance > prob_value) {
      fade_black(prob_fade);
      delay(prob_delay_speed);
    } else {
      long prob_fade = random(10, 40);
      long prob_delay_speed = random(1, 5);
      long prob_saturation = random(225, 255);
      long prob_brightness = random(50, 150);
      for(int i = 0; i < NUM_LEDS; i++) {   
        fade_black(prob_fade);
        leds[i] = CHSV(64, prob_saturation, prob_brightness);
        uint32_t color = leds_2.Color(leds[i].red, leds[i].green, leds[i].blue);
        leds_2.setPixelColor(i, color);
        leds_4.setPixelColor(i, color);
        leds_5.setPixelColor(i, color);
      }
      delay(prob_delay_speed);
    }
  leds_2.show();
  leds_4.show();
  leds_5.show();
  }
}

void loop(){
  yellow_flash_animation();
}

