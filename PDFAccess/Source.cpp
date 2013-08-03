#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include "sqlite3.h"

#define MAX 1000000

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
int InsertFile(string name)
{
	const char* dbname = name.c_str();
	sqlite3 *database;
	int rc = sqlite3_open(dbname, &database);
	char *zErrMsg = 0;
	char buffer[MAX];

	// tutaj za³atwiamy wczytywanie do binarnego

	ifstream file;
	file.open("6Zasady_zachowania.pdf", ios::binary);
		
		if ( ! file )
{
        cout << "An error occurred opening the file" << endl;
}

		ofstream ofs("temp.txt", ios::trunc); 
		ofs << file.rdbuf();

	int count = 0;

	while(file.good())
	{
		char c=file.get();
		buffer[count] = c;
		count++;
	}

	// dalej jedyne co zmieniamy, to w bind_blob trzeci argument. Patrz instrukcja w pdfie pod sqlite3_binb_blob

	sqlite3_stmt *stmt = NULL;

	char* statement = "INSERT INTO ONE(     ID,    NAME,    LABEL,    GRP,    FILE ) VALUES (     NULL,    'fedfs',    NULL,    NULL,   ?);";

	rc = sqlite3_prepare_v2(database, statement, 0, &stmt, NULL);

	rc = sqlite3_bind_blob(stmt, 1, file.rdbuf(), -1, NULL);

	// rc to wynik zapytania, 21 oznacze z³e u¿ycie SQLa ( jak narazie ci¹gle to wywala)
	// 1 to z³y syntax zapytania,a 0 to wszystko ok

	if(rc!=SQLITE_OK)
	{
		fprintf(stderr, "bind failed",  sqlite3_errmsg(database));
		cout << rc << endl;
		sqlite3_free(zErrMsg);
	}

	rc = sqlite3_step(stmt);

	sqlite3_close(database);

	return 0;

}

int main()
{
	sqlite3_initialize();

	//CreateDatabase("WTF.db");
	InsertFile("WTF.db");

	sqlite3_shutdown();

	return 0;
}