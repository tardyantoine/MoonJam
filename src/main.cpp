#include <iostream>
#include "universe.h"

int main(int argc, char* argv[])
{
  Universe universe;
  std::cout << "Universe go for launch, starting simulation..." << std::endl;
  universe.run();
  std::cout << "Exiting main simulation, goodbye" << std::endl;
  return 0;
}