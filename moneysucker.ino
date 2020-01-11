// Dragon staff LED controller using FastLED
// 2019 Mike Fink
// MIT License

#include <stdlib.h>
#include "FastLED.h"
#include <pixeltypes.h>
#include <stdlib.h>

FASTLED_USING_NAMESPACE

#define BUTTON_PIN        5
#define LED_PIN           14
#define LED_TYPE          WS2812B
#define COLOR_ORDER       GRB
#define NUM_LEDS          300
#define BRIGHTNESS        128
#define FRAMES_PER_SECOND 500

CRGB leds[NUM_LEDS];

uint8_t gHue = 0; //rainbow cycle hue index
int wHue = 0;

uint32_t button_press_time = -3000;
int button_press_length = 3000;

//off-orange-red-pink-off gradient palette to fade through
DEFINE_GRADIENT_PALETTE( fast_rainbow ) {
0,   0,   0,   0,
0,   0,   0,   0,
0,   0,   0,   0,
31,  255, 0,   0,
63,  255, 255, 0,
95,  0,   255, 0,
127, 0,   255, 255,
159, 0,   0,   255,
191, 255, 0,   255,
223, 255, 0,   0,
255, 0,   0,   0};

CRGBPalette16 rainPal = Rainbow_gp;
CRGBPalette16 fastPal = fast_rainbow;

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(BUTTON_PIN, button_interrupt, RISING);
  FastLED.addLeds<LED_TYPE,LED_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
  if ((long)(millis()-button_press_time) <= button_press_length) {
    // Run fast leds
    rainbow_fast(fastPal);
  } else {
    // Run slower leds
    rainbow_slow();
  }
}

void rainbow_slow() {
  //slowly fade through a rainbow gradient fill
  fill_rainbow(leds, NUM_LEDS, gHue, 1);
  FastLED.show();
  EVERY_N_MILLISECONDS(1) {gHue++;}
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void rainbow_fast(CRGBPalette16 pal) {
  //cycle through an arbitrary gradient palette pretty quickly
  for( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette(pal, wHue + i*6);
  }
  FastLED.show();
  wHue+=8;
  FastLED.delay(1000/FRAMES_PER_SECOND);
}

void button_interrupt() {
  // Interrupt function to change led program with debounce
  static unsigned long last_interrupt_time = 0;
    unsigned long interrupt_time = millis();
    if (interrupt_time - last_interrupt_time > 200) 
    {
      button_press_time = interrupt_time;
    }
  last_interrupt_time = interrupt_time;
}
