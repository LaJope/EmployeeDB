#include <cstring>
#include <string>

#include "Application.h"
#include "Logger.h"

int main(int argc, char *argv[]) {

  Application app(argc, argv);

  Logger::GetInstance().Log("Finished processing command line arguments");

  Logger::GetInstance().Log("Starting application");

  int retcode = app.run();

  Logger::GetInstance().Log("Application finished with return code " +
                            std::string(strerror(retcode)));

  // Logger::GetInstance().Log("Start generating pseudorandom employees");
  // auto vec = EmployeeGenerator::getEmployeeVector1M();
  // Logger::GetInstance().Log("Finished generating pseudorandom employees");
  // std::cout << vec.capacity() * sizeof(vec[0]) / 8 / 1024 / 1024 <<
  // std::endl;
  //
  // int retcode = 0;

  return retcode;
}
