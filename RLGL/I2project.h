                              */
#ifndef I2PROJECT_H
#define I2PROJECT_H
#include "I2project.h"

class I2Project { 

  public:
    I2Project(int b); 
    void Begin();
    void Run();
    void CheckColor();
    void colorGo(); 
    void getColor();
    void setAllPixels(int red, int green, int blue);
  private:
    int _button;
};


#endif 
