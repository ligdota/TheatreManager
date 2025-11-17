#include "play.h"
#include "production.h"
#include "member.h"



void viewFinanceReporting(sqlite3* db);

void userMode(sqlite3* db);
void manageMode(sqlite3* db);
void purchaseTickets(sqlite3* db);


int main() {


    sqlite3* db;
    int dbStatus = sqlite3_open("theatre.db", &db);

    if (dbStatus) {
        cerr << "Error: Cannot open database" << sqlite3_errmsg(db) << endl;
        return 1;
    }   else {
        cout << "Database connected" << endl;
    }
    int mode_choice;
    cout << "Welcome to the Bella SW Community Theatre Management System" << endl;
    cout << "1. for user mode" << endl;
    cout << "2. for management mode" << endl;
    cin >> mode_choice;
    if (mode_choice == 1) {
        userMode(db);
    }
    else if (mode_choice == 2) {
        manageMode(db);
    }
    else {
        cout << "Error: Invalid option entered. Shutting down." << endl;
        return -1;
    }

    char c;

sqlite3_close(db);
return 0;
}




void viewFinanceReporting(sqlite3* db) {
    // Implementation for viewing finance reporting

    //system("CLS");
}







void userMode(sqlite3* db) {
    int choice;
    cout << "1. View upcoming productions" << endl;
    cout << "2. Buy tickets" << endl;
    cin >> choice;
    if (choice == 1) {
        listProductions(db);
    } else if (choice == 2) {
        listProductions(db);
        purchaseTickets(db);
    }
}

void purchaseTickets(sqlite3* db) {
    int input_id;
    int num_tickets;
    string prod = "production";
    cout << "Enter the ID of the production you want to buy tickets for" << endl;
    cin >> input_id;
    if (exists(db, prod, input_id)) {
        // the production they want to buy tickets for currently exists
        // 
        cout << "Enter the number of tickets you want to buy " << endl;
        cin >> num_tickets;
        string sql = "SELECT s.seat_id, s.seat_row, s.seat_num "
        "FROM seat s "
        "LEFT JOIN ticket t ON s.seat_id = t.seat_id AND t.production_id = ? "
        "WHERE t.ticket_id IS NULL;";
        vector<vector<string>> EmptySeats = executeSQLQuery(db, sql, {to_string(input_id)});

        for (const auto& row : EmptySeats) {
        cout << "Seat ID: " << row[0]
         << " Row: " << row[1]
         << " Seat: " << row[2] << endl;
}

    }
    else {
    cout << "Production ID does not exist" << endl;
    return;
    }

}





void manageMode(sqlite3* db) {

    char c;



        while (c != 'Q' && c != 'q') {

    cout << "1. to edit productions" << endl;
    cout << "2. to edit plays" << endl; 
    cout << "3. to view finance reporting" << endl;
    cout << "4. to edit members" << endl;
    cout << "q. to quit" << endl;
    cin >> c;


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
            viewFinanceReporting(db);
            // Add finance reporting logic here
            break;
        case '4':
            cout << "Editing members..." << endl;
            editMembers(db);
            // Add member editing logic here
            break;
        case 'Q':
        case 'q':
            cout << "Quitting the program." << endl;
            return;
        default:
            cout << "Invalid option, please try again." << endl;
    }
}
return;
}

