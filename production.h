#ifndef PRODUCTION_H
#define PRODUCTION_H
#include "sqlite3.h"
#include "execute.h"
#include "play.h"
#include "member.h"
#include <limits>
#include <iomanip>
#include <iostream>
using namespace std;
void listProductions(sqlite3* db);
void updateProduction(sqlite3* db);
void addProduction(sqlite3* db);
void editProductions(sqlite3* db);
void deleteProduction(sqlite3* db);

#endif