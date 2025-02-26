#ifndef CLICLI_H
#define CLICLI_H
#include "I2project.h"

 class clicli {

  public:
   clicli(I2Project &I2project) ;
   void Begin();   //must be called from  void setup()
   void Run();   //must be called from  void loop()

  private:
   I2Project &myProject;
   int number;

 };
#endif 