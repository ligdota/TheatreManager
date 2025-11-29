#ifndef FINANCES_H
#define FINANCES_H

#include <iostream>
#include <memory>
#include <array>
#include <string>
#include <stdexcept>
#include <cstdio>
#include "sqlite3.h"
#include <cstdlib>
#include "json.hpp"
#include "execute.h"
#include "finances.h"
using namespace std;
using json = nlohmann::json;


void parseJSON(int production_id);
std::string runPythonBalanceSheet(int productionID);
void viewFinanceReporting(sqlite3* db);
void totalFinances(sqlite3* db);
void productionFinances(sqlite3* db);
void memberDuesFinances(sqlite3* db);




#endif