/*
 * Futurama: a Stacker-esque Arduino game for the Art of Engineering 
 * common project. 
 * 
 * Required libraries:
 * Adafruit_LEDBackpack
 * Adafruit_GFX
 */

// Input Pin assignments
#define SPEED_POTENTIOMETER 0  // analog input
#define RED_BUTTON 12  // digital input
#define GREEN_BUTTON 13  // digital input

// Output Pin assignments
#define RED_BACKLIGHT 9  // PWM output
#define GREEN_BACKLIGHT 10  // PWM output
#define BLUE_BACKLIGHT 11  // PWM output

void setup() {
  Serial.begin(9600);
  // Set pin modes
  pinmode(SPEED_POTENTIOMETER, INPUT);
  pinmode(RED_BUTTON, INPUT_PULLUP);
  pinmode(GREEN_BUTTON, INPUT_PULLUP);
  pinmode(RED_BACKLIGHT, OUTPUT);
  pinmode(GREEN_BACKLIGHT, OUTPUT);
  pinmode(BLUE_BACKLIGHT, OUTPUT);
}

void loop() {
  
}

