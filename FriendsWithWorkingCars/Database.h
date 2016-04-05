#ifndef DATABASE_H_
#define DATABASE_H_

#include <string>
#include <iostream>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include<iomanip>
#include<vector>

#include "Query.h"

const int MAX_DATA = 100;
using namespace std;
string insertPerson(string name, bool AAA, string contact, string contact2, bool gasLocal, bool gasTrip);

class Database {
public:
	void query(vector<int> columnWidths, vector<string>columnNames, string q);
	vector<string> miniQuery(string q);

	vector<string> getCities() {
		return cities;
	}
	vector<string> getCarTypes() {
		return carTypes;
	}
	vector<string> getNames()
	{
		return names;
	}
	void updateUtilityVectors();

	void insert();

	Database(string path);
	~Database();

private:
	RETCODE rc;
	HENV henv;
	HDBC hdbc;
	HSTMT hstmt;
	char szData[MAX_DATA];
	string stSQL;
	SDWORD cbData;

	void displayOutput(string msg, bool boolOutput, int maxLength); //may want to move this to a UI class

	vector<string> cities;
	vector<string> carTypes;
	vector<string> names;

};


/*@Function Database Constructor
* sets up the connection to the database
* initializes the vectors of cities and car types that exist in our database
* @param path : a string containing the path (local or absolute) to the access database
*/
Database::Database(string path)
{
	SQLAllocEnv(&henv);
	SQLAllocConnect(henv, &hdbc);

	SQLCHAR szConnectOut[1024];
	SQLSMALLINT cchConnect;

	string stConnect = path;
	rc = SQLDriverConnectA(hdbc, NULL, (SQLCHAR *)stConnect.c_str(), stConnect.length(), szConnectOut, 1024, &cchConnect, SQL_DRIVER_NOPROMPT);
	
	updateUtilityVectors();
}

/* @function Database Destructor
* disconnects and frees the objects used for the database connection
*/
Database::~Database()
{
	
	SQLDisconnect(hdbc);
	SQLFreeConnect(hdbc);
	SQLFreeEnv(henv);
}


void Database::updateUtilityVectors()
{
	string q1;
	q1 = "SELECT DISTINCT Type";
	q1 += " FROM Car ORDER BY Type ASC;";
	carTypes = this->miniQuery(q1);

	string q2;
	q2 = "SELECT DISTINCT City";
	q2 += " FROM Location ORDER BY City ASC;";
	cities = this->miniQuery(q2);

	string q3 = "SELECT DISTINCT FullName FROM PERSON ORDER BY FullName ASC;";
	names = this->miniQuery(q3);
}


/* @function query
* Runs the query and displays the result using displayOutput. 
* @param columnWidths : a vector of integers corresponding to the widths of the columns for display purposes. Based on constants, and created in the query builder. 
* @param columnNames : a vector of strings corresponding to the names of the columns, for display purposes. Created in the query builder. 
* @param q : a string containing the SQL query to be executed. 
*
*/
void Database::query(vector<int> columnWidths, vector<string>columnNames, string q)
{
	SQLAllocStmt(hdbc, &hstmt);

	rc = SQLExecDirectA(hstmt, (SQLCHAR*)q.c_str(), SQL_NTS); //execute the query
	SQLSMALLINT columns;

	if (columnWidths.size() != columnNames.size())
	{
		//throw an error?
		cout << "Vector size mismatch" << endl;
		return;
	}

	for (int i = 0; i < columnWidths.size(); i++)
	{
		displayOutput(columnNames[i], false, columnWidths[i]);
		displayOutput("|", false, 1);
	}

	while (rc == SQL_SUCCESS) {
		displayOutput("\n", false, 1);
		rc = SQLFetch(hstmt);
		SQLNumResultCols(hstmt, &columns);

		if (columnWidths.size() > columns)
		{
			//throw an error?
			cout << "More vector columns than result columns" << endl;
			return;
		}
		if (columnWidths.size() < columns)
		{
			//throw an error?
			cout << "Fewer vector columns than result columns" << endl;
			//return;
		}
		for (int i = 1; i <= columnWidths.size(); i++)
		{
			if (SQLGetData(hstmt, i, SQL_C_CHAR, szData, sizeof(szData), &cbData) == SQL_SUCCESS) {
				displayOutput(szData, true, columnWidths[i - 1]);
				displayOutput("|", false, 1);
			}
		}
	}
	SQLFreeStmt(hstmt, SQL_CLOSE);
}

/* @function displayOutput
* displays a line, or part of a line of output. If given a string representing boolean values (such as "0"), will output true or false.
* left aligns text, and truncates it to the appropriate length. If shorter than the appropriate length, left aligns the text and fills the extra space.
* @param msg : a string containing the message to be printed
* @param boolOutput : a boolean indicating if the string passed is going to be "0" or "1", and therefore needs to be printed as a boolean
* @param maxLength : the length to truncate/extend the output to. "1234" with length 3 will print "123". "1234" with length 6 will print "1234  "
*/
void Database::displayOutput(string msg, bool boolOutput, int maxLength)
{
	//sql booleans don't appear as booleans, but rather as ones or zeros in a string

	if (boolOutput)
	{
		if (msg == "0")
			msg = "No";
		else if (msg == "1")
			msg = "Yes";
	}
	if (msg.length() > maxLength) //truncate the output to the desired length
	{
		msg = msg.substr(0, maxLength);
	}
	cout << setw(maxLength) << left << msg;
}

/*
*@Function miniQuery
* This function takes a string for a single column query, and returns the results as a vector of strings
*@param q : the string of sql for the query to execute
*@return : a vector of strings of the results from the single columns
*/
vector<string> Database::miniQuery(string q)
{
	SQLAllocStmt(hdbc, &hstmt);
	vector<string> data;
	
	rc = SQLExecDirectA(hstmt, (SQLCHAR*)q.c_str(), SQL_NTS); //execute the query
	while (rc == SQL_SUCCESS) {
		rc = SQLFetch(hstmt);
		if (SQLGetData(hstmt, 1, SQL_C_CHAR, szData, sizeof(szData), &cbData) == SQL_SUCCESS)
			data.push_back(szData);
	}

	SQLFreeStmt(hstmt, SQL_CLOSE);
	return data;
}

//This funciton is not my code. I copy-pasted it to help with debugging, because I don't understand the statements/environments/other abbreviated objects well enough for the documentation to be helpful. 
// We did try reading the documentation first though
//http://community.actian.com/wiki/ODBC_Bulk_Insert_Example (where this came from)
//https://msdn.microsoft.com/en-us/library/ms712628%28v=vs.85%29.aspx (the API information that I struggled with because I don't understand embedded sql well enough to choose the correct parameters)
void print_err(RETCODE rc, HENV henv, HDBC hdbc, SQLHSTMT hstmt)
{
	SQLCHAR    SqlState[6], Msg[SQL_MAX_MESSAGE_LENGTH];
	SQLINTEGER NativeError;
	SQLSMALLINT i = 1, MsgLen;
	RETCODE ret;

	printf("Error number is %d\n", rc);
	ret = SQL_SUCCESS;
	if (hstmt == SQL_NULL_HSTMT)
	{
		while (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		{
			ret = SQLErrorA(henv, hdbc, hstmt, SqlState,
				&NativeError, Msg, sizeof(Msg), &MsgLen);
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
			{
				printf("%d %s\n", NativeError, Msg);
			}
		}
		exit(-1);
	}
	printf("hstmt\n");
	while (SQLGetDiagRecA(SQL_HANDLE_STMT, hstmt, i,
		SqlState, &NativeError, Msg, sizeof(Msg), &MsgLen) != SQL_NO_DATA)
	{
		printf("State: %s  Native Error Code: %d\n %s\n",
			SqlState, NativeError, Msg);
		i++;
	}
	exit(-1);
}

void Database::insert() {
	SQLAllocStmt(hdbc, &hstmt);
	string ins = insertPerson("Dad", true, "123456", "123654", false, false);
	SQLINTEGER TextLength = ins.size();

	SQLCHAR * StatementText = (SQLCHAR *)ins.c_str();

	rc = SQLPrepareA(hstmt, StatementText, SQL_NTS);
	if (rc != SQL_SUCCESS) {
		cout << "PROBLEM " << endl;
		print_err(rc, NULL, SQL_NULL_HDBC, hstmt);
	}

	rc = SQLExecute(hstmt);
	if (rc != SQL_SUCCESS) {
		cout << "PROBLEM " << endl;
		print_err(rc, NULL, SQL_NULL_HDBC, hstmt);
	}
	rc = SQLTransact(henv, hdbc, SQL_COMMIT);
	if (rc != SQL_SUCCESS)
		cout << "PROBLEM2 " << endl;
	SQLFreeStmt(hstmt, SQL_CLOSE);
}




string insertPerson(string name, bool AAA, string contact, string contact2, bool gasLocal, bool gasTrip) {
	//Tried: brackets around field names, using YES for true, using 1 for true, using -1 for true (because according to the internet, these are all the 'only' way that access stores yes/no as true.)
	//the query works in access, but not in the connection
	stringstream ss;
	ss << "INSERT INTO" << " Person ("; 
	ss << "FullName, Contact, Contact2, AAA, GasMoneyLocal, GasMoneyTrip) VALUES ('";
	ss << name << "', ";
	
	ss << " \"" << contact << "\", \"" << contact2 << "\"";
	ss << ", ";

	if (AAA)
		ss << "-1, ";
	else
		ss << "0, ";


	if (gasLocal)
		ss << "-1, ";
	else
		ss << "0, ";

	if (gasLocal)
		ss << "-1";
	else
		ss << "0";
	ss << ");";
	cout << ss.str();
	return ss.str();
}


#endif
