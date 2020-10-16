#include <iostream>
#include <cstdlib>
#include "universe.h"

int main(int argc, char* argv[])
{
  int x = 0;
  if(argc > 1) {
    x = atoi(argv[1]);
    if(x < 1 || x > 5) {
      std::cout << "Invalid number of planets as argument (valid range is [1 - 5]), launching with 1 planet..." << std::endl;
    }
    else {
      std::cout << x << " planets requested, launching..." << std::endl;
    }
  }

  Universe universe;
  std::cout << "Universe go for launch, starting simulation..." << std::endl;
  universe.run(x);
  std::cout << "Exiting main simulation, goodbye" << std::endl;
  return 0;
}