#include <iostream>
#include "sqlite3.h"
#include <string>
#include <cstdlib>
using namespace std;


void editProductions(sqlite3* db);
void editPlays(sqlite3* db);
void viewFinanceReporting();
void editMembers();
void printPlays(sqlite3* db);
void addPlays(sqlite3* db);




int main() {

    sqlite3* db;
    int dbStatus = sqlite3_open("theatre.db", &db);

    if (dbStatus) {
        cerr << "Error: Cannot open database" << sqlite3_errmsg(db) << endl;
        return 1;
    }   else {
        cout << "Database connected" << endl;
    }

    char c;
    
    while (c != 'Q' && c != 'q') {
    cout << "Welcome to the Bella SW Community Theatre Management System" << endl;
    cout << "press 1 to edit productions" << endl;
    cout << "press 2 to edit plays" << endl; 
    cout << "press 3 to view finance reporting" << endl;
    cout << "press 4 to edit members" << endl;
    cout << "press Q to quit" << endl;
    cin >> c;
    //system("CLS");
    
    

    switch(c) {
        case '1':
            cout << "Editing productions..." << endl;
            editProductions(db);
            // Add production editing logic here
            break;
        case '2':
            cout << "Editing plays..." << endl;
            editPlays(db);
            // Add play editing logic here
            break;
        case '3':
            cout << "Viewing finance reporting..." << endl;
            viewFinanceReporting();
            // Add finance reporting logic here
            break;
        case '4':
            cout << "Editing members..." << endl;
            editMembers();
            // Add member editing logic here
            break;
        case 'Q':
        case 'q':
            cout << "Quitting the program." << endl;
            return 0;
        default:
            cout << "Invalid option, please try again." << endl;
    }
}
sqlite3_close(db);
return 0;
}



void editProductions(sqlite3* db) {
    // connect to the database, fetch production data, and allow editing
    // Implementation for editing productions
    cout << "press 1 to add a production" << endl;
    cout << "press 2 to delete a production" << endl;
    cout << "press 3 to update a production" << endl;
    cout << "press 4 to return to the previous menu" << endl;
    char choice;
    cin >> choice;
    switch(choice) {
        case '1':
            cout << "Adding a production..." << endl;
            // Add logic to add a production
            break;
        case '2':
            cout << "Deleting a production..." << endl;
            // Add logic to delete a production
            break;
        case '3':
            cout << "Updating a production..." << endl;
            // Add logic to update a production
            break;
        case '4':
            cout << "Returning to previous menu..." << endl;
            break;
    }
    //system("CLS");

}
void editPlays(sqlite3* db) {
    // Implementation for editing plays
    cout << "press 1 to add a play" << endl;
    cout << "press 2 to delete a play" << endl;
    cout << "press 3 to update a play" << endl;
    cout << "press 4 to return to the previous menu" << endl;
    char choice;
    cin >> choice;
    switch(choice) {
        case '1':
            cout << "Adding a play..." << endl;
            addPlays(db);
            // Add logic to add a play
            break;
        case '2':
            cout << "Deleting a play..." << endl;
            // Add logic to delete a play
            break;
        case '3':
            cout << "Updating a play..." << endl;
            // Add logic to update a play
            break;
        case '4':
            cout << "Returning to previous menu..." << endl;
            break;
    }

    //system("CLS");
}
void viewFinanceReporting() {
    // Implementation for viewing finance reporting

    //system("CLS");
}
void editMembers() {
    // Implementation for editing members

    //system("CLS");
}



void addPlays(sqlite3* db) {
    const char* sql = "INSERT INTO play (play_title, play_author, play_genre, play_num_acts) VALUES (?, ?, ?, ?);";
    sqlite3_stmt* stmt;
    
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    string title, author, genre;
    int num_acts;

    cout << "Enter play title: ";
    cin >> ws; // to consume any leading whitespace
    getline(cin, title);
    cout << "Enter play author: ";
    getline(cin, author);
    cout << "Enter play genre: ";
    getline(cin, genre);
    cout << "Enter number of acts: ";
    cin >> num_acts;

    sqlite3_bind_text(stmt, 1, title.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, author.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, genre.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 4, num_acts);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Play added successfully." << endl;
    }

    sqlite3_finalize(stmt);
}

void printPlays(sqlite3* db) {
    const char* sql = "SELECT id, title, author, genre num_acts FROM plays;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Plays List:" << endl;
    cout << "ID | Title | Author | Duration" << endl;
    cout << "----------------------------------" << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* title = sqlite3_column_text(stmt, 1);
        const unsigned char* author = sqlite3_column_text(stmt, 2);
        const unsigned char* genre = sqlite3_column_text(stmt, 3);
        int num_acts = sqlite3_column_int(stmt, 4);

        cout << id << " | " << title << " | " << author << " | " << genre <<  " | " << num_acts << " |" << endl;
    }

    sqlite3_finalize(stmt);
}

