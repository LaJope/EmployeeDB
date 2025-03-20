#include <cstdlib>
#include <cstring>
#include <string>

#include "AppSettings.h"
#include "Application.h"
#include "HelpMessage.h"
#include "Logger.h"

int main(int argc, char *argv[]) {

  AppSettings settings(argc, argv);
  Logger::GetInstance().Log("Finished processing command line arguments");

  if (settings.m_help) {
    Logger::GetInstance().Write(helpMessage);
    return EXIT_SUCCESS;
  }

  std::string psqlURL = "postgresql://postgres:hello@localhost:5432/postgres";
  Application app(psqlURL);

  Logger::GetInstance().Log("Starting application");
  int retcode = app.run(settings);

  Logger::GetInstance().Log("Application finished with return code " +
                            std::string(strerror(retcode)));

  // int retcode = 0;

  return retcode;
}
