#ifndef PURCHASE_TICKETS_H
#define PURCHASE_TICKETS_H

#include <iostream>
#include <string>
#include <vector>
#include "sqlite3.h"
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <cmath>      // for std::abs
#include "execute.h"
#include <cstdlib>    // for std::stoi


void purchaseTickets(sqlite3 *db);



#endif