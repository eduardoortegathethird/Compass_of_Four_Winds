/// @file    Fire2012.ino
/// @brief   Simple one-dimensional fire animation
/// @example Fire2012.ino

// All rights go to the original creator
// Utilizing this as basecode for LED art project
// Utilized by Eduardo Ortega
#include <Arduino.h>
#include <FastLED_NEOPixel.h>

#define LED_PIN_2 2
#define LED_PIN_4 4
#define CHIPSET WS2812B
#define NUM_LED_PER_STRIP 13
#define NUM_STRIP_PER_DC 15
// top 2 MULTIPLY TOGETHER
#define NUM_LEDS 195

#define BRIGHTNESS  100
#define FRAMES_PER_SECOND 45

CRGB leds_2[NUM_LEDS];
//CRGB leds_4[NUM_LEDS];

void setup() {
  delay(3000); // sanity delay
  Serial.begin(115200);
  FastLED.addLeds<CHIPSET, LED_PIN_2, COLOR_ORDER>(leds_2, NUM_LEDS).setCorrection( TypicalLEDStrip );
  //FastLED.addLeds<CHIPSET, LED_PIN_4, COLOR_ORDER>(leds_4, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );
}

void loop()
{
  // Add entropy to random number generator; we use a lot of it.
  // random16_add_entropy( random());

  //Simulation Frame for each strip
  Fire_strip(0, 13);
  //Fire_strip_rev(26, 13);

  
  FastLED.show(); // display this frame
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}


// Fire2012 by Mark Kriegsman, July 2012
// as part of "Five Elements" shown here: http://youtu.be/knWiGsmgycY
//// 
// This basic one-dimensional 'fire' simulation works roughly as follows:
// There's a underlying array of 'heat' cells, that model the temperature
// at each point along the line.  Every cycle through the simulation, 
// four steps are performed:
//  1) All cells cool down a little bit, losing heat to the air
//  2) The heat from each cell drifts 'up' and diffuses a little
//  3) Sometimes randomly new 'sparks' of heat are added at the bottom
//  4) The heat from each cell is rendered as a color into the leds array
//     The heat-to-color mapping uses a black-body radiation approximation.
//
// Temperature is in arbitrary units from 0 (cold black) to 255 (white hot).
//
// This simulation scales it self a bit depending on NUM_LEDS; it should look
// "OK" on anywhere from 20 to 100 LEDs without too much tweaking. 
//
// I recommend running this simulation at anywhere from 30-100 frames per second,
// meaning an interframe delay of about 10-35 milliseconds.
//
// Looks best on a high-density LED setup (60+ pixels/meter).
//
//
// There are two main parameters you can play with to control the look and
// feel of your fire: COOLING (used in step 1 above), and SPARKING (used
// in step 3 above).
//
// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100 
#define COOLING  45

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 60

void Fire_strip(int start, int end)
{
// Array of temperature readings at each simulation cell
  static uint8_t heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
    for( int i = start; i < end; i++) {
      heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / end) + 2));
    }
  
    // Step 2.  Heat from each cell drifts 'up' and diffuses a little
    for( int k= end - 1; k >= (start+2); k--) {
      heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
    }
    
    // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
    if( random8() < SPARKING ) {
      int y = random8(7);
      heat[y] = qadd8( heat[y], random8(160,255) );
    }

    // Step 4.  Map from heat cells to LED colors
    for( int j = start; j < end; j++) {

      CRGB color = HeatColor(heat[j]);
      leds_2[j] = color;
      leds_2[j+(2*end)] = color;
      leds_2[j+(4*end)] = color;
      leds_2[j+(6*end)] = color;
      leds_2[j+(8*end)] = color;
      leds_2[j+(10*end)] = color;
      leds_2[j+(12*end)] = color;
      leds_2[j+(14*end)] = color;
      leds_2[j+(16*end)] = color;
      leds_2[((2*end)-1)-j] = color;
      leds_2[((4*end)-1)-j] = color;
      leds_2[((6*end)-1)-j] = color;
      leds_2[((8*end)-1)-j] = color;
      leds_2[((10*end)-1)-j] = color;
      leds_2[((12*end)-1)-j] = color;
      leds_2[((14*end)-1)-j] = color;
      /*
      leds_4[j] = color;
      leds_4[j+(2*end)] = color;
      leds_4[j+(4*end)] = color;
      leds_4[j+(6*end)] = color;
      leds_4[j+(8*end)] = color;
      leds_4[j+(10*end)] = color;
      leds_4[j+(12*end)] = color;
      leds_4[j+(14*end)] = color;
      leds_4[((2*end)-1)-j] = color;
      leds_4[((4*end)-1)-j] = color;
      leds_4[((6*end)-1)-j] = color;
      leds_4[((8*end)-1)-j] = color;
      leds_4[((10*end)-1)-j] = color;
      leds_4[((12*end)-1)-j] = color;
      leds_4[((14*end)-1)-j] = color;
      */
    }
}



