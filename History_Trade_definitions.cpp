#ifndef _HISTORY_TRADE_CPP_
#define _HISTORY_TRADE_CPP_

#include <cassert>
#include <iomanip>
#include <iostream>

#include "corehelper.hpp"
#include "History.hpp"
#include "Trades.hpp"

// DEFINITIONS FOR TRADE CLASS

// Constructor
// While "this" was not needed, I am told it is good practice to be explicit!

Trade::Trade( std::string ticker_symbol,  unsigned int day_date,
        unsigned int month_date,  unsigned year_date,
        bool buy_sell_trans,  unsigned int number_shares,
        double trans_amount ) {

	this->symbol = ticker_symbol;
	this->day = day_date;
	this->month = month_date;
	this->year = year_date;
	this->shares = number_shares;
	this->amount = trans_amount;

	this->acb = 0;
	this->acb_per_share = 0;
	this->share_balance = 0;
	this->cgl = 0;

	if (buy_sell_trans == true) {
		trans_type = "Buy";
	}
	else {
		trans_type = "Sell";
	}

	trans_id = assigned_trans_id;
	assigned_trans_id++;

	p_next = nullptr;
}

// Destructor: this could have just not been defined as the default
Trade::~Trade() {

}

// Overloaded < operator to check if a date is earlier than another
bool Trade::operator<( Trade const &other ) {
	if (year < other.year ) {
		return true;
	}
	else if ( (month < other.month) and (other.year == year) ) {
		return true;
	}
	else if ((day < other.day) and (other.month == month) and (other.year == year)) {
		return true;
	}
	return false;
}

// Member functions to get values.
std::string Trade::get_symbol() const { return symbol; }
unsigned int Trade::get_day() const { return day; }
unsigned int Trade::get_month() const { return month; }
unsigned int Trade::get_year() const { return year; }
unsigned int Trade::get_shares() const { return shares; }
double Trade::get_amount() const { return amount; }
double Trade::get_acb() const { return acb; }
double Trade::get_acb_per_share() const { return acb_per_share; }
unsigned int Trade::get_share_balance() const { return share_balance; }
double Trade::get_cgl() const { return cgl; }
bool Trade::get_trans_type() const { return (trans_type == "Buy") ? true: false ; }
unsigned int Trade::get_trans_id() const { return trans_id; }
Trade *Trade::get_next() { return p_next; }

// Member functions to set values.
void Trade::set_acb( double acb_value ) { acb = acb_value; }
void Trade::set_acb_per_share( double acb_share_value ) { acb_per_share = acb_share_value; }
void Trade::set_share_balance( unsigned int bal ) { share_balance = bal ; }
void Trade::set_cgl( double value ) { cgl = value; }
void Trade::set_next( Trade *p_new_next ) { p_next = p_new_next; }

// Print the table of trades
void Trade::print() {
  std::cout << std::fixed << std::setprecision(2);
  std::cout << std::setw(4) << get_trans_id() << " "
    << std::setw(4) << get_symbol() << " "
    << std::setw(4) << get_day() << " "
    << std::setw(4) << get_month() << " "
    << std::setw(4) << get_year() << " ";


  if ( get_trans_type() ) {
    std::cout << "  Buy  ";
  } else { std::cout << "  Sell "; }

  std::cout << std::setw(4) << get_shares() << " "
    << std::setw(10) << get_amount() << " "
    << std::setw(10) << get_acb() << " " << std::setw(4) << get_share_balance() << " "
    << std::setw(10) << std::setprecision(3) << get_acb_per_share() << " "
    << std::setw(10) << std::setprecision(3) << get_cgl()
    << std::endl;
}

// HISTORY CLASS DEFINITIONS

// Constructor
History::History() {
	p_head = nullptr;
}

// Destructor
History::~History()
{
    Trade *p_temp = nullptr;

    while(p_head != nullptr && p_head->get_next() != nullptr)
    {
        p_temp = p_head;
        p_head = p_head->get_next();
        p_temp->set_next(nullptr);
        delete p_temp;
        p_temp = nullptr;
    }
    delete p_head;
    p_head = nullptr;
}

// Read the trade history from file.
void History::read_history() {

	core::open_file();

	while (core::next_trans_entry()) {
		Trade * p_new = new Trade (

			core::get_trans_symbol(),
			core::get_trans_day(),
			core::get_trans_month(),
			core::get_trans_year(),
			core::get_trans_type(),
			core::get_trans_shares(),
			core::get_trans_amount()

		);

		this->insert(p_new);
	}

	core::close_file();

}


// Insert trade into linked list.
void History::insert(Trade *p_new_trans) {

	if (p_head == nullptr) {
		//Empty case
		p_head = p_new_trans;
	}
	else {
		//General case
		Trade * p_traverse {p_head};

		while (p_traverse->get_next() != nullptr) {
				p_traverse = p_traverse->get_next();
			}

			p_traverse->set_next(p_new_trans);
			p_new_trans->set_next(nullptr);
	}
}

// sort_by_date(): Sort the linked list by trade date using insert sort. Inefficient but simple sort method.
// I've also tried binary search and bubble sort methods for fun, but only insert sort is shown here.
// To fully understand the pointer and linked list logic, there is a bit of inefficiency in the code I'll revisit one day.

void History::sort_by_date() {

	// Check if list is empty or contains one node.
	if (p_head == nullptr or p_head->get_next() == nullptr) {
	}
	else {

		// Create handling pointers:
		Trade * p_sorted {nullptr};
		Trade * p_temp2 {nullptr};
		Trade * p_temp1 {p_head};

		// 1a. Move p_head to the second node
		p_head = p_head->get_next();

		// 1b. Disconnect first node from p_head list
		p_temp1->set_next(nullptr);

		// 1c. Point p_sorted to first node
		p_sorted = p_temp1;

		// Main loop for general case
		while (p_head != nullptr) {

			// 2a: Break off the next node.
			p_temp1 = p_head;
			p_head = p_head->get_next();
			p_temp1->set_next(nullptr);

			// 2b: Test whether that node goes to the head of the list or NOT.
			if (*p_temp1 < *p_sorted) {

				// Node to head of list
				p_temp1->set_next(p_sorted);
				p_sorted = p_temp1;
			}

			else {
				// Node NOT to head of list
				p_temp2 = p_sorted;

				// Find where the node should be sorted to
				while ((p_temp2->get_next() != nullptr) && !(*p_temp1 < *(p_temp2->get_next()))) {
					p_temp2 = p_temp2->get_next();
				}

				// Insert the new node
				p_temp1->set_next(p_temp2->get_next());

				p_temp2->set_next(p_temp1);
			}
		}

		p_head = p_sorted;

	}
}

/*
Merge Sort Exploration:

void History::sort_by_date(Trade ** p_head) {

	struct Trade * p_temp1{nullptr};
	struct Trade * p_temp2{nullptr};

	// Empty case: list is empty or contains one node
	if ((p_head == nullptr) || (p_head->get_next() == nullptr)) {
		return;
	}

	// Split list into two sublists
	split(p_head, &p_temp1, &p_temp2);

	// Recursive sorting of sublists (new halves)
	sort_by_date(&p_temp1);
	mergeSort(&p_temp2);

	// Merge two lists together
	p_head = sortedMerge(p_temp1, p_temp2)

	// INCOMPLETE: Split helper function.

	//

	// INCOMPLETE: Insert node at beginning of linked list. Push helper function.
	struct Trade * p_new = (struct Trade *)




 */


// Updates the ACB, Share, ACB/Share, and CGL values.
void History::update_acb_cgl() {

		Trade * p_temp {p_head};
		double my_acb = 0, my_acbS = 0, my_shareB = 0, my_cgl = 0, prev_acbS = 0;

		while (p_temp != nullptr) {

			//Buy calculations
			if (p_temp->get_trans_type()) {

				my_acb += p_temp->get_amount();
				my_shareB += p_temp->get_shares();
				my_acbS = ( my_acb / my_shareB );

				p_temp->set_acb(my_acb);
				p_temp->set_share_balance(my_shareB);
				p_temp->set_acb_per_share(my_acbS);
			}

			//Sell calculations
			else {
				my_acb -= (p_temp->get_shares() * prev_acbS);
				my_shareB -= p_temp->get_shares();
				my_acbS = ( my_acb / my_shareB);

				p_temp->set_acb(my_acb);
				p_temp->set_share_balance(my_shareB);
				p_temp->set_acb_per_share(my_acbS);

				//CGL set
				my_cgl = ( p_temp->get_amount() - ( p_temp->get_shares() * prev_acbS ));
				p_temp->set_cgl( my_cgl );
			}

			prev_acbS = p_temp->get_acb_per_share();
			p_temp = p_temp->get_next();
		}
}


// Compute a yearly CGL (same as the single CGL's calculated above but over a year)
// Could probably have made it another helper function

double History::compute_cgl(unsigned int year) {

	Trade * p_temp {p_head};
	double prev_acbS = 0, my_cgl = 0;

	while (p_temp != nullptr) {

		// Only summed if trade is a Sell type, in our year.
		if ((p_temp->get_trans_type() == false) and (p_temp->get_year() == year)) {

			my_cgl += ( p_temp->get_amount() - ( p_temp->get_shares() * prev_acbS) );

		}

		prev_acbS = p_temp->get_acb_per_share();

		p_temp = p_temp->get_next();
	}
	return my_cgl;
}

// Setup to print the trade history.
void History::print() {

	Trade * p_temp {p_head};
	std::cout << "========== BEGIN TRADE HISTORY ============" << std::endl;
	// NOT p_temp->get_next() because then it would stop one short
	while (p_temp != nullptr) {
		p_temp->print();
		p_temp = p_temp->get_next();
	}
	std::cout << "========== END TRADE HISTORY ============" << std::endl;
}


// Full access to the linked list, starting at p_head
Trade *History::get_p_head() { return p_head; }


#endif
