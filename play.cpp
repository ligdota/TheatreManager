#include "play.h"
#include "execute.h"
#include <iostream>
#include <limits>
using namespace std;


void editPlays(sqlite3* db) {
    // Implementation for editing plays
    cout << "press 1 to add a play" << endl;
    cout << "press 2 to delete a play" << endl;
    cout << "press 3 to update a play" << endl;
    cout << "press 4 to view all plays" << endl;
    cout << "press 5 to return to the previous menu " << endl;
    char choice;
    cin >> choice;
    switch(choice) {
        case '1':
            cout << "Adding a play..." << endl;
            addPlay(db);
            // Add logic to add a play
            break;
        case '2':
            cout << "Deleting a play..." << endl;
            deletePlay(db);
            // Add logic to delete a play
            break;
        case '3':
            cout << "Updating a play..." << endl;
            // Add logic to update a play
            updatePlay(db);
            break;
        case '4':
            printPlays(db);
            editPlays(db);
            break;
        
        case '5':
            cout << "Returning to the previous screen" << endl;
            return;
        default:
        cout << "Invalid option entered returning to previous screen" << endl;
        return;
    }
    return;
    //system("CLS");
}

void deletePlay(sqlite3* db) {

    printPlays(db);
    cout << endl;
    int input_play_id;
    string play_name;
    int user_choice = 0;

    cout << "1. Delete by play name" << endl 
         << "2. Delete by ID" << endl;
        cin >> user_choice;

    if (user_choice == 1) {
        cout << "Enter the name of the play to delete" << endl;
        fflush(stdin);
        getline(cin, play_name);
        const char* sql = "DELETE FROM play WHERE play_title = ?;";
        if (executeSQLStatement(db, sql, {play_name})) {
            cout << "Success: Deleted play " + play_name << endl;
        } else {
            cout << "Failed: Deleting play " + play_name << endl;
        }
    } else if (user_choice == 2) {

    cout << "Enter the ID of the play to delete" << endl;
    cin >> input_play_id;
    const char* sql = "DELETE FROM play WHERE play_id = ?;";
    if (executeSQLStatement(db, sql, {to_string(input_play_id)})) {
        cout << "Success: Deleted play " + play_name << endl;
    } else {
        cout << "Failed: Deleting play " + play_name << endl;
    }
    }
    else {
        cout << "Invalid input entered" << endl;
        cout << "Returning to the previous menu" << endl;
        return;
    }

    return;

}

void updatePlay(sqlite3* db) {

    printPlays(db);
    fflush(stdin);
    string play_id_str;
    int update_input;
    cout << "Enter the id of the play to update ";
    getline(cin, play_id_str);

    
    cout << "1. Update title" << endl
         << "2. Update author" << endl
         << "3. Update genre" << endl
         << "4. Update number of acts" << endl
         << "5. Update all fields" << endl;

        cin >> update_input;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        string title, author, genre;
        int number_acts;
        string concat_options;

        switch (update_input) {
            case 1: {
                cout << "Enter the new title" << endl;
                getline(cin, title);
                concat_options = "play_title = '" + title + "'";
                break;
                // SET PLAY_TITLE = TITLE
            }
            case 2: {
                cout << "Enter the new author" << endl;
                getline(cin, author);
                concat_options += "play_author = '" + author + "'";
                break;
                
            }
            case 3: {
                cout << "Enter the new genre" << endl;
                getline(cin, genre);
                concat_options += "play_genre = '" + genre + "'";
                break;
            }
            case 4: {
                cout << "Enter the new number of acts" << endl;
                cin >> number_acts;
                concat_options += "play_num_acts = '" + to_string(number_acts) + "'";
                break;
            }
            case 5: {
                cout << "Enter the new title" << endl;
                getline(cin, title);
                cout << "Enter the new author" << endl;
                getline(cin, author);
                cout << "Enter the new genre" << endl;
                getline(cin, genre);
                cout << "Enter the new number of acts" << endl;
                cin >> number_acts;
                concat_options =
                "play_title = '" + title + "', "
                "play_author = '" + author + "', "
                "play_genre = '" + genre + "', "
                "play_num_acts = " + to_string(number_acts);
                break;
            }
            default: {
                cout << "Invalid input entered" << endl;
                cout << "Returning to the previous menu..." << endl;
                return;
            }
            
            
            
        }
        
        sqlite3_stmt* stmt;
        string sql = "UPDATE play "
        "SET " + concat_options + " WHERE play_id = " + play_id_str + ";";
        if (executeSQLStatement(db, sql, {})) {
            cout << "play updated" << endl;
        } else {
            cout << "play failed to update" << endl;
        }
        
         

return;
}



void addPlay(sqlite3* db) {
    const char* sql = "INSERT INTO play (play_title, play_author, play_genre, play_num_acts) VALUES (?, ?, ?, ?);";

    string title, author, genre;
    string num_acts;

    cout << "Enter play title: ";
    cin >> ws; // to consume any leading whitespace
    getline(cin, title);
    cout << "Enter play author: ";
    getline(cin, author);
    cout << "Enter play genre: ";
    getline(cin, genre);
    cout << "Enter number of acts: ";
    getline(cin, num_acts);

    if (executeSQLStatement(db, sql, {title, author, genre, num_acts})) {
        cout << "Successfully added play " + title << endl;
    }

}






void printPlays(sqlite3* db) {
    const char* sql = "SELECT play_id, play_title, play_author, play_genre, play_num_acts FROM play;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << "Plays List:" << endl;
    cout << "ID | Title | Author | Genre | Number of Acts" << endl;
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
