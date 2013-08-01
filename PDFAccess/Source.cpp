#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include "sqlite3.h"

using namespace std;

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i=0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int CreateDatabase(string name)
{
	name+=".db";
	const char* dbname = name.c_str();
	char *zErrMsg = 0;
	int rc;

	sqlite3 *database;
	rc = sqlite3_open(dbname, &database);

	if( rc ){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(database));
		// obs³uga b³êdu dostêpu do bazy
		exit(0);
	}else{
		fprintf(stdout, "Opened database successfully\n");
	}

	char* statement = "CREATE TABLE ONE(ID    INTEGER     PRIMARY KEY AUTOINCREMENT     NOT NULL, NAME           CHAR(50)    NOT NULL, LABEL         CHAR(50), GRP             CHAR(50), FILE         BLOB        );";

	rc = sqlite3_exec(database, statement, callback, 0, &zErrMsg);

	if( rc != SQLITE_OK ){
		// obs³uga b³êdu tworzenia tabeli
		sqlite3_free(zErrMsg);
	}else{
		fprintf(stdout, "Table created successfully\n");
	}
	sqlite3_close(database);

	return 0;
}
int InsertFile(string name)
{
	const char* dbname = name.c_str();
	sqlite3 *database;
	int rc = sqlite3_open(dbname, &database);
	char *zErrMsg = 0;

	// zmien open

	ifstream file;
	file.open("6Zasady_zachowania.pdf", ios::in, ios::binary);

	if(file.is_open())
	{
		cout << "jest ok" << endl;
	}else
		cout << "kurr" << endl;


	sqlite3_stmt *stmt = NULL;

	char* statement = "INSERT INTO ONE(     ID,    NAME,    LABEL,    GRP,    FILE ) VALUES (     NULL,    'fedfs',    NULL,    NULL,    NULL);";

	//rc = sqlite3_prepare_v2(database, statement, 0, &stmt, NULL);

	rc = sqlite3_exec(database, statement, callback, 0, &zErrMsg);

	if(rc!=SQLITE_OK)
	{
		fprintf(stderr, "prepare failed",  sqlite3_errmsg(database));
		sqlite3_free(zErrMsg);
	}
	/*
	rc = sqlite3_bind_blob(stmt, 1, NULL, -1, NULL);

	if(rc!=SQLITE_OK)
	{
		fprintf(stderr, "bind failed",  sqlite3_errmsg(database));
		cout << rc << endl;
		sqlite3_free(zErrMsg);
	}
	*/
	//rc = sqlite3_step(stmt);

	sqlite3_close(database);

	return 0;

}
int main()
{
	sqlite3_initialize();

	//CreateDatabase("WTF");
	InsertFile("WTF");

	sqlite3_shutdown();

	return 0;
}