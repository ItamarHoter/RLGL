#include "I2project.h"
#include <Arduino.h>
#include "ev3lego.h"
#include <Wire.h>
#include "Adafruit_TCS34725.h"
#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>
#include "HardwareSerial.h"


#define PIN 10
#define NUM_PIXELS 12

#define REDUCTOR 65
bool isGreen;
bool isRed;
float r, g, b;
volatile unsigned long lastInterruptTime = 0;  // Store last time ISR was triggered
const unsigned long debounceDelay = 50;  // 50ms debounce delay
volatile bool flag = false;

Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
Adafruit_NeoPixel strip(NUM_PIXELS, PIN, NEO_GRB + NEO_KHZ800);
LiquidCrystal_I2C lcd(0x27, 16, 2);

ev3lego mymotor(3, //encoder1(blue). must be either pin 2 or 3
                4, //encoder2(yellow)
                6, //in1 (L298N)
                7, //in2
                5, //enA
                65); //wheel size
ev3lego mymotor2(3, 4, 8, 9, 11, 65);

I2Project::I2Project(int b) {}

void buttonPressed() {
    unsigned long currentTime = millis();
    
    // Check if the last interrupt was too recent (bounce protection)
    if (currentTime - lastInterruptTime > debounceDelay) {
        flag = !flag;
        lastInterruptTime = currentTime;  // Update last press time
        Serial.println(flag);
    }
}
void I2Project::Begin()
{
  lcd.init();
  Serial.begin(115200);
  lcd.clear();
  
  lcd.backlight();
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(2, INPUT_PULLUP);
  lcd.print("welcome"); 
  attachInterrupt(digitalPinToInterrupt(2), buttonPressed, FALLING);

  delay(500);
  lcd.clear();
}
void I2Project::Run(){
  if(flag == true){
  lcd.setCursor(0, 0);
  lcd.print("Mode_2 Activated");
  lcd.setCursor(0, 1);
  lcd.print("Awaiting color..");
  getColor();
  CheckColor();
  colorGo();
  if (isGreen)
  {
  lcd.setCursor(0, 1);
  lcd.print("Found Green!          ");
  setAllPixels(0,100,0);
  delay(500);
  }
  else if(isRed) {
    lcd.setCursor(0, 1);
    lcd.print("Found Red!       ");
    setAllPixels(100, 0, 0);
    delay(500);
  }
  }
  else if (flag == false)
  {
    setAllPixels(100, 100, 100);
    lcd.setCursor(0, 0);
    mymotor.motgo(0);
    mymotor2.motgo(0);
    lcd.print("Mode_1 Activated");
    lcd.setCursor(0, 1);
    lcd.print("Observing...    ");
    delay(200);

  }
  
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

  if (b > g && b > r)
  {
  isRed = false;
  isGreen = false;
  }
  else if(g > r + 20)
  {
    isGreen = true;
    isRed = false;
  }
  else if (r > g + 20)
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
     mymotor2.motgo(0);
     Serial.println("It's red");
  }
  else if (isGreen) 
  {
    mymotor.motgo(-100);
    mymotor2.motgo(-100);
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

void I2Project::setAllPixels(int red, int green, int blue) {
  for (int i = 0; i < NUM_PIXELS; i++) {
    strip.setPixelColor(i, strip.Color(red, green, blue));

  }
}
