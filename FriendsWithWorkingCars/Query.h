#ifndef QUERY_H
#define QUERY_H

#include <string>
#include <vector>
#include <sstream>
#include "Database.h"

using namespace std;


//TODO:
// add insert/modify/delete functionality


//constants for column widths
const int FULL_NAME_LENGTH = 20;
const int CONTACT = 10;
const int CONTACT2 = 15;
const int GAS_MONEY_LOCAL = 3;
const int GAS_MONEY_TRIP = 3;
const int AAAval = 3;
const int MILES_LIMIT = 4;
const int CITY = 15;
const int AREA = 10;
const int DAY = 3;
const int DESCRIPTIONval = 20;
const int CAN_MOVE_SOFA = 4;
const int CAN_SNOW = 4;
const int GAS_ECON = 3;
const int RELIABILITY = 11;
const int SEATBELTS = 9;

class Query {
public:

	Query(//these first three lines of booleans are for what is shown
		bool gasMoneyLocal, bool gasMoneyTrip, bool AAA, bool milesLimit,
		bool showCity, bool showArea, bool availability, bool canMoveSofa,
		bool canSnow, bool gasEconomy, bool reliability, bool seatbelts,

		//these booleans determine the query type
		bool cityChange, bool specificWeekDay, bool expandCityToArea,

		//these strings determine the search parameters, an * means we don't care 
		string startCity, string startTimeOfYear, string endCity, string endTimeOfYear, string aDay, string minGasEconomy, string minReliability, string minSeatbelts,
		string carType, bool needsToMoveSofa, bool driverIsAAAMember, string minMilesNeeded, bool doesNotNeedLocalGas, bool doesNotNeedTripGas,
		bool canDriveInSnow);
	//builds the query based on the parameters
	void buildQuery();
	//returns the query string
	string getQueryString();

	//expands the search to include all cities in the same area
	void expandToArea() {
		this->expandCityToArea = true;
		buildQuery();
	}
	//expands the search to show general availability instead of specific days
	void expandToGeneralAvailability() {
		aDay = "GEN";
		buildQuery();
	}

	void runQuery(Database * friends);

private:
	string queryString;

	//these booleans are for what is shown (used to make the SELECT part of the query)
	//true indicates that this value is shown, 
	bool gasMoneyLocal, gasMoneyTrip, AAA, milesLimit, showCity, showArea, availability, canMoveSofa, canSnow, gasEconomy, reliability, seatbelts;

	//these booleans determine the query type
	//cityChange means that there are two cities involved, specificWeekday means that availability is checked and limiited to a specific day, 
	//and expandCityToArea expands the query to all Cities in the same Area
	bool cityChange, specificWeekDay, expandCityToArea;

	//these strings determine the search parameters, an * means we don't care 
	//these booleans alse determine the search parameters, true means we care, false means we don't care
	string startCity, startTimeOfYear, endCity, endTimeOfYear, aDay, minGasEconomy, minReliability, minSeatbelts, carType;
	bool needsToMoveSofa, driverIsAAAMember;
	string minMilesNeeded;
	bool doesNotNeedLocalGas, doesNotNeedTripGas, canDriveInSnow;

};


Query::Query(	//these first three lines of booleans are for what is shown
	bool gasMoneyLocal, bool gasMoneyTrip, bool AAA, bool milesLimit,
	bool showCity, bool showArea, bool availability, bool canMoveSofa,
	bool canSnow, bool gasEconomy, bool reliability, bool seatbelts,

	//these booleans determine the query type
	bool cityChange, bool specificWeekDay, bool expandCityToArea,

	//these strings determine the search parameters, an * means we don't care 
	string startCity, string startTimeOfYear, string endCity, string endTimeOfYear , string aDay, string minGasEconomy, string minReliability, string minSeatbelts,
	string carType, bool needsToMoveSofa, bool driverIsAAAMember, string minMilesNeeded, bool doesNotNeedLocalGas, bool doesNotNeedTripGas,
	bool canDriveInSnow

	)
{
	//there is probably a better way to do this. Will ask about it later. 
	//Initialize the class variables to set up the query parameters. 
	this->gasMoneyLocal = gasMoneyLocal;
	this->gasMoneyTrip = gasMoneyTrip;
	this->AAA = AAA;
	this->milesLimit = milesLimit;
	this->showCity = showCity;
	this->showArea = showArea;
	this->availability = availability;
	this->canMoveSofa = canMoveSofa;
	this->canSnow = canSnow;
	this->gasEconomy = gasEconomy;
	this->reliability = reliability;
	this->cityChange = cityChange;
	this->specificWeekDay = specificWeekDay;
	this->expandCityToArea = expandCityToArea;
	this->startCity = startCity;
	this->startTimeOfYear = startTimeOfYear;
	this->endCity = endCity;
	this->endTimeOfYear = endTimeOfYear;
	this->aDay = aDay;
	this->minGasEconomy = minGasEconomy;
	this->minReliability = minReliability;
	this->minSeatbelts = minSeatbelts;
	this->carType = carType;
	this->needsToMoveSofa = needsToMoveSofa;
	this->driverIsAAAMember = driverIsAAAMember;
	this->minMilesNeeded = minMilesNeeded;
	this->doesNotNeedLocalGas = doesNotNeedLocalGas;
	this->doesNotNeedTripGas = doesNotNeedTripGas;
	this->canDriveInSnow = canDriveInSnow;
	this->seatbelts = seatbelts;
}


void Query::buildQuery()
{

	string query = "";
	query += "SELECT";
	query += " DISTINCT ";
	query +=" P.FullName, P.Contact, P.Contact2";
	if (gasMoneyLocal)
		query += ", P.GasMoneyLocal";
	if (gasMoneyTrip)
		query += ", P.GasMoneyTrip";
	if (AAA)
		query += ", P.AAA";
	if (milesLimit)
		query += ", PC.milesLimit";
	if (showCity)
		query += ", L.City";
	if (showArea)
		query += ", L.Area";
	if (availability)
		query += ", A.Day, A.Description";
	if (canMoveSofa)
		query += ", C.CanMoveSofa";
	if (canSnow)
		query += ", PC.CanSnow";
	if (gasEconomy)
		query += ", C.GasEconomy";
	if (reliability)
		query += ", C.Reliability";
	if (seatbelts)
		query += ", C.Seatbelts";

	//choose the tables to select from, if there is a change of city during the trip, include a second set of location tables
	//if the search is expanded to search by area instead of by city, add a location table for that
	query += " FROM Person as P, Location as L, Availability as A, Car as C, PersonHasCar as PC, PersonIsAtLocation as PL";
	if (cityChange)
		query += ", PersonIsAtLocation as PL2, Location as L2";
	if (expandCityToArea)
		query += ", Location as LAreas";
	if (expandCityToArea && cityChange)
	{
		query += ", location as LAreas2";
	}

	//join the tables in the start of the where clause
	query += " WHERE P.FullName = PL.Person AND PL.City = L.City AND P.FullName = PC.Person AND PC.CarID = C.ID AND PC.Availability = A.ID ";
	if (cityChange) {
		query += " AND P.FullName = PL2.Person";
		if (!expandCityToArea) {
			query += " AND PL2.City = L2.City AND L2.City = '";
			query += endCity;
			query += "'";
		}
		query += " AND PL2.";
		query += endTimeOfYear;

	}

	//starting time of year is mandatory
	query += " AND PL.";
	query += startTimeOfYear;
	query += " = YES AND A.";
	query += startTimeOfYear;
	query += " = YES";

	//If we care about the day
	if (specificWeekDay)
	{
		query += " AND A.Day = \'";
		query += aDay;
		query += "\'";
	}

	//starting location is mandatory, but may be expanded to area. If we are including all cities, we don't care about the areas involved
	if (startCity != "*")
	{
		if (expandCityToArea)
		{
			query += " AND LAreas.City = \'";
			query += startCity;
			query += "\' AND L.Area = LAreas.Area";

			if (cityChange)
			{
				query += " AND LAreas2.City = \'";
				query += endCity;
				query += "\' And L2.Area = LAreas2.Area";
			}
		}

		else
		{
			query += " AND L.City = \'";
			query += startCity;
			query += "\'";
		}
	}

	//refine for minimum gas economy (mpg), reliability (scale of 1-10), and number of seatbelts
	query += " AND C.GasEconomy >= ";
	query += minGasEconomy;
	query += " AND C.Reliability >= ";
	query += minReliability;
	query += " AND C.Seatbelts >= ";
	query += minSeatbelts;

	//refine for the type of car, if moving a sofa, if the driver is a AAA member, wants gas money, and can drive that car in snow
	if (carType != "*")
	{
		query += " AND C.type = \'";
		query += carType;
		query += "\'";
	}
	if (needsToMoveSofa)
		query += " AND C.CanMoveSofa = YES";
	if (driverIsAAAMember)
		query += " AND P.AAA = YES";
	query += " AND PC.MilesLimit >= ";
	query += minMilesNeeded;
	if (doesNotNeedLocalGas)
		query += " AND P.GasMoneyLocal = NO";
	if (doesNotNeedTripGas)
		query += " AND P.GasMoneyTrip = NO";
	if (canDriveInSnow)
		query += " AND PC.CanSnow = YES";

	//order by the person's name, and put a semicolon on the end of the query
	query += " ORDER BY P.FullName;";

	queryString = query;
}

string Query::getQueryString() {
	return queryString;
}




//calls queryBuilder to build the sql string, builds the vectors of column names and widths
void Query::runQuery(Database * friends)
{
	string q = queryString;


	vector<int> columnWidths;
	vector<string> columnName;

	columnWidths.push_back(FULL_NAME_LENGTH);
	columnName.push_back("Name");
	columnWidths.push_back(CONTACT);
	columnName.push_back("Contact");
	columnWidths.push_back(CONTACT2);
	columnName.push_back("Contact 2");

	if (gasMoneyLocal) {
		columnWidths.push_back(GAS_MONEY_LOCAL);
		columnName.push_back("Gas Money Local");
	}

	if (gasMoneyTrip) {
		columnWidths.push_back(GAS_MONEY_TRIP);
		columnName.push_back("Gas Money Trip");
	}

	if (AAA) {
		columnWidths.push_back(AAAval);
		columnName.push_back("AAA");
	}

	if (milesLimit) {
		columnWidths.push_back(MILES_LIMIT);
		columnName.push_back("Miles Limit");
	}

	if (showCity) {
		columnWidths.push_back(CITY);
		columnName.push_back("City");
	}

	if (showArea) {
		columnWidths.push_back(AREA);
		columnName.push_back("Area");
	}

	if (availability) {
		columnWidths.push_back(DAY);
		columnName.push_back("Day");

		columnWidths.push_back(DESCRIPTIONval);
		columnName.push_back("Description");
	}

	if (canMoveSofa) {
		columnWidths.push_back(CAN_MOVE_SOFA);
		columnName.push_back("Sofa");
	}

	if (canSnow) {
		columnWidths.push_back(CAN_SNOW);
		columnName.push_back("Snow");
	}

	if (gasEconomy) {
		columnWidths.push_back(GAS_ECON);
		columnName.push_back("Mpg");
	}

	if (reliability) {
		columnWidths.push_back(RELIABILITY);
		columnName.push_back("Reliability");
	}

	if (seatbelts) {
		columnWidths.push_back(SEATBELTS);
		columnName.push_back("Seatbelts");
	}

	//now actually run the query
	friends->query(columnWidths, columnName, queryString);
}





//person







#endif