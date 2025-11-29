#ifndef PLAY_H
#define PLAY_H
#include "sqlite3.h"



    void editPlays(sqlite3* db);
    void addPlay(sqlite3* db);
    void deletePlay(sqlite3* db);
    void updatePlay(sqlite3* db);
    void printPlays(sqlite3* db);
    



#endif