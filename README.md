## Help
usage: ptmk [-h | --help] [-v | --verbose] [-l | --logfile \<filename\>]
       mode [<Employee data for mode 2>]

Default:  
  logfile "ptmk.log"

--help    - Show this help message  
--verbose - Show all logs and warnings  
--logfile - Set log filename (.log) (Only logs for mode 5)  

Modes:  
1 - Create database Employee with (ID, fullname, birthDate, gender)  
2 - Insert provided in command line arguments Employee info into the database.  
Arguments format:    
|"employee-fullname"|"employee-birthdate"|"Male/Female"|
|-|-|-|
|Any String|String %Y-%M-%d|String|
                                    
3 - Query the database for all employees + calculate employee years  
4 - Generate ~1M pseudorandom employees and insert them into database  
5 - Test querying the database with different variations of indexes.
Writes test results to logfile

## Dependencies
For building the project you need to install g++ (that supports c++20), cmake, libpq and postgresql-server-dev-13 (or other version).
You can install them using your prefered package manager. For example:
#### Ubuntu
```bash
sudo apt install g++ cmake libpq-dev postgresql-server-dev-13
```

## Build
To build the program in project root directory type:
```bash
cmake -S . -B build ;
cmake --build build
```
The executable will be placed in ./build/bin/

## Launch
The program connects to postgres server
```
postgresql://postgres:hello@localhost:5432/postgres
```
You can easily launch the official postgres docker image with all the necessary information
```bash
docker run -e POSTGRES_PASSWORD=hello -e POSTGRES_DB=postgres -d -p 5432:5432 postgres
```

## [Postgresql c++ library used in this project](https://github.com/jtv/libpqxx)
**Скорость с партициями и индексам fullname и gender**: ~~ 90 миллисекунд  

Всё время выполнения - на моей локальной машине. При тесте на сервере время
выполнения запросов с left(fullname, 1) = 'F' в некоторых случаях снижалось до
40 миллисекунд, а с LIKE оставалось примерно таким же.
