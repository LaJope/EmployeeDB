#pragma once

static const char *helpMessage = R""""(
usage: ptmk [-h | --help] [-v | --verbose] [-l | --logfile <filename>]
       mode [<Employee data for mode 2>]

Default:
  logfile "ptmk.log"

--help    - Show this help message
--verbose - Show all logs and warnings
--logfile - Set log filename (.log) (Only logs for mode 5)

Modes:
1 - Create database Employee with (ID, fullname, birthDate, gender)
2 - Insert provided in command line arguments Employee info into the database.
Arguments format: "employee-fullname" "employee-birthdate" "Male/Female"
                  Any String          String %Y-%M-%d      String
3 - Query the database for all employees + calculate employee years
4 - Generate ~1M pseudorandom employees and insert them into database
5 - Test querying the database with different variations of indexes.
    Writes test results to logfile
)"""";
