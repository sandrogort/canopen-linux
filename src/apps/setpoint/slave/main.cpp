#include "app.h"

#include "drv_can_socketcan.h"

#include <string>
#include <cstring>
#include <vector>
#include <iostream>

int main(int argc, const char* argv[])
{
  std::string current_exec_name = argv[0]; // Name of the current exec program
  std::string arg1;

  if (argc == 2) {
      arg1 = argv[1];
  } else {
      std::cout << "Usage:" << std::endl;
      std::cout << argv[0] << " <can-interface-name>" << std::endl;
      return -1;
  }

  strcpy(CanInterfaceName, arg1.c_str());

  AppStart();
}
