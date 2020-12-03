#include <iostream>
#include "History.hpp"
#include "Trades.hpp"

// This file needs to be edited for trade histories of different years

unsigned int Trade::assigned_trans_id = 0;
int main() {
  History trans_history{};
  trans_history.read_history();

  std::cout << "[Starting history]:" << std::endl;
  trans_history.print();
  trans_history.sort_by_date();

  std::cout << "[Sorted          ]:" << std::endl;
  trans_history.print();

  trans_history.update_acb_cgl();
  trans_history.print();

  std::cout << "[CGL for 2019    ]: " << trans_history.compute_cgl(2019) << std::endl;
  std::cout << "[CGL for 2020    ]: " << trans_history.compute_cgl(2020) << std::endl;

  return 0;
}
