#include <FastLED.h>

#define READ_TIMES 100

#define LED_PIN 7
#define BAT_PIN A0
#define BUTTON_PIN 2
#define LOAD_PIN 11

#define MAX_V 4.3
#define MIN_V 3.6
#define HYSTERESIS_V 0.15
#define BRIGHTNESS 255
#define LED_SECONDS 10

CRGB led[1];
void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), button, FALLING);

  pinMode(LOAD_PIN, OUTPUT);
  
  FastLED.addLeds<WS2812, LED_PIN, GRB>(led, 1);
  led[0] = CRGB(0, 0, 0);
  FastLED.show();
  Serial.begin(9600);
}

volatile unsigned long time;
void button(){
  time = millis();
}

float float_map(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float bat_level;
int led_state;
int counter = 0;
unsigned long avg_value = 0;
bool led_off = false;
bool low_bat = false;
void loop() {
  if(counter == READ_TIMES) {
    bat_level = float_map(((float)avg_value)/READ_TIMES, 0.0, 1023.0, 0.0, 4.995);

    if(bat_level <= MIN_V + ALLOWED_ERROR_V + ((low_bat == true) ? HYSTERESIS_V : 0)) {
      digitalWrite(LOAD_PIN, LOW);
      low_bat = true;
    } else {
      digitalWrite(LOAD_PIN, HIGH);
      low_bat = false;
    }
    
    if(millis() - time <= LED_SECONDS*1000){
      led_state = (int)float_map(bat_level, MIN_V, MAX_V, 0.0, (float)BRIGHTNESS);
      if(led_state>BRIGHTNESS) {
        led_state = BRIGHTNESS;
      } else if (led_state<0) {
        led_state = 0;
      }
      Serial.print(bat_level);
      Serial.print("  ");
      Serial.println(led_state);
      led[0] = CRGB(map(led_state, 0, BRIGHTNESS, BRIGHTNESS, 0), led_state, 0);
      FastLED.show();
      led_off = false;
    } else if (!led_off){
      led[0] = CRGB(0, 0, 0);
      FastLED.show();
      led_off = true;
    }
    counter = 0;
    avg_value = 0;
  } else {
    avg_value += analogRead(BAT_PIN);
    counter++;
  }
}
