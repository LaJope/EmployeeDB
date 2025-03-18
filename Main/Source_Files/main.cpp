#include <cstring>
#include <iostream>
#include <string>

// #include "Application.h"
#include "EmployeeGen.h"
#include "Logger.h"

// int main(int argc, char *argv[]) {
int main(int, char *[]) {

  // Application app(argc, argv);
  // Logger::GetInstance().Log("Finished processing command line arguments");
  //
  // Logger::GetInstance().Log("Starting application");
  // int retcode = app.run();
  // Logger::GetInstance().Log("Application finished with return code " +
  //                           std::string(strerror(retcode)));
 
  Logger::GetInstance().SetVerbose(true);


  auto vec = EmployeeGenerator::getEmployeeVector1M();
  Logger::GetInstance().Write("Hello1");
  std::cout << vec.size() << std::endl;

  int retcode = 0;

  return retcode;
}
