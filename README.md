# Li-Ion_checker
Sketch for checking Li-Ion battery voltage and cutting it if it's too small.
Also uses addresable LED for voltage indicating to save arduino pins, so you can use it with really small boards like Digispark ATTINY85.
One pin for load; one pin for LED; one pin for button to enable LED (interrupt).
## Dependency
FastLED library for Arduino.
You can install it directly from Arduino IDE library manager or from github: https://github.com/FastLED/FastLED
## Settings
You can change some values in defines.
### Pins
#### LED_PIN 
That pin connects to DIN on LED.
#### BAT_PIN 
Analog pin.
Connect it to battery through resistor (minimum 10k).
#### BUTTON_PIN
Pin that connects to button (button connects to ground).
Pin is internally pulled up.
Interrupt needs to be attached to that pin, so you need to check which pins support interrupts on your board:
https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
#### LOAD_PIN 
Pin that is responsible for load managing.
You should connect it to MOSFET transistor or relay, etc. Or you can just don't touch it if you only want to indicate voltage by LED.
### Parameters
#### READ_TIMES 
Arduino reads data on analog pin several times to make average number so ve can exclude random noises.
#### MAX_V 
Used for LED values mapping.
Maximum voltage for Li-Ion is 4.2V, sometimes it can be 4.3V or 4.4V.
#### MIN_V 
Used to cut-off load from battery if there is too few voltage.
Minimum voltage often is 2.5V-3.0V, but it's better not to take a risk and set it to 3.4V-3.6V.
#### HYSTERESIS_V
So, here is situation: 
Battery is under heavy load, and now cell voltage is lower than MIN_V. Arduino disables load, and now voltage is higher than MIN_V due to voltage drop from load. Load enables, works a bit, disables again and so on.
Hysteresis fixes it - if load was disabled, than battery should have MIN_V+HYSTERESIS_V voltage to enable load.
#### BRIGHTNESS 
I think this is clear.
#### LED_SECONDS 
After clicking button LED will work during this time.
