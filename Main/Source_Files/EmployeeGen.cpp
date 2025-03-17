#include <chrono>
#include <string>
#include <vector>

#include "EmployeeGen.h"
#include "EmployeeModel.h"

namespace PseudoRandomEmployee {

std::string getFullname(int &patronymic, int &name, int &surname,
                        bool genderMale) {
  int curr_pat = patronymic++, curr_name = name, curr_sur = surname;
  if (patronymic == 10) {
    patronymic = 0;
    name++;
  }
  if (name == 10) {
    name = 0;
    surname = (surname + 1) % 26;
  }

  if (genderMale)
    return surnames_male[curr_sur] + " " + names_male[curr_name] + " " +
           patronymics_male[curr_pat];
  return surnames_female[curr_sur] + " " + names_female[curr_name] + " " +
         patronymics_female[curr_pat];
}
std::chrono::year_month_day getBirthDate(int &) {
  return std::chrono::year_month_day(
      std::chrono::year(2000), std::chrono::month(2), std::chrono::day(20));
}

std::vector<ptmk::EmployeeModel> getEmployeeVector() {
  std::vector<ptmk::EmployeeModel> result;
  int surname = 0, name = 0, patronymics = 0, birthDate = 0;
  for (uint64_t ind = 0; ind < RANDOM_NAME_COUNT; ind++)
    result.emplace_back(getFullname(patronymics, name, surname, true),
                        getBirthDate(birthDate), ptmk::EmployeeModel::MALE);
  return result;
}

} // namespace PseudoRandomEmployee
