#include "ev3lego.h"
#include "Adafruit_TCS34725.h"
#define REDUCTOR 65
bool isRed;
bool isGreen;
float r, g, b;
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);

ev3lego mymotor(3, //encoder1(blue). must be either pin 2 or 3
                4, //encoder2(yellow)
                6, //in1 (L298N)
                7, //in2
                5, //enA
                65); //wheel size



void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}

void loop()
{
    tcs.getRGB(&r, &g, &b);
    CheckColor(r, g, b);
    colorGo(isRed, isGreen);
}

void CheckColor(float r, float g, float b)
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
  else if (r > g)
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

void colorGo (bool isRed, bool isGreen)
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
