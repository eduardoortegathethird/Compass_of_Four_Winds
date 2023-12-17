// Author | Eduardo Ortega
#include <FastLED_NeoPixel.h>
#define BUTTON_PIN 5
#define LED_PIN 4
#define FALSE 0
#define TRUE 1
#define NUM_LEDS 32
#define LED_TYPE            WS2812B
#define BRIGHTNESS          100
CRGB leds[NUM_LEDS];
FastLED_NeoPixel<NUM_LEDS, LED_PIN, NEO_GRB> leds_4;
int value = 0;
// 0-8 is RED (west)
// 8-16 is blue (north)
// 16-24 is yellow (east)
// 24-32 is white (south)
int b_thresh_1 = 4;
int b_thresh_2 = 28;
int r_thresh = 12;
int y_thresh = 20;
int w_thresh = 28;

void setup() {
  Serial.begin(115200);
  pinMode (BUTTON_PIN, INPUT);
  leds_4.begin();
  leds_4.setBrightness( BRIGHTNESS );
}

int read_button() {
  value = digitalRead(BUTTON_PIN);
  if(value == 1){
   return false;
  } else{
    return true;
  }
}

bool trigger_SPIN() {
  bool button_init = read_button();
  if(!button_init) { return false;}
  delay(750);
  bool button_after = read_button();
  if((button_init == true) & (button_after == true)) {
    return true;
  } else {
    return false;
  }
}

void set_color_based_on_index(int i) {
  if((i < b_thresh_1) | (i >= b_thresh_2)) {
    // BLUE
    leds[i] = CRGB(0, 0, 255);
    uint32_t color = leds_4.Color(leds[i].blue, leds[i].green, leds[i].red);
    leds_4.setPixelColor(i, color);
  } else if((i >= b_thresh_1) & (i < r_thresh)) {
    // RED 
    leds[i] = CRGB(255, 0, 0);
    uint32_t color = leds_4.Color(leds[i].blue, leds[i].green, leds[i].red);
    leds_4.setPixelColor(i, color);
  } else if((i >= r_thresh) & (i < y_thresh)) {
    // YELLOW
    leds[i] = CHSV(64, 230, 125);
    uint32_t color = leds_4.Color(leds[i].red, leds[i].green, leds[i].blue);
    leds_4.setPixelColor(i, color);
  } else {
    // WHITE
    leds[i] = CRGB(255, 255, 255);
    uint32_t color = leds_4.Color(leds[i].blue, leds[i].green, leds[i].red);
    leds_4.setPixelColor(i, color);
  }
}

void default_animation() {
  for(int i = 0; i < NUM_LEDS; i++) {
    set_color_based_on_index(i);
  }
  leds_4.show();
}

void black_out_LEDS(){
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CRGB(0, 0, 0);
    uint32_t color = leds_4.Color(leds[i].blue, leds[i].green, leds[i].red);
    leds_4.setPixelColor(i, color);
  }
  leds_4.show();
}

void spin_the_wheel(long num_of_spins, int delay_val) {
  int spin_count = 0;
  while(spin_count <= num_of_spins) {
    set_color_based_on_index(0);
    leds_4.show();
    delay(delay_val);
    for(int i=1; i < NUM_LEDS; i++) {
      leds[i-1] = CRGB(0, 0, 0);
      uint32_t color = leds_4.Color(leds[i-1].blue, leds[i-1].green, leds[i-1].red);
      leds_4.setPixelColor(i-1, color);
      set_color_based_on_index(i);
      leds_4.show();
      delay(delay_val);
    }
    leds[NUM_LEDS-1] = CRGB(0, 0, 0);
    uint32_t color = leds_4.Color(leds[NUM_LEDS-1].blue, leds[NUM_LEDS-1].green, leds[NUM_LEDS-1].red);
    leds_4.setPixelColor(NUM_LEDS-1, color);
    leds_4.show();
    delay(delay_val);
    spin_count += 1;
  }
}

void loop() {
  // s1 | do automatic LED show
  default_animation();
  // s1->s2 transition, check trigger SPIN
  bool spin_wheel = trigger_SPIN();
  if(spin_wheel) {
    Serial.println("Doing the animation");
    black_out_LEDS();
    delay(100);
    long num_of_spins = random(5, 10);
    spin_the_wheel(num_of_spins, 10);
  }
  // s2 |  if trigger SPIN do SPIN_LED
  delay(100);
}

