# [Проект на github](https://github.com/LaJope/EmployeeDB)

# Перед запуском

Программа подключается к серверу postgresql с именем postgres и паролем hello
```
postgresql://postgres:hello@localhost:5432/postgres
```
Можно запустить в docker
```bash
docker run -e POSTGRES_PASSWORD=hello -e POSTGRES_DB=postgres -d -p 5432:5432 postgres
```

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

## [Postgresql c++ library used in this project](https://github.com/jtv/libpqxx)


# Пункт 6
## При всех измерениях разброс времени ~~ 20 миллисекунд
## Без партиций
```sql
CREATE TYPE sex AS ENUM('Male', 'Female');
CREATE TABLE Employee (
    ID SERIAL PRIMARY KEY, 
    fullname VARCHAR(100) NOT NULL, 
    birthDate DATE NOT NULL, 
    gender sex NOT NULL);
```
Запросы (по времени одинаковы):
```sql
SELECT * FROM Employee WHERE fullname LIKE 'F%' AND gender = 'Male';
SELECT * FROM Employee WHERE left(fullname, 1) = 'F%' AND gender = 'Male';
```
**Скорость без всего**: ~~ 140 миллисекунд  
**Скорость с индексами на полях ФИО/Пол** (везде примерно одинакова): ~~ 130 миллисекунд  
## С партициями
```sql
CREATE TYPE sex AS ENUM('Male', 'Female');
CREATE TABLE Employee (
    ID SERIAL, 
    fullname VARCHAR(100) NOT NULL, 
    birthDate DATE NOT NULL, 
    gender sex NOT NULL
) PARTITION BY LIST (left(fullname, 1));
-- Затем создаю партиции для всех букв латинского алфавита A-Z
```
Партиции - в моём понимании, дополнительные таблицы, куда раскидываются данные
изначальной таблицы по определённому критерию. В данном примере критерий -
первая буква ФИО.  
Потом при поиске по данному критерию (используя ту же формулировку, что и при
создании партиций) база данных будет обращаться лишь к той(тем) таблицам,
которые подходят по указанному в запросе критерию, ускоряя выполнение запроса.
#### Запрос с LIKE:
```sql
SELECT * FROM Employee WHERE fullname LIKE 'F%' AND gender = 'Male';
```
**Скорость с партициями**: ~~ 150 миллисекунд  
**Скорость с партициями и индексам fullname**: ~~ 140 миллисекунд  
**Скорость с партициями и индексам gender**: ~~ 140 миллисекунд  
**Скорость с партициями и индексам fullname и gender**: ~~ 140 миллисекунд  
#### Запрос с left(fullname, 1) = 'F':
```sql
SELECT * FROM Employee WHERE fullname LIKE 'F%' AND gender = 'Male';
```
###### ***Все варианты выполняются в рамках (80 - 110) миллисекунд***  
**Скорость с партициями**: ~~ 100 миллисекунд  
**Скорость с партициями и индексам fullname**: ~~ 90 миллисекунд  
**Скорость с партициями и индексам gender**: ~~ 90 миллисекунд  
**Скорость с партициями и индексам fullname и gender**: ~~ 90 миллисекунд  

Всё время выполнения - на моей локальной машине. При тесте на сервере время
выполнения запросов с left(fullname, 1) = 'F' в некоторых случаях снижалось до
40 миллисекунд, а с LIKE оставалось примерно таким же.
