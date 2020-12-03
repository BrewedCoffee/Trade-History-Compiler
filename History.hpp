#ifndef _HISTORY_HPP_
#define _HISTORY_HPP_

#include <iomanip>
#include <iostream>
#include <string>

#include "Trades.hpp"

class History {
public:
	// Constructor
	History();

	// Destructor.
	~History();

	// Read the trade history from file.
	void read_history();

	// Insert trade in linked list.
	void insert(Trade *p_new_trans);

	// Sort using insertion sort.
	void sort_by_date();

	// Update the ACB members in the linked list.
	void update_acb_cgl();

	// Compute the ACB, and CGL.
	double compute_cgl(unsigned int year);

	// Print the trade history.
	void print();

	Trade *get_p_head();

private:
	Trade *p_head;
};

#endif
