#include "I2project.h"
#include <Arduino.h>
#include "ev3lego.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>

#define PIN 10
#define NUM_PIXELS 12

#define REDUCTOR 65
bool isGreen;
bool isRed;
float r, g, b;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
Adafruit_NeoPixel strip(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);

ev3lego mymotor(3, //encoder1(blue). must be either pin 2 or 3
                4, //encoder2(yellow)
                6, //in1 (L298N)
                7, //in2
                5, //enA
                65); //wheel size

I2Project::I2Project(int b) {}

void I2Project::Begin()
{
  Serial.begin(115200);


}
void I2Project::Run(){
  getColor();
  CheckColor();
  colorGo();
  setAllPixels(int(r), int(g), int(b));
  NeoPrint();
}
void I2Project::CheckColor()
{
    if (r > REDUCTOR)
    r -= REDUCTOR; 
  else
    r = 0;
  
  if (g > REDUCTOR)
    g -= REDUCTOR;
  else
    g = 0;
  if ( b > REDUCTOR)
    b -= REDUCTOR;
  else
    b = 0;

  if (b >= g || b >= r)
  {
  isRed = false;
  isGreen = false;
  }
  else if(g > r)
  {
    isGreen = true;
    isRed = false;
  }
  else if (r > g + 40)
  {
    isRed = true;
    isGreen = false;
  }

  Serial.print("R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.println(b);
  Serial.print("is red: "); Serial.println(isRed);
  Serial.print("is Green: "); Serial.println(isGreen);

}

void I2Project::colorGo ()
{
  if (isRed)
  {
     mymotor.motgo(0);
     Serial.println("It's red");
  }
  else if (isGreen) 
  {
    mymotor.motgo(100);
    Serial.println("It's green");
  }
  else
  {
    Serial.println(" None of the conditions have been met");
  }

}

void I2Project::getColor() {
  tcs.getRGB(&r, &g, &b);
}

void setAllPixels(int red, int green, int blue) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));

  }
}

void NeoPrint()
{
  if (r > REDUCTOR)
    r -= REDUCTOR; 
  else
    r = 0;
  
  if (g > REDUCTOR)
    g -= REDUCTOR;
  else
    g = 0;
  if ( b > REDUCTOR)
    b -= REDUCTOR;
  else
    b = 0;
   // Print RGB values for debugging

  
  Serial.print("R: "); Serial.print(r);
  Serial.print(" G: "); Serial.print(g);
  Serial.print(" B: "); Serial.println(b);

  // Set NeoPixel colors based on sensor readings
  setAllPixels((int)r, (int)g, (int)b);
  strip.show();

  delay(500);
  strip.clear();
}