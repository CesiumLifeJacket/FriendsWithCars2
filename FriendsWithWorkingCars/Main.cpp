#include <iostream>
#include <string>
#include <windows.h>
#include <sql.h>
#include <sqlext.h>
#include<iomanip>

#include "Query.h"
#include "Menu.h"
#include "Database.h"

void displayOutput(string msg, bool sqlOutput, int maxLength = 5);
using namespace std;

int main()
{
	system("mode 150,30"); 
	SMALL_RECT WinRect = { 0, 0, 150, 30 };
	SMALL_RECT* WinSize = &WinRect;
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), true, WinSize);
	Database * friends = new Database("DRIVER={Microsoft Access Driver (*.mdb, *.accdb)};DBQ=..\\..\\CS374W_FriendsWithCars.accdb;UID=Admin;PWD=;");
	
	//friends->insert();


	int menuOpt = menu();

	switch (menuOpt)
	{
	case 1:
		roadTrip(friends);
		break;
	case 2:
		breakTrip(friends);
		break;
	case 3:
		localTrip(friends);
		break;
	case 4:
		break;
	default:
		break;
	}
		
	
	delete friends; //closes the connection

	return 0;
}