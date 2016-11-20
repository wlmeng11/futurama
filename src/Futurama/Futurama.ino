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

// Input Pin assignments
#define SPEED_POTENTIOMETER 0  // analog input
#define RED_BUTTON 12  // digital input
#define GREEN_BUTTON 13  // digital input

// Output Pin assignments
#define RED_BACKLIGHT 9  // PWM output
#define GREEN_BACKLIGHT 10  // PWM output
#define BLUE_BACKLIGHT 11  // PWM output

Adafruit_7segment scoreboard = Adafruit_7segment();
uint16_t score = 0;

void setup() {
  Serial.begin(9600);

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
  score++;
  scoreboard.print(score);
  scoreboard.writeDisplay();
  delay(20);
}

