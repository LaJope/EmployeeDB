# [Проект на github](https://github.com/LaJope/EmployeeDB)

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
которые подходят по указанному в запросе критерию.
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
