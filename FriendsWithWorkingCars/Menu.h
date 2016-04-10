#include <iostream>
#include <string>
#include<sstream>
using namespace std;

void display(string msg, bool endline)
{
	if (endline)
		cout << msg << endl;
	else
		cout << msg;
}

bool boolIn() {
	int ans;
	do {
		cin >> ans;
	} while (ans < 1 || ans >2);

	if (ans == 1)
		return true;
	else
		return false;

}

bool is_number(string s) {
	for (int i = 0; i < s.length(); i++)
		if (!isdigit(s[i]))
			return false;

	return true;
}

template<typename T>
T menuPrompt(string prompt, vector<string> options, vector<T> outputs) {
	if (options.size() != outputs.size()) {
		// TODO: make this a real exception
		throw "Range mismatch AHHHH";
	}

	int input;
	while (true) {
		string input_str;
		cout << prompt << endl;
		for (int i = 0; i < options.size(); i++) {
			cout << i + 1 << ") " << options[i] << endl;
		}
		cin >> input_str;

		if (is_number(input_str)) {
			input = stoi(input_str);
			if (input < 1 || input > options.size()) {
				cout << "input is out of range" << endl;
				continue;
			}
			break;
		}
		else {
			cout << "please enter an integer between 1 and " << options.size() << endl;
		}
	}
	return outputs[input - 1];
}

bool boolPrompt(string prompt) {
	vector<string> options = { "Yes", "No" };
	vector<bool> outputs = { true, false };
	return menuPrompt(prompt, options, outputs);
}

int intPrompt(string prompt, int min, int max) {
	while (true) {
		string in_str;
		cout << prompt << ' ';
		cin >> in_str;
		if (is_number(in_str)) {
			int in = stoi(in_str);
			if (in < min || in > max) {
				cout << "Please input an integer between " << min << " and " << max << endl;
			}
			else {
				return in;
			}
		}
	}
}

int menu() {
	vector<string> options = {
		"Plan road trip",
		"Plan break trip",
		"Plan local trip",
		"insert something??"
	};
	vector<int> outputs = { 1, 2, 3, 4};
	string prompt = "What would you like to do?";
	return menuPrompt(prompt, options, outputs);
}

string cityPrompt(int calltype, Database * friends) {
	vector<string> options = friends->getCities();
	vector<string> outputs = options;
	string prompt = calltype == 0 ?
		"What city are you in?" :
		"What city are you going to?";
	return menuPrompt(prompt, options, outputs);
}

string timeOfYearPrompt(int calltype) {
	vector<string> options = { "School", "Summer", "Break" };
	vector<string> outputs = { "SCHOOL", "SUMMER", "BREAK" };
	string prompt = calltype == 0 ?
		"What time of year is it?" :
		"What time of year will it be?";
	return menuPrompt(prompt, options, outputs);
}

string dayPrompt() {
	vector<string> options = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "General Availability" };
	vector<string> outputs = { "SUN", "MON", "TUES", "WED", "THU", "FRI", "SAT", "GEN" };
	string prompt = "What day would you like to look at?";
	return menuPrompt(prompt, outputs, options);
}

//TODO: Refactor prompting user for refinement
void roadTrip(Database *friends) {
	string city = cityPrompt(0, friends);
	string timeOfYear = timeOfYearPrompt(0);

	//refine the search
	//refine-able attributes, all set to default value which will not refine search
	string gasEconomy = "0";
	string reliability = "0";
	string seatbelts = "0";
	bool careSofa = false;
	bool CareIfDriverHasAAA = false; //if we care if the driver has AAA, we only care that they /do/ have AAA
	string type = "*";
	string miles = "0";
	bool gasMoneyTrip = false;
	bool careAboutSnow = false;
	//do we want to show this attribute? default to show all
	bool showGasEcon = true;
	bool showReliability = true;
	bool showSeatbelts = true;
	bool showSofa = true;
	bool ShowdriverHasAAA = true;
	bool showType = true;
	bool showMilesLimit = true;
	bool showGasMoneyTrip = true;
	bool showCanSnow = true;

	stringstream ss;
	stringstream ssTemp;
	bool expandArea = false;


	bool refine = boolPrompt("would you like to refine the search?");

	//min miles limit
	if (refine)
	{
		display("How many miles will you be traveling?", false);
		int intmiles;
		cin >> intmiles;

		ss << intmiles; //using stringstream to convert to string
		miles = ss.str();
		ss.str(""); //clear stringstream
	}

	showMilesLimit = boolPrompt("Would you like to display miles limits?");

	//only show drivers that will not ask for gas money for a trip
	if (refine) {
		gasMoneyTrip = boolPrompt("Would you like to only show drivers that will not ask for gas money on a trip?");
		if (gasMoneyTrip) {
			showGasMoneyTrip = false; //assuming user does not want column of 'yes'
		}
	}
	if (showGasMoneyTrip) { //if we haven't already changed showGasMoneyTrip
		showGasMoneyTrip = boolPrompt("Would you like to display whether drivers may ask for gas money?");
	}

	//min gas economy
	if (refine) {
		string prompt = "Enter a minimum gas economy (Enter 0 if you don't care)";
		gasEconomy = to_string(intPrompt(prompt, 0, INT_MAX));
	}
	showGasEcon = boolPrompt("Would you like to display Gas Economy?");

	//min reliability
	if (refine) {
		string prompt = "Enter a min reliability (Enter 0 if you don't care)";
		reliability = to_string(intPrompt(prompt, 0, MAX_RELIABILITY));
	}

	showReliability = boolPrompt("Would you like to display Reliability?");

	//min seatbelts
	if (refine) {
		string prompt = "Enter a minimum number of seatbelts (Including the driver)";
		seatbelts = to_string(intPrompt(prompt, 0, INT_MAX));
	}

	showSeatbelts = boolPrompt("Would you like to display number of seatbelts?");

	//only show can sofa

	if (refine) {
		careSofa = boolPrompt("Would you like to only show drivers who can move a sofa?");
		if (careSofa)
			showSofa = false;
	}

	if (showSofa) {
		showSofa = boolPrompt("Would you like to display sofa-bility?");
	}

	//only show w/ AAA
	if (refine) {
		bool aaa = boolPrompt("Would you like to only show drivers with AAA?");
		if (aaa) {
			CareIfDriverHasAAA = true;
			ShowdriverHasAAA = false;
		}
	}
	if (ShowdriverHasAAA) {
		ShowdriverHasAAA = boolPrompt("Would you like to display AAA status?");
	}

	//cansnow
	if (refine) {
		bool snow = boolPrompt("Would you like to only show snow drivers?");
		if (snow)
		{
			careAboutSnow = true;
			showCanSnow = false;
		}
	}
	if (showCanSnow) {
		showCanSnow = boolPrompt("Would you like to show whether drivers can drive on snow?");
	}

	//type
	vector<string> carTypes = friends->getCarTypes();

	if (refine) {
		carTypes.push_back("Any");
		vector<string> outputs = friends->getCarTypes();
		outputs.push_back("*");
		string prompt = "Select car types you would like to view: ";
		type = menuPrompt(prompt, carTypes, outputs);
		if (type != "*")
			showType = false;
	}

	if (showType) {
		showType = boolPrompt("Would you like to display car types?");
	}

	Query roadTrip(false, showGasMoneyTrip, ShowdriverHasAAA, showMilesLimit, false, false, false, showSofa, showCanSnow, showGasEcon, showReliability, showSeatbelts,
		false, false, false,
		city, timeOfYear, "*", "*", "*", gasEconomy, reliability, seatbelts, type, careSofa, CareIfDriverHasAAA, miles, false, gasMoneyTrip, careAboutSnow);
	roadTrip.buildQuery();
	roadTrip.runQuery(friends);

	bool expand = boolPrompt("Would you like to expand your search to the area?");
	if (expand)
	{
		roadTrip.expandToArea();
		roadTrip.buildQuery();
		roadTrip.runQuery(friends);
	}

}
//TODO: Refactor prompting user for refinement
void breakTrip(Database *friends) {
	string fromCity = cityPrompt(0, friends); //where are we now
	string toCity = cityPrompt(1, friends); //where are we going
	string fromTime = timeOfYearPrompt(0); //what time period is it
	string toTime = timeOfYearPrompt(1); //what time period is it going to be
										 //TODO: would you like to refine the search?


										 //refine the search
										 //refine-able attributes, all set to default value which will not refine search
	string gasEconomy = "0";
	string reliability = "0";
	string seatbelts = "0";
	bool careSofa = false;
	bool CareIfDriverHasAAA = false; //if we care if the driver has AAA, we only care that they /do/ have AAA
	string type = "*";
	bool gasMoneyTrip = false;
	bool careAboutSnow = false;
	//do we want to show this attribute? default to show all
	bool showGasEcon = true;
	bool showReliability = true;
	bool showSeatbelts = true;
	bool showSofa = true;
	bool ShowdriverHasAAA = true;
	bool showType = true;
	bool showGasMoneyTrip = true;
	bool showCanSnow = true;

	int show;
	bool expandArea = false;


	bool refine = boolPrompt("Would you like to refine the search?");


	//gas money
	if (refine) {
		bool gasDrivers = boolPrompt("Would you like to only show drivers that will not ask for gas money on a trip?");
		if (gasDrivers) {
			gasMoneyTrip = true;
			showGasMoneyTrip = false; //assuming user does not want column of 'yes'
		}
	}
	if (showGasMoneyTrip) {
		showGasMoneyTrip = boolPrompt("Would you like to display whether drivers may ask for gas money?");
	}

	//min gas economy
	if (refine) {
		string prompt = "Enter a minimum gas economy (Enter 0 if you don't care)";
		gasEconomy = to_string(intPrompt(prompt, 0, INT_MAX));
	}
	showGasEcon = boolPrompt("Would you like to display Gas Economy?");

	//min reliability
	if (refine) {
		string prompt = "Enter a min reliability (Enter 0 if you don't care";
		reliability = to_string(intPrompt(prompt, 0, MAX_RELIABILITY));
	}

	showReliability = boolPrompt("Would you like to display Reliability?");

	//min seatbelts
	if (refine) {
		string prompt = "Enter a minimum number of seatbelts (Including the driver)";
		seatbelts = to_string(intPrompt(prompt, 0, INT_MAX));
	}

	showSeatbelts = boolPrompt("Would you like to display number of seatbelts?");

	//only show can sofa
	if (refine) {
		bool boolSofa = boolPrompt("Would you like to only show drivers who can move a sofa?");
		if (boolSofa) {
			careSofa = true;
			showSofa = false;
		}
	}
	if (showSofa) {
		showSofa = boolPrompt("Would you like to display sofa-bility?");
	}

	//only show w/ AAA
	if (refine) {
		bool aaa = boolPrompt("Would you like to only show drivers with AAA?");
		if (aaa) {
			CareIfDriverHasAAA = true;
			ShowdriverHasAAA = false;
		}
	}
	if (ShowdriverHasAAA) {
		ShowdriverHasAAA = boolPrompt("Would you like to display AAA status?");
	}

	//cansnow
	if (refine) {
		bool snow = boolPrompt("Would you like to only show snow drivers?");
		if (snow)
		{
			careAboutSnow = true;
			showCanSnow = false;
		}
	}
	if (showCanSnow) {
		showCanSnow = boolPrompt("Would you like to show whether drivers can drive on snow?");
	}

	//type
	if (refine) {
		vector<string> carTypes = friends->getCarTypes(), outputs = friends->getCarTypes();
		carTypes.push_back("Any");
		outputs.push_back("*");
		string prompt = "Select car types you would like to view: ";
		type = menuPrompt(prompt, carTypes, outputs);
		if (type != "*")
			showType = false;
	}

	if (showType) {
		showType = boolPrompt("Would you like to display car types?");
	}

	Query breakTrip(false, showGasMoneyTrip, ShowdriverHasAAA, false, false, false, false, showSofa, showCanSnow, showGasEcon, showReliability, showSeatbelts,
		true, false, false,
		fromCity, fromTime, toCity, toTime, "*", gasEconomy, reliability, seatbelts, type, careSofa, CareIfDriverHasAAA, "0", false, gasMoneyTrip, careAboutSnow);
	breakTrip.buildQuery();
	breakTrip.runQuery(friends);

	bool expand = boolPrompt("Would you like to expand your search to the area?");
	if (expand)
	{
		breakTrip.expandToArea();
		breakTrip.buildQuery();
		breakTrip.runQuery(friends);
	}


}
//TODO: Refactor prompting user for refinement
void localTrip(Database *friends) {
	string city = cityPrompt(0, friends);
	string timeOfYear = timeOfYearPrompt(0); //what part of year is it
	string day = dayPrompt();//what day



							 //refine the search
							 //refine-able attributes, all set to default value which will not refine search
	string reliability = "0";
	string seatbelts = "0";
	bool careSofa = false;
	bool CareIfDriverHasAAA = false; //if we care if the driver has AAA, we only care that they /do/ have AAA
	string type = "*";
	string miles = "0";
	bool gasMoneyLocal = false;
	bool careAboutSnow = false;
	//do we want to show this attribute? default to show all
	bool showReliability = true;
	bool showSeatbelts = true;
	bool showSofa = true;
	bool ShowdriverHasAAA = true;
	bool showType = true;
	bool showMilesLimit = true;
	bool showGasMoneyLocal = true;
	bool showCanSnow = true;

	int show;
	stringstream ss;

	bool expandArea = false;


	bool refine = boolPrompt("would you like to refine the search?");

	//min miles limit
	if (refine)
	{
		string prompt = "How many miles will you be traveling? Enter 0 for very short trip";
		miles = to_string(intPrompt(prompt, 0, INT_MAX));
	}

	showMilesLimit = boolPrompt("Would you like to display miles limits?"); //accept no sass

	//only show drivers that will not ask for gas money for a local trip
	if (refine) {
		bool gasDrivers = boolPrompt("Would you like to only show drivers that will not ask for gas money on a local trip?");
		if (gasDrivers) {
			gasMoneyLocal = true;
			showGasMoneyLocal = false; //assuming user does not want column of 'yes'
		}
	}
	if (showGasMoneyLocal) { //if we haven't already changed showGasMoneyLocal
		showGasMoneyLocal = boolPrompt("Would you like to display whether drivers may ask for gas money?");
	}

	//min seatbelts
	if (refine) {
		string prompt = "Enter a minimum number of seatbelts (Including the driver)";
		seatbelts = to_string(intPrompt(prompt, 0, INT_MAX));
	}

	showSeatbelts = boolPrompt("Would you like to display number of seatbelts?");

	//only show can sofa
	if (refine) {
		bool boolSofa = boolPrompt("Would you like to only show drivers who can move a sofa?");
		if (boolSofa) {
			careSofa = true;
			showSofa = false;
		}
	}
	if (showSofa) {
		showSofa = boolPrompt("Would you like to display sofa-bility?");
	}

	//cansnow
	if (refine) {
		bool snow = boolPrompt("Would you like to only show snow drivers?");
		if (snow)
		{
			careAboutSnow = true;
			showCanSnow = false;
		}
	}
	if (showCanSnow) {
		showCanSnow = boolPrompt("Would you like to show whether drivers can drive on snow?");
	}

	//type
	
	ss.str("");
	if (refine) {
		vector<string> carTypes = friends->getCarTypes(), outputs = friends->getCarTypes();
		carTypes.push_back("Any");
		outputs.push_back("*");
		string prompt = "Select car types you would like to view:";
		type = menuPrompt(prompt, carTypes, outputs);
		if (type != "*")
			showType = false;
	}

	if (showType) {
		showType = boolPrompt("Would you like to display car types?");
	}

	Query localTrip(showGasMoneyLocal, false, ShowdriverHasAAA, showMilesLimit, false, false, true, showSofa, showCanSnow, false, showReliability, showSeatbelts,
		false, true, false,
		city, timeOfYear, "*", "*", day, "0", reliability, seatbelts, type, careSofa, CareIfDriverHasAAA, miles, gasMoneyLocal, false, careAboutSnow);

	//query ((show{[GasMoneyLocal], false, false, [miles limit], [city?], [area?], [availability], [canSofa], [canSnow],false, false} 
	//queryType{ false,true, [expand] }
	//Parameters{ [startCity],[StartTimeoy],end,end,[day],'0',[min seatbelts],[min reliability = 0],[type],[sofa],false,[miles],[local gas],false,[can snow] })

	bool expand = boolPrompt("Would you like to expand your search to the area?");
	if (expand)
	{
		localTrip.expandToArea();
		localTrip.buildQuery();
		localTrip.runQuery(friends);
	}

	bool gen = boolPrompt("Would You like to see General Availability results?");
	if (gen) {
		localTrip.expandToGeneralAvailability();
		localTrip.buildQuery();
		localTrip.runQuery(friends);
	}



}
void manageFriend() {
	//would you like to add/modify/ delete friend/car/avaliability
	//insert friend
	string name;
	string contact1;
	bool secondContact;
	string contact2 = "None";
	int milesLimit;
	bool AAA;
	bool gasMoneyLocal;
	bool gasMoneyTrip;

	display("What is your friend's name? ", true);
	cin.ignore();
	getline(cin, name);

	display("What is your friend's contact information? (Phone number or email)", true);
	getline(cin, contact1);
	secondContact = boolPrompt("Does your friend have any secondary contact information?");
	if (secondContact)
	{
		display("What is your friend's secondary contact information?", true);
		getline(cin, contact2);
	}

	AAA = boolPrompt("Does your friend have AAA?");

	gasMoneyLocal = boolPrompt("Does your friend ask for gas money for local trips?");

	gasMoneyTrip = boolPrompt("Does your friend ask for gas money for longer trips?");

	//TODO: add car
	//TODO: avaliability



	//TODO: modify friend
	//TODO: delete friend
	//TODO: add/delete/modify cars to people
	//TODO: add/delete/modify avaliability
}