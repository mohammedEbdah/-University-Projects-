// DataStructureAssignment02.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "ctime"
#include "iostream"

using namespace std;

int getCurrentTime() {
	time_t current_time;

	time(&current_time);

	int seconds = current_time % 86400;
	int minutes = seconds / 60;

	return minutes;
}

struct reservationType
{
	//Define member variables
};

struct nodeType
{
public:
	reservationType* reservation;
	nodeType* link;
};

class parkingHashTable {
private:
	int length;
	double pricePerHour;
	double totalAmount;
	//Define member variables

public:
	parkingHashTable(double price) {
		pricePerHour = price;
	}

	//Define member functions
};

int main()
{
	double price;

	cout << "****** WELCOME TO MY PARKING ******" << endl << endl;
	cout << "Enter the Parking Price Per Hour" << endl;

	cin >> price;

	parkingHashTable parking(price);

	while (true) {
		cout << "Please choose one of the following options?" << endl;

		cout << "1. Check In Car" << endl;
		cout << "2. Print Parking Slot Info" << endl;
		cout << "3. Checkout Car" << endl;
		cout << "4. Print Total Income" << endl;
		cout << "5. Print Cars in Parking" << endl;

		int option;

		cout << "Enter the option now (E.g. 3) ";

		cin >> option;

		switch (option) {
		case 1:
			//Check In Car
			break;
		case 2:
			//Print Parking Slot Info
			break;
		case 3:
			//Checkout Car
			break;
		case 4:
			//Print Total Income
			break;
		case 5:
			//Print Cars in Parking
			break;
		default:
			cout << "Selected option is not available!!!!";
		}
	}

	return 0;
}