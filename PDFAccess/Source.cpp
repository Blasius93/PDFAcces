#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <queue>
#include "sqlite3.h"

#define MAX 100000000

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
int InsertFile(const string& db_name)
{
    ifstream file("Sql.pdf", ios::in | ios::binary);
    if (!file) {
        cerr << "An error occurred opening the file\n";
        return 12345;
    }
    file.seekg(0, ifstream::end);
    streampos size = file.tellg();
    file.seekg(0);

    char* buffer = new char[size];
    file.read(buffer, size);

    sqlite3 *db = NULL;
    int rc = sqlite3_open_v2(db_name.c_str(), &db, SQLITE_OPEN_READWRITE, NULL);
    if (rc != SQLITE_OK) {
        cerr << "db open failed: " << sqlite3_errmsg(db) << endl;
    } else {
        sqlite3_stmt *stmt = NULL;
        rc = sqlite3_prepare_v2(db, "INSERT INTO ONE(ID, NAME, LABEL, GRP, FILE) VALUES(NULL, 'fedfsdfss', NULL, NULL, ?)",-1, &stmt, NULL);
        if (rc != SQLITE_OK) {
            cerr << "prepare failed: " << sqlite3_errmsg(db) << endl;
        } else {
            // SQLITE_STATIC because the statement is finalized
            // before the buffer is freed:
            rc = sqlite3_bind_blob(stmt, 1, buffer, size, SQLITE_STATIC);
            if (rc != SQLITE_OK) {
                cerr << "bind failed: " << sqlite3_errmsg(db) << endl;
            } else {
                rc = sqlite3_step(stmt);
                if (rc != SQLITE_DONE)
                    cerr << "execution failed: " << sqlite3_errmsg(db) << endl;
            }
        }
        sqlite3_finalize(stmt);
    }
    sqlite3_close(db);

    delete[] buffer;
}
int main()
{
	sqlite3_initialize();

	//CreateDatabase("WTF.db");
	InsertFile("WTF.db");

	sqlite3_shutdown();

	return 0;
}