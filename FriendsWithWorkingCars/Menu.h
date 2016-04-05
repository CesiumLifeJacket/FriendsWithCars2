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

int menu() {
	// cout and cin --> display abstraction
	int ans;
	do {
		display("What would you like to do?", true);
		display("1) Plan road trip", true);
		display("2) Plan break trip", true);
		display("3) Plan local trip", true);
		//<< "4) Manage friends" << endl; //
		cin >> ans;
	} while (ans > 3 || ans < 1);
	return ans;
}

string cityPrompt(int calltype, Database * friends) {
	vector<string> cities = friends->getCities();
	int ans;
	stringstream temp;
	if (calltype == 0)
	{
		display( "what city are you in?" , true);
	}
	else
		display("What city are you going to?", true);
	
	//list of entered friends.cities
	
	for (int i = 0; i < cities.size(); i++)
	{
		temp.str("");
		temp << i+1 << ") " << cities[i] << endl;
		display(temp.str(), false);
	}
	cin.ignore();
	cin.clear();
	cin >> ans;

	return cities[ans-1];
}

string timeOfYearPrompt(int calltype) {
	int time;
	string timeOfYr;
	if (calltype == 0) { //time of year we are in now
						 //display
		display("What time of year is it?", true);
	}
	else {
		display("What time of year will it be?", true);
	}
	do { //can change this to allow string entry, check string in while
		display("1) School", true);
		display("2) Summer", true);
		display( "3) Break", true);
		cin >> time;
	} while (time <1 || time > 3);
	switch (time)
	{
	case 1:
		timeOfYr = "SCHOOL";
		break;
	case 2:
		timeOfYr = "SUMMER";
		break;
	case 3:
		timeOfYr = "BREAK";
		break;
	default:
		break;
	}
	return timeOfYr;
}

string dayPrompt() {
	int ans;
	string day;
	stringstream temp;
	display("What day would you like to look at?", true);
	do {
		temp << "1) Sunday" << endl
			<< "2) Monday" << endl
			<< "3) Tuesday" << endl
			<< "4) Wednesday" << endl
			<< "5) Thursday" << endl
			<< "6) Friday" << endl
			<< "7) Saturday" << endl
			<< "8) General availability" << endl;
		display(temp.str(), false);
		cin >> ans;
	} while (ans < 1 || ans >8);

	switch (ans)
	{
	case 1:
		day = "SUN";
		break;
	case 2:
		day = "MON";
		break;
	case 3:
		day = "TUES";
		break;
	case 4:
		day = "WED";
		break;
	case 5:
		day = "THU";
		break;
	case 6:
		day = "FRI";
		break;
	case 7:
		day = "SAT";
		break;
	case 8:
		day = "GEN";
		break;
	default:
		break;
	}

	return day;
}


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

	int show;
	stringstream ss;
	stringstream ssTemp;
	bool expandArea = false;


	int refine;
	ssTemp << "would you like to refine the search?" << endl
		<< "1) Yes" << endl
		<< "2) No" << endl;
	display(ssTemp.str(), false);
	refine = boolIn();

	//min miles limit
	if (refine == 1)
	{
		display("How many miles will you be traveling?", false);
		int intmiles;
		cin >> intmiles;

		ss << intmiles; //using stringstream to convert to string
		miles = ss.str();
		ss.str(""); //clear stringstream
	}

	display("Would you like to display miles limits?", true);
	ssTemp.str("");
	ssTemp << "1) Yes" << endl << "2) No" << endl;
	display(ssTemp.str(), false);
	showMilesLimit = boolIn();

	//only show drivers that will not ask for gas money for a trip
	if (refine == 1) {
		int gasDrivers;
		display( "Would you like to only show drivers that will not ask for gas money on a trip?", true);
		do {
			display(ssTemp.str(), false); //is still the same prompt as above
			cin >> gasDrivers;
		} while (gasDrivers < 1 || gasDrivers > 2);
		if (gasDrivers == 1) {
			gasMoneyTrip = true;
			showGasMoneyTrip = false; //assuming user does not want column of 'yes'
		}
	}
	if (showGasMoneyTrip) { //if we haven't already changed showGasMoneyTrip
		display("Would you like to display whether drivers may ask for gas money?", true);
		display(ssTemp.str(), false); //still the same boolean prompt
		showGasMoneyTrip = boolIn();
	}

	//min gas economy
	if (refine == 1) {
		int gasEcon;
		display( "Enter a minimum gas economy (Enter 0 if you don't care)", true);
		cin >> gasEcon;

		ss << gasEcon;
		gasEconomy = ss.str();
		ss.str("");
	}
	display( "Would you like to display Gas Economy?", true);
	display(ssTemp.str(), false); //still the same boolean prompt
	showGasEcon = boolIn();

	//min reliability
	if (refine == 1) {
		int intreliability;
		display( "Enter a min reliability (Enter 0 if you don't care", true);
		cin >> intreliability;

		ss << intreliability;
		reliability = ss.str();
		ss.str("");
	}

	display( "Would you like to display Reliability?", true);
	display(ssTemp.str(), false); //still the same boolean prompt

	showReliability = boolIn();

	//min seatbelts
	if (refine == 1) {
		int seat;
		display( "Enter a minimum number of seatbelts (Including the driver)", true );
		cin >> seat;

		ss << seat;
		seatbelts = ss.str();
		ss.str("");
	}

	display( "Would you like to display number of seatbelts?", true);
	display(ssTemp.str(), false); //still the same boolean prompt
	showSeatbelts = boolIn();

	//only show can sofa
	int intSofa;
	if (refine == 1) {
		display( "Would you like to only show drivers who can move a sofa?", true);
		display(ssTemp.str(), false); //still the same boolean prompt
		cin >> intSofa;
		careSofa = boolIn();
		if (careSofa)
			showSofa = false;
		}
	

	if (showSofa) {
		display("Would you like to display sofa-bility?", true);
		display(ssTemp.str(), false); //still the same boolean prompt
		
		showSofa = boolIn();
	}

	//only show w/ AAA
	if (refine == 1) {
		int aaa;
		display( "Would you like to only show drivers with AAA?", true);
		display(ssTemp.str(), false); //still the same boolean prompt
		cin >> aaa;

		if (aaa == 1) {
			CareIfDriverHasAAA = true;
			ShowdriverHasAAA = false;
		}
	}
	if (ShowdriverHasAAA) {

		display("Would you like to display AAA status?", true);
		display(ssTemp.str(), false); //still the same boolean prompt
		cin >> show;

		if (show == 1)
			ShowdriverHasAAA = true;
		else
			ShowdriverHasAAA = false;
	}

	//cansnow
	if (refine == 1) {
		int snow;
		display( "Would you like to only show snow drivers?", true);
		display(ssTemp.str(), false); //still the same boolean prompt
		cin >> snow;
		if (snow == 1)
		{
			careAboutSnow = true;
			showCanSnow = false;
		}
	}
	if (showCanSnow) {
		display( "Would you like to show whether drivers can drive on snow?", true);
		display(ssTemp.str(), false); //still the same boolean prompt
		cin >> show;
		if (show == 1)
			showCanSnow = true;
		else
			showCanSnow = false;
	}

	//type
	vector<string> carTypes = friends->getCarTypes();
	if (refine == 1) {
		int chooseType;
		cout << "Select car types you would like to view: " << endl;
		for (int i = 0; i < carTypes.size(); i++)
		{
			ss.str("");
			ss << i + 1 << ") " << carTypes[i] << endl;
			display(ss.str(), false); 
		}
		ss.str("");
		ss << carTypes.size() + 1 << ") Any" << endl;
		display(ss.str(), false);
		do {
			cin >> chooseType;
		} while (chooseType <1 || chooseType > carTypes.size() + 1);
		if (chooseType < carTypes.size() + 1 ) //user chose a specific car type
		{
			type = carTypes[chooseType - 1];
			showType = false;
		}
		else type = "*";
	}

	if (showType) {
		display("Would you like to display car types?", true);
		display(ssTemp.str(), false); //still the same boolean prompt
		cin >> show;
		if (show == 1)
			showType = true;
		else
			showType = false;
	}

	Query roadTrip(false, showGasMoneyTrip, ShowdriverHasAAA, showMilesLimit, false, false, false, showSofa, showCanSnow, showGasEcon, showReliability, showSeatbelts,
		false, false, false,
		city, timeOfYear, "*", "*", "*", gasEconomy, reliability, seatbelts, type, careSofa, CareIfDriverHasAAA, miles, false, gasMoneyTrip, careAboutSnow);
	roadTrip.buildQuery();
	roadTrip.runQuery(friends);

	stringstream boolPrompt;
	int expand;
	display( "Would you like to expand your search to the area?", true);
	boolPrompt << "1) Yes" << endl << "2) No" << endl;
	display(boolPrompt.str(), false);
	cin >> expand;
	if (expand == 1)
	{
		roadTrip.expandToArea();
		roadTrip.buildQuery();
		roadTrip.runQuery(friends);
	}

}
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
	stringstream ss;
	stringstream boolPrompt;
	bool expandArea = false;


	int refine;
	display("would you like to refine the search?", true);
	boolPrompt << "1) Yes" << endl << "2) No" << endl;
	display(boolPrompt.str(), false);
	cin >> refine;


	//gas money
	if (refine == 1) {
		int gasDrivers;
		display("Would you like to only show drivers that will not ask for gas money on a trip?", true);
		do {
			display(boolPrompt.str(), false);
			cin >> gasDrivers;
		} while (gasDrivers != 1 && gasDrivers != 2);
		if (gasDrivers == 1) {
			gasMoneyTrip = true;
			showGasMoneyTrip = false; //assuming user does not want column of 'yes'
		}
	}
	if (showGasMoneyTrip) {
		display( "Would you like to display whether drivers may ask for gas money?", true);
		display(boolPrompt.str(), false);
		cin >> show;
		if (show == 1)
			showGasMoneyTrip = true;
		else
			showGasMoneyTrip = false;
	}

	//min gas economy
	if (refine == 1) {
		int gasEcon;
		display("Enter a minimum gas economy (Enter 0 if you don't care)", true);
		cin >> gasEcon;

		ss << gasEcon;
		gasEconomy = ss.str();
		ss.str("");
	}
	display("Would you like to display Gas Economy?", true);
	display(boolPrompt.str(), false);
	cin >> show;
	if (show == 1)
		showGasEcon = true;
	else
		showGasEcon = false;

	//min reliability
	if (refine == 1) {
		int intreliability;
		display( "Enter a min reliability (Enter 0 if you don't care", true);
		cin >> intreliability;

		ss << intreliability;
		reliability = ss.str();
		ss.str("");
	}

	display("Would you like to display Reliability?", true);
	display(boolPrompt.str(), false);
	cin >> show;

	if (show == 1)
		showReliability = true;
	else
		showReliability = false;

	//min seatbelts
	if (refine == 1) {
		int seat;
		display("Enter a minimum number of seatbelts (Including the driver)", true);
		cin >> seat;

		ss << seat;
		seatbelts = ss.str();
		ss.str("");
	}

	display("Would you like to display number of seatbelts?", true);
	display(boolPrompt.str(), false);
	cin >> show;

	if (show == 1)
		showSeatbelts = true;
	else
		showSeatbelts = false;

	//only show can sofa
	if (refine == 1) {
		int intSofa;
		display( "Would you like to only show drivers who can move a sofa?", true);
		display(boolPrompt.str(), false);
		cin >> intSofa;

		if (intSofa == 1) {
			careSofa = true;
			showSofa = false;
		}
	}
	if (showSofa) {
		display("Would you like to display sofa-bility?", true);
		display(boolPrompt.str(), false);
		cin >> show;

		if (show == 1)
			showSofa = true;
		else
			showSofa = false;
	}

	//only show w/ AAA
	if (refine == 1) {
		int aaa;
		display("Would you like to only show drivers with AAA?", true);
		display(boolPrompt.str(), false);
		cin >> aaa;

		if (aaa == 1) {
			CareIfDriverHasAAA = true;
			ShowdriverHasAAA = false;
		}
	}
	if (ShowdriverHasAAA) {

		display("Would you like to display AAA status?", true);
		display(boolPrompt.str(), false);
		cin >> show;

		if (show == 1)
			ShowdriverHasAAA = true;
		else
			ShowdriverHasAAA = false;
	}

	//cansnow
	if (refine == 1) {
		int snow;
		display( "Would you like to only show snow drivers?", true);
		display(boolPrompt.str(), false);
		cin >> snow;
		if (snow == 1)
		{
			careAboutSnow = true;
			showCanSnow = false;
		}
	}
	if (showCanSnow) {
		display("Would you like to show whether drivers can drive on snow?", true);
		display(boolPrompt.str(), false);
		cin >> show;
		if (show == 1)
			showCanSnow = true;
		else
			showCanSnow = false;
	}

	//type
	vector<string> carTypes = friends->getCarTypes();
	if (refine == 1) {
		int chooseType;
		display( "Select car types you would like to view: ", true);
		for (int i = 0; i < carTypes.size(); i++)
		{
			ss.str(""); //laura clears the string stream before using it
			ss << i + 1 << ") " << carTypes[i] << endl;
			display(ss.str(), false);
			ss.str(""); //kay clears it after
		}
		ss.str("");
		ss << carTypes.size() + 1 << ") Any" << endl;
		display(ss.str(), false);
		ss.str("");
		do {
			cin >> chooseType;
		} while (chooseType <1 || chooseType > carTypes.size() + 1);
		if (chooseType < carTypes.size() + 1) //user chose a specific car type
		{
			type = carTypes[chooseType - 1];
			showType = false;
		}
		else type = "*";
	}

	if (showType) {
		display( "Would you like to display car types?", true);
		display(boolPrompt.str(), false);
		cin >> show;
		if (show == 1)
			showType = true;
		else
			showType = false;
	}

	Query breakTrip(false, showGasMoneyTrip, ShowdriverHasAAA, false, false, false, false, showSofa, showCanSnow, showGasEcon, showReliability, showSeatbelts,
		true, false, false,
		fromCity, fromTime, toCity, toTime, "*", gasEconomy, reliability, seatbelts, type, careSofa, CareIfDriverHasAAA, "0", false, gasMoneyTrip, careAboutSnow);
	breakTrip.buildQuery();
	breakTrip.runQuery(friends);

	int expand;
	display("Would you like to expand your search to the area?", true);
	display(boolPrompt.str(), false);
	cin >> expand;
	if (expand == 1)
	{
		breakTrip.expandToArea();
		breakTrip.buildQuery();
		breakTrip.runQuery(friends);
	}


}
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
	stringstream boolPrompt;
	boolPrompt << "1) Yes" << endl << "2) No" << endl;

	bool expandArea = false;


	int refine;
	display( "would you like to refine the search?", true);
	display(boolPrompt.str(), false);
	cin >> refine;

	//min miles limit
	if (refine == 1)
	{
		display( "How many miles will you be traveling? Enter 0 for very short trip", true);
		int intmiles;
		cin >> intmiles;

		ss << intmiles; //using stringstream to convert to string
		miles = ss.str();
		ss.str(""); //clear stringstream
	}

	display("Would you like to display miles limits?", true);
	display(boolPrompt.str(), false);
	cin >> show;
	if (show == 1)
		showMilesLimit = true;
	else //accept no sass
		showMilesLimit = false;

	//only show drivers that will not ask for gas money for a local trip
	if (refine == 1) {
		int gasDrivers;
		display("Would you like to only show drivers that will not ask for gas money on a local trip?", true);
		do {
			display(boolPrompt.str(), false);
			cin >> gasDrivers;
		} while (gasDrivers != 1 && gasDrivers != 2);
		if (gasDrivers == 1) {
			gasMoneyLocal = true;
			showGasMoneyLocal = false; //assuming user does not want column of 'yes'
		}
	}
	if (showGasMoneyLocal) { //if we haven't already changed showGasMoneyLocal
		display("Would you like to display whether drivers may ask for gas money?", true);
		display(boolPrompt.str(), false);
		cin >> show;
		if (show == 1)
			showGasMoneyLocal = true;
		else
			showGasMoneyLocal = false;
	}

	//min seatbelts
	if (refine == 1) {
		int seat;
		display( "Enter a minimum number of seatbelts (Including the driver)", true);
		cin >> seat;

		ss << seat;
		seatbelts = ss.str();
		ss.str("");
	}

	display("Would you like to display number of seatbelts?", true);
	display(boolPrompt.str(), false);
	cin >> show;

	if (show == 1)
		showSeatbelts = true;
	else
		showSeatbelts = false;

	//only show can sofa
	if (refine == 1) {
		int intSofa;
		display("Would you like to only show drivers who can move a sofa?", true);
		display(boolPrompt.str(), false);
		cin >> intSofa;

		if (intSofa == 1) {
			careSofa = true;
			showSofa = false;
		}
	}
	if (showSofa) {
		display("Would you like to display sofa-bility?", true);
		display(boolPrompt.str(), false);
		cin >> show;

		if (show == 1)
			showSofa = true;
		else
			showSofa = false;
	}

	//cansnow
	if (refine == 1) {
		int snow;
		display("Would you like to only show snow drivers?", true);
		display(boolPrompt.str(), false);
		cin >> snow;
		if (snow == 1)
		{
			careAboutSnow = true;
			showCanSnow = false;
		}
	}
	if (showCanSnow) {
		display("Would you like to show whether drivers can drive on snow?", true);
		display(boolPrompt.str(), false);
		cin >> show;
		if (show == 1)
			showCanSnow = true;
		else
			showCanSnow = false;
	}

	//type
	vector<string> carTypes = friends->getCarTypes();
	ss.str("");
	if (refine == 1) {
		int chooseType;
		display("Select car types you would like to view: ", true);
		for (int i = 0; i < carTypes.size(); i++)
		{
			ss << i + 1 << ") " << carTypes[i] << endl;
			display(ss.str(), false);
		}
		ss.str("");
		ss << carTypes.size() + 1 << ") Any" << endl;
		display(ss.str(), false);
		do {
			cin >> chooseType;
		} while (chooseType <1 || chooseType > carTypes.size() + 1);
		if (chooseType < carTypes.size() + 1) //user chose a specific car type
		{
			type = carTypes[chooseType - 1];
			showType = false;
		}
		else type = "*";
	}

	if (showType) {
		display("Would you like to display car types?", true);
		display(boolPrompt.str(), false);
		cin >> show;
		if (show == 1)
			showType = true;
		else
			showType = false;
	}

	Query localTrip(showGasMoneyLocal, false, ShowdriverHasAAA, showMilesLimit, false, false, true, showSofa, showCanSnow, false, showReliability, showSeatbelts,
		false,true,false, 
		city, timeOfYear, "*", "*", day, "0", reliability, seatbelts, type, careSofa, CareIfDriverHasAAA, miles, gasMoneyLocal, false, careAboutSnow);

	//query ((show{[GasMoneyLocal], false, false, [miles limit], [city?], [area?], [availability], [canSofa], [canSnow],false, false} 
	//queryType{ false,true, [expand] }
	//Parameters{ [startCity],[StartTimeoy],end,end,[day],'0',[min seatbelts],[min reliability = 0],[type],[sofa],false,[miles],[local gas],false,[can snow] })

	int expand;
	display("Would you like to expand your search to the area?", true);
	display(boolPrompt.str(), false);
	cin >> expand;
	if (expand == 1)
	{
		localTrip.expandToArea();
		localTrip.buildQuery();
		localTrip.runQuery(friends);
	}

	int gen;
	display( "Would You like to see General Availability results?", true);
	display(boolPrompt.str(), false);
	cin >> gen;
	if (gen == 1) {
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
	int secondContact;
	string contact2 = "None";
	int milesLimit;
	int AAA;
	int gasMoneyLocal;
	int gasMoneyTrip;
	stringstream boolPrompt;

	display("What is your friend's name? ", true);
	cin.ignore();
	getline(cin, name);

	display("What is your friend's contact information? (Phone number or email)", true);
	getline(cin, contact1);
	display("Does your friend have any secondary contact information?", true);
	boolPrompt << "1) Yes" << endl << "2) No" << endl;
	display(boolPrompt.str(), false);
	cin >> secondContact;
	cin.ignore();
	if (secondContact == 1)
	{
		display("What is your friend's secondary contact information?", true);
		getline(cin, contact2);
	}

	display("Does your friend have AAA?", true);
	display(boolPrompt.str(), false);
	cin >> AAA;

	display("Does your friend ask for gas money for local trips?", true);
	display(boolPrompt.str(), false);
	cin >> gasMoneyLocal;

	display("Does your friend ask for gas money for longer trips?", true);
	display(boolPrompt.str(), false);
	cin >> gasMoneyTrip;

	//car
	//avaliability



	//modify friend
	//delete friend
	//add/delete/modify cars to people
	//add/delete/modify avaliability
}