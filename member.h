#ifndef MEMBER_H
#define MEMBER_H

#include "sqlite3.h"
#include "execute.h"
#include <iomanip>
#include <ctime>
#include <iostream>
using namespace std;

void editMembers(sqlite3* db);
void updateMember(sqlite3* db);
void deleteMember(sqlite3* db);
void addMember(sqlite3* db);
void printMembers(sqlite3* db);


#endif