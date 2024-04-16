#include "FastLED.h"

#define HOUSE_LIGHTS_PIN 6
#define HOUSE_BREAKBEAM_SENSOR_PIN 4
#define HOUSE_NUM_LIGHT 36

// house light behavior variables
const byte HOUSE_LIGHT_COLOR_R = 0xab;
const byte HOUSE_LIGHT_COLOR_G = 0x40;
const byte HOUSE_LIGHT_COLOR_B = 0x02;
const byte LIGHT_SIZE = 10;
const byte LIGHT_TRAIL_DECAY = 64;
const boolean LIGHT_RANDOM_DECAY = true;
const int DELAY_TIME = 30;

CRGB house_lights[HOUSE_NUM_LIGHT];

// variable will change:
int ball_through_house = 0;         // variable for reading the breakbeam status

void fadeToBlack(int ledNo, byte fadeValue) {
  house_lights[ledNo].fadeToBlackBy( fadeValue );
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
  house_lights[Pixel].r = red;
  house_lights[Pixel].g = green;
  house_lights[Pixel].b = blue;
}

void turnOnFairyHouseLights() {  
  FastLED.clear();
 
  for(int i = 0; i < HOUSE_NUM_LIGHT+HOUSE_NUM_LIGHT; i++) {
  
    // fade brightness all house_lights one step
    for(int j=0; j<HOUSE_NUM_LIGHT; j++) {
      if( (!LIGHT_RANDOM_DECAY) || (random(10)>5) ) {
        fadeToBlack(j, LIGHT_TRAIL_DECAY );        
      }
    }
    
    // draw light
    for(int j = 0; j < LIGHT_SIZE; j++) {
      if( ( i-j <HOUSE_NUM_LIGHT) && (i-j>=0) ) {
        setPixel(i-j, HOUSE_LIGHT_COLOR_R, HOUSE_LIGHT_COLOR_G, HOUSE_LIGHT_COLOR_B);
      }
    }
    
    FastLED.show();
    delay(DELAY_TIME);
  }
}

void setup() {
  FastLED.addLeds<NEOPIXEL, HOUSE_LIGHTS_PIN>(house_lights, HOUSE_NUM_LIGHT); // set up fast house_lights
  pinMode(HOUSE_BREAKBEAM_SENSOR_PIN, INPUT); // initialize the sensor pin as an input
  digitalWrite(HOUSE_BREAKBEAM_SENSOR_PIN, HIGH); // turn on the pullup
}

void loop(){
  // read the state of the breakbeam value:
  ball_through_house = digitalRead(HOUSE_BREAKBEAM_SENSOR_PIN);
  // check if the sensor beam is broken (low)
  if (ball_through_house == LOW) {     
    turnOnFairyHouseLights(); 
  }
  // turn lights off:
  FastLED.clear();
}