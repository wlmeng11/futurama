/*
 * Futurama: a Stacker-esque Arduino game for the Art of Engineering 
 * common project. 
 * 
 * Target board: Arduino UNO R3
 * Processor: ATmega328p, 16 MHz
 * 
 * Required libraries:
 * Adafruit_LEDBackpack
 * Adafruit_GFX
 * ST7565
 */

#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include "ST7565.h"

// Input Pin assignments
#define SPEED_POTENTIOMETER 0  // analog input
#define RED_BUTTON 12  // digital input
#define GREEN_BUTTON 13  // digital input

// Output Pin assignments
#define RED_BACKLIGHT 9  // PWM output
#define GREEN_BACKLIGHT 10  // PWM output
#define BLUE_BACKLIGHT 11  // PWM output
int red = 0;
int green = 0;
int blue = 0;

// 7-segment numeric display to show the user's score
Adafruit_7segment scoreboard = Adafruit_7segment();
uint16_t score = 0;

// Graphic LCD for playing the game
// pin 8 - Serial data out (SID)
// pin 7 - Serial clock out (SCLK)
// pin 6 - Data/Command select (RS or A0)
// pin 5 - LCD reset (RST)
// pin 4 - LCD chip select (CS)
ST7565 glcd(8, 7, 6, 5, 4);

void setup() {
  Serial.begin(9600);
  Serial.println("Welcome to Futurama!");
  // code for graphic LCD
  #ifdef __AVR__
    Serial.print("Bytes of RAM available: ");
    Serial.println(freeRam());
  #endif
  // initialize graphic LCD and set the contrast to 0x18
  glcd.begin(0x18);

  setColor(255,0,0);  // set backlight to red
  glcd.display(); // show splashscreen
  delay(2000);
  glcd.clear();

  // Set pin modes
  pinMode(SPEED_POTENTIOMETER, INPUT);
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(GREEN_BUTTON, INPUT_PULLUP);
  pinMode(RED_BACKLIGHT, OUTPUT);
  pinMode(GREEN_BACKLIGHT, OUTPUT);
  pinMode(BLUE_BACKLIGHT, OUTPUT);

  scoreboard.begin(0x70);  // connect to numeric display at I2C address 0x70
  scoreboard.print(score);
  scoreboard.writeDisplay();

}

void loop() {
  if (digitalRead(RED_BUTTON) == LOW) {
    Serial.println("Red button pressed!");
  }
  glcd.fillrect(5, 5, 10, 10, BLACK);
  glcd.display();
  glcd.clear();

  score++;
  scoreboard.print(score);
  scoreboard.writeDisplay();
  delay(20);
}

/*
 * The code for setColor() is based on code from the Adafruit RGB LED tutorial.
 * https://learn.adafruit.com/adafruit-arduino-lesson-3-rgb-leds/arduino-sketch
 */
void setColor(int red, int green, int blue) {
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(RED_BACKLIGHT, red);
  analogWrite(GREEN_BACKLIGHT, green);
  analogWrite(BLUE_BACKLIGHT, blue);  
}

/*
 * The code for freeRam() comes from the Adafruit ST7565 example.
 * https://github.com/adafruit/ST7565-LCD/blob/master/ST7565/examples/st7565lcd/st7565lcd.pde
 */
#ifdef __AVR__
// this handy function will return the number of bytes currently free in RAM, great for debugging!   
int freeRam(void) {
  extern int  __bss_end; 
  extern int  *__brkval; 
  int free_memory; 
  if((int)__brkval == 0) {
    free_memory = ((int)&free_memory) - ((int)&__bss_end); 
  }
  else {
    free_memory = ((int)&free_memory) - ((int)__brkval); 
  }
  return free_memory; 
} 
#endif
