#pragma once

#include <string>
#include <vector>

#include "EmployeeModel.h"

namespace PseudoRandomEmployee {

static const std::vector<std::string> surnames_female = {
    "Anderson", "Brown",    "Carter",    "Davis",   "Evans",  "Fisher",
    "Green",    "Harris",   "Irwin",     "Johnson", "Keller", "Lewis",
    "Morgan",   "Nelson",   "Owens",     "Parker",  "Quinn",  "Roberts",
    "Smith",    "Taylor",   "Underwood", "Vaughn",  "White",  "Xavier",
    "Young",    "Zimmerman"};

static const std::vector<std::string> surnames_male = {
    "Adams",  "Baker",  "Clark",   "Dixon", "Edwards", "Foster", "Graham",
    "Hill",   "Ingram", "Jackson", "King",  "Lee",     "Miller", "Newman",
    "Oliver", "Powell", "Quinn",   "Reed",  "Scott",   "Turner", "Upton",
    "Vance",  "Walker", "Xander",  "York",  "Zane"};

static const std::vector<std::string> names_female = {
    "Sofia",      "Anastasia", "Victoria", "Ksenia", "Arina",
    "Yelizaveta", "Adelina",   "Irina",    "Yelena", "Polina"};

static const std::vector<std::string> names_male = {
    "Artyom", "Aleksandr", "Roman",  "Yevgeny", "Ivan",
    "Maksim", "Denis",     "Alexey", "Dmitry",  "Danyl"};

static const std::vector<std::string> patronymics_female = {
    "Aleksandrovna", "Dmitrievna", "Sergeevna", "Ivanovna", "Andreevna",
    "Nikolaevna",    "Viktorovna", "Pavlovna",  "Olegovna", "Yurievna"};

static const std::vector<std::string> patronymics_male = {
    "Aleksandrovich", "Dmitrievich", "Sergeevich", "Ivanovich", "Andreevich",
    "Nikolaevich",    "Viktorovich", "Pavlovich",  "Olegovich", "Yurievich"};

static const uint64_t RANDOM_NAME_COUNT =
    surnames_male.size() * names_male.size() * patronymics_male.size();

inline std::string getFullname(int &, int &, int &, bool);
inline std::chrono::year_month_day getBirthDate(int &);

std::vector<ptmk::EmployeeModel> getEmployeeVector();

} // namespace PseudoRandomEmployee
