#include <iostream>
#include <string>
#include "MyDB.h"
using namespace std;

int main()
{
	MyDB db;
	string exec="select *from usr";
	db.initDB("localhost","root","","yy");
	db.exeSQL(exec);
	return 0;
}
