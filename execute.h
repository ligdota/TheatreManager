#include "sqlite3.h"
#include <string>
#include <vector>
#include <iostream>
using namespace std;


bool executeSQLStatement(sqlite3* db, const string& sql, const vector<string>& sqlParams);
vector<vector<string>> executeSQLQuery(sqlite3* db, const string& sql, const vector<string>& params);

bool exists(sqlite3* db, string& table, int id);