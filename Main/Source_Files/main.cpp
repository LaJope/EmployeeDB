#include <cstring>
#include <string>

#include "Application.h"
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

  int i = 0;
  for (auto &elem : PseudoRandomEmployee::getEmployeeVector()) {
    Logger::GetInstance().Write(elem);
    if (i++ == 30)
      break;
  }
  int retcode = 0;

  return retcode;
}
