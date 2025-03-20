#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "EmployeeModel.h"

class EmployeeGenerator {
public:
  static std::vector<ptmk::EmployeeModel> getEmployeeVector1M();

  static std::vector<ptmk::EmployeeModel> getEmployeeVectorMaleF();

private:
  inline static const std::vector<std::string> surnames_female = {
      "Anderson", "Brown",     "Carter",    "Davis",    "Evans",   "Fisher",
      "Green",    "Harris",    "Irwin",     "Johnson",  "Keller",  "Lewis",
      "Morgan",   "Nelson",    "Owens",     "Parker",   "Quinn",   "Roberts",
      "Smith",    "Taylor",    "Underwood", "Vaughn",   "White",   "Xavier",
      "Young",    "Zimmerman", "Allen",     "Brooks",   "Collins", "Dawson",
      "Elliott",  "Fletcher",  "Griffin",   "Hughes",   "Iverson", "Jenkins",
      "Knight",   "Lawrence",  "Mitchell",  "Norris",   "Owen",    "Patterson",
      "Quinlan",  "Russell",   "Stewart",   "Thompson", "Ullman",  "Vance",
      "Watson",   "Xander",    "Yates",     "Zeller"};

  inline static const std::vector<std::string> surnames_male = {
      "Adams",    "Baker",  "Clark",    "Dixon",    "Edwards",   "Foster",
      "Graham",   "Hill",   "Ingram",   "Jackson",  "King",      "Lee",
      "Miller",   "Newman", "Oliver",   "Powell",   "Quinn",     "Reed",
      "Scott",    "Turner", "Upton",    "Vance",    "Walker",    "Xander",
      "York",     "Zane",   "Arnold",   "Bennett",  "Carson",    "Dawson",
      "Erickson", "Ford",   "Gibson",   "Harrison", "Ivy",       "Jennings",
      "Klein",    "Logan",  "Mason",    "Norton",   "Ortega",    "Peters",
      "Quincy",   "Rogers", "Sullivan", "Tucker",   "Underhill", "Vaughan",
      "Wells",    "Xavier", "Yoder",    "Zimmerman"};

  inline static const std::vector<std::string> names_female = {
      "Sofia",      "Anastasia", "Victoria", "Ksenia", "Arina",
      "Yelizaveta", "Adelina",   "Irina",    "Yelena", "Polina"};

  inline static const std::vector<std::string> names_male = {
      "Artyom", "Aleksandr", "Roman",  "Yevgeny", "Ivan",
      "Maksim", "Denis",     "Alexey", "Dmitry",  "Danyl"};

  inline static const std::vector<std::string> patronymics_female = {
      "Aleksandrovna", "Dmitrievna", "Sergeevna", "Ivanovna", "Andreevna",
      "Nikolaevna",    "Viktorovna", "Pavlovna",  "Olegovna", "Yurievna"};

  inline static const std::vector<std::string> patronymics_male = {
      "Aleksandrovich", "Dmitrievich", "Sergeevich", "Ivanovich", "Andreevich",
      "Nikolaevich",    "Viktorovich", "Pavlovich",  "Olegovich", "Yurievich"};

  inline static const uint8_t BIRTHDAY_COUNT = 97;
  inline static const uint8_t GENDER_COUNT = 2;
  inline static const uint64_t UNIQUE_NAME_COUNT =
      surnames_male.size() * names_male.size() * patronymics_male.size() *
      BIRTHDAY_COUNT;

  inline static const std::chrono::year_month_day START_BIRTHDAY = {
      std::chrono::year(1970), std::chrono::month(1), std::chrono::day(1)};

private:
  static std::string getFullname(bool, size_t = 0);
  static std::chrono::year_month_day getBirthDate();
};
