#include <chrono>
#include <cstdint>
#include <string>
#include <vector>

#include "EmployeeGen.h"
#include "EmployeeModel.h"

// EmployeeGenerator public

std::vector<ptmk::EmployeeModel> EmployeeGenerator::getEmployeeVector1M() {
  std::vector<ptmk::EmployeeModel> result;
  result.reserve(UNIQUE_NAME_COUNT * 2);
  for (size_t ind = 0; ind < UNIQUE_NAME_COUNT; ind++) {
    result.emplace_back(getFullname(true), getBirthDate(),
                        ptmk::EmployeeModel::MALE);
    result.emplace_back(getFullname(false), getBirthDate(),
                        ptmk::EmployeeModel::FEMALE);
  }
  return result;
}

std::vector<ptmk::EmployeeModel> EmployeeGenerator::getEmployeeVectorMaleF() {
  size_t vectorSize = names_male.size() * patronymics_male.size();
  std::vector<ptmk::EmployeeModel> result;
  result.reserve(vectorSize);
  for (size_t ind = 0; ind < vectorSize; ind++) {
    result.emplace_back(getFullname(true, 5), getBirthDate(),
                        ptmk::EmployeeModel::MALE);
  }
  return result;
}

// EmployeeGenerator private

std::string EmployeeGenerator::getFullname(bool genderMale,
                                           size_t startSurname) {
  static size_t surnames = startSurname, names = 0, patronymics = 0;

  if (startSurname != 0) {
    surnames = startSurname;
    names = 0;
    patronymics = 0;
  }

  size_t curr_sur = surnames, curr_name = names, curr_pat = patronymics;
  if (genderMale)
    return surnames_male[curr_sur] + " " + names_male[curr_name] + " " +
           patronymics_male[curr_pat];

  if (++surnames == surnames_male.size()) {
    surnames = 0;
    names++;
  }
  if (names == names_male.size()) {
    names = 0;
    patronymics = (patronymics + 1) % patronymics_male.size();
  }
  return surnames_female[curr_sur] + " " + names_female[curr_name] + " " +
         patronymics_female[curr_pat];
}

std::chrono::year_month_day EmployeeGenerator::getBirthDate() {
  static uint16_t add = 0;

  uint16_t currAdd = add;
  add = (add + 1) % BIRTHDAY_COUNT;

  return {START_BIRTHDAY.year() + std::chrono::years(currAdd / 12),
          START_BIRTHDAY.month() + std::chrono::months(currAdd % 11),
          START_BIRTHDAY.day() + std::chrono::days(currAdd % 28)};
}
