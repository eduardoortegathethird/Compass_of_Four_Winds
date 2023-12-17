// Author | Eduardo Ortega
#include <Arduino.h>
#include <FastLED_NeoPixel.h>

#define LED_PIN_2 2
#define LED_PIN_4 4
#define LED_PIN_5 5
#define CHIPSET WS2812B
#define NUM_LED_PER_STRIP 13
#define NUM_STRIP 15
#define NUM_LEDS 195

#define BRIGHTNESS  100
#define FRAMES_PER_SECOND 15

FastLED_NeoPixel<NUM_LEDS, LED_PIN_2, NEO_GRB> leds_2;
FastLED_NeoPixel<NUM_LEDS, LED_PIN_4, NEO_GRB> leds_4;
FastLED_NeoPixel<NUM_LEDS, LED_PIN_5, NEO_GRB> leds_5;

void setup() {
  delay(3000); // sanity delay
  Serial.begin(115200);
  leds_2.begin();
  leds_4.begin();
  leds_5.begin();
  leds_2.setBrightness( BRIGHTNESS );
  leds_4.setBrightness( BRIGHTNESS );
  leds_5.setBrightness( BRIGHTNESS );
}

void RED_ANIMATION()
{
  //Simulation Frame for each strip
  Fire_strip();
  leds_2.show();
  leds_4.show();
  leds_5.show();
  delay(1000 / FRAMES_PER_SECOND);
}

void assign_FIRE(int cell_index, uint32_t color, int strip) {
  if( strip == 0) {
    leds_2.setPixelColor(cell_index, color);
    leds_4.setPixelColor(cell_index, color);
    leds_5.setPixelColor(cell_index, color);
  } else {
    leds_2.setPixelColor(cell_index+(strip*2*NUM_LED_PER_STRIP), color);
    leds_2.setPixelColor(((strip*2*NUM_LED_PER_STRIP)-1)-cell_index, color);
    leds_4.setPixelColor(cell_index+(strip*2*NUM_LED_PER_STRIP), color);
    leds_4.setPixelColor(((strip*2*NUM_LED_PER_STRIP)-1)-cell_index, color);    
    leds_5.setPixelColor(cell_index+(strip*2*NUM_LED_PER_STRIP), color);
    leds_5.setPixelColor(((strip*2*NUM_LED_PER_STRIP)-1)-cell_index, color);
  }
}

void Fire_strip()
{
  static uint8_t heat[NUM_LED_PER_STRIP][NUM_STRIP];

  for( int j = 0; j < NUM_STRIP; j++) {
    long COOLING = random(42, 48);
    for( int i = 0; i < NUM_LED_PER_STRIP; i++) {
      //long COOLING = random(42, 48);
      heat[i][j] = qsub8( heat[i][j],  random8(0, ((COOLING * 10) / NUM_LED_PER_STRIP) + 2));
    }
  }
  
  for( int j = 0; j < NUM_STRIP; j++) {
    for( int k= NUM_LED_PER_STRIP - 1; k >= (0+2); k--) {
      heat[k][j] = (heat[k - 1][j] + heat[k - 2][j] + heat[k - 2][j] ) / 3;
    }
  }

  for ( int j = 0; j < NUM_STRIP; j++) {
    long SPARKING = random(55, 65);
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y][j] = qadd8( heat[y][j], random8(160,255) );
    }
  }

  for ( int strip = 0; strip < NUM_STRIP; strip++) {
    for( int cell_index = 0; cell_index < NUM_LED_PER_STRIP; cell_index++) {
      CRGB col_h = HeatColor(heat[cell_index][strip]);
      uint32_t color = leds_2.Color(col_h.red, col_h.green, col_h.blue);
      assign_FIRE(cell_index, color, strip);
    }
  }
}

void loop() {
  RED_ANIMATION();
}



