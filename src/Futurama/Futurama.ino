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
#include "LinkedList.h"

// Input Pin assignments
#define SPEED_POTENTIOMETER 0  // analog input
#define RED_BUTTON 13  // digital input

// Output Pin assignments
#define RED_BACKLIGHT 9  // PWM output
#define GREEN_BACKLIGHT 10  // PWM output
#define BLUE_BACKLIGHT 11  // PWM output
int red = 0;
int green = 0;
int blue = 0;
#define COMMON_ANODE

// 7-segment numeric display to show the user's score
Adafruit_7segment scoreboard = Adafruit_7segment();
uint16_t score = 0;

/* Graphic LCD for playing the game
 *  Dimensions: 128x64 pixels
 *  pin 8 - Serial data out (SID)
 *  pin 7 - Serial clock out (SCLK)
 *  pin 6 - Data/Command select (RS or A0)
 *  pin 5 - LCD reset (RST)
 *  pin 4 - LCD chip select (CS)
*/
ST7565 glcd = ST7565(8, 7, 6, 5, 4);

int x = 0;
int y = 0;
int length = 40;
int level = 1;
int height = 5;
bool direction = true;
int left_y;
int right_y;
LinkedList<int> listX = LinkedList<int>();
LinkedList<int> listY = LinkedList<int>();
LinkedList<int> listLength = LinkedList<int>();

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

  setColor(255,100,255);  // set backlight to red
  glcd.display(); // show splashscreen
  delay(6000);
  glcd.clear();

  // Set pin modes
  pinMode(SPEED_POTENTIOMETER, INPUT);
  pinMode(RED_BUTTON, INPUT_PULLUP);
  pinMode(RED_BACKLIGHT, OUTPUT);
  pinMode(GREEN_BACKLIGHT, OUTPUT);
  pinMode(BLUE_BACKLIGHT, OUTPUT);

  scoreboard.begin(0x70);  // connect to numeric display at I2C address 0x70
  scoreboard.print(score);
  scoreboard.writeDisplay();

}

void loop() {
  if(y+length == 64) { //handles direction of box movement
    direction = true;
  } else if (y == 0) {
    direction = false;
  }

  if(direction) { //Changes y movement
    y--;
  } else {
    y++;
  }

  if(level != 1) {
    glcd.fillrect((128-((level-1)*height))%128, y%64, height, length, BLACK);
  }
  
  glcd.display();
  glcd.clear();
  scoreboard.print(score);
  scoreboard.writeDisplay();
  delay(getSpeed()/5);
  
  if (digitalRead(RED_BUTTON) == LOW) {
    Serial.println("Red button pressed!");
    if(level > 2) {
      left_y = listY.get(level-2) + listLength.get(level-2);
      right_y = listY.get(level-2);
            
      if(y < right_y && (y+length) < left_y) { 
        length -= (right_y - y);               
        y = right_y;
      } else if(y > right_y &&(y+length) > left_y) {
        length -= (y + length - left_y);
        y = left_y - length;
      }
    }
    if(length > 0) {
      setColor(255, length*6, length*6);
    } else {
      end();
    }
    
    listX.add(128-((score)*height));
    listY.add(y%64);
    listLength.add(length);
    
    
    if(level < 15) { //20 is the max number of levels displayed
      level++;
    } else {
      listX.remove(0);
      listY.remove(0);
      listLength.remove(0);
    }
    score++;
  }

  for (int i = 0; i < listX.size(); i++) {
    glcd.fillrect(128-listX.get(0)+listX.get(i), listY.get(i), height, listLength.get(i), BLACK);
  }
}

/*
 * Returns the speed setting as an integer from 0 to 1023
 */
 int getSpeed() {
  int speed_setting = analogRead(SPEED_POTENTIOMETER);
  Serial.print("Speed: "); Serial.println(speed_setting);
  return speed_setting;
 }

 void end() {
  while(true) {
    setColor(255,0,0);
    int size_stack = listX.size();
    for (int i = 0; i < size_stack; i++) {
      for (int j = 0; j < listX.size(); j++) {
        glcd.fillrect(128-listX.get(0)+listX.get(j), listY.get(j), height, listLength.get(j), BLACK);
      }
      listX.remove(listX.size()-1);
      listY.remove(listX.size()-1);
      listLength.remove(listX.size()-1);
      glcd.display();
      glcd.clear();
      delay(getSpeed()/5);
    }
  }
  glcd.display();
  glcd.clear();
  delay(getSpeed()/5);
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
