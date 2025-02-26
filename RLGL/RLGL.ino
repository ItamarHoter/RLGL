#include "I2project.h"
#include "Adafruit_TCS34725.h"
#include "clicli.h"


I2Project myProject(0);
clicli mycli(myProject);

void setup() {
  mycli.Begin();


}

void loop() {
  mycli.Run();

}

