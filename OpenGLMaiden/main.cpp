// Author: pampas_83
// abhijit.93@hotmail.com

#include <iostream>

#include "display.h"

int main(int argc, char** argv) {

  Display display(800, 600, "OpenGL");
  
  while (!display.IsClosed()) {
    
    display.Clear(0.0f, 0.4f, 0.3f, 1.0f);
    display.Update();
  }

  return 0;
}