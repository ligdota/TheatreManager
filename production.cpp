#include "production.h"

void editProductions(sqlite3* db) {
    // connect to the database, fetch production data, and allow editing
    // Implementation for editing productions
    cout << "press 1 to add a production" << endl;
    cout << "press 2 to delete a production" << endl;
    cout << "press 3 to update a production" << endl;
    cout << "press 4 to view all productions" << endl;
    cout << "Press 5 to return to the previous menu " << endl;
    char choice;
    cin >> choice;
    switch(choice) {
        case '1':
            cout << "Adding a production..." << endl;
            addProduction(db);
            // Add logic to add a production
            break;
        case '2':
            cout << "Deleting a production..." << endl;
            deleteProduction(db);
            // Add logic to delete a production
            break;
        case '3':
            cout << "Updating a production..." << endl;
            updateProduction(db);
            // Add logic to update a production
            break;
        case '4':
            //printProductions(db);
            listProductions(db);
            editProductions(db);
            break;

        default:
            cout << "Returning to previous menu..." << endl;
            return;
    }
    return;
    //system("CLS");

}
#include "production.h"

void deleteProduction(sqlite3* db) {
    char input;
    int prod_id;
    cout << "press 1 to view a list of productions" << endl;
    cout << "press 2 to delete a production by id " << endl;
    cin >> input;

    if (input == '1') {
        listProductions(db);
    }
    cout << "Enter the id of the production to delete" << endl;
    cin >> prod_id;

    const char* sql = "DELETE FROM production WHERE production_id = ?;";
    executeSQLStatement(db, sql, {to_string(prod_id)});
    return;

}

void updateProduction(sqlite3* db) {
    listProductions(db);
    int production_id, play_id, producer_id;
    double ticket_price;
    string production_date;
    cout << "Enter the ID of the production to update" << endl;
    cin >> production_id;
    printPlays(db);
    cout << "Enter the new play id for production " << production_id << endl;
    cin >> play_id;
    cout << "Enter the new producer for production " << production_id << endl;
    cin >> producer_id;
    cout << "Enter the production ticket price for production " << production_id << endl;
    cin >> ticket_price;
    cout << "Enter the date production " << production_id << " will happen" << endl;
    getline(cin, production_date);
    const char* sql = "UPDATE production SET play_id = ?, producer_id = ?, production_ticket_price = ?, production_date = ? WHERE production_id = ?;";
    executeSQLStatement(db, sql, {to_string(production_id), to_string(play_id), to_string(producer_id), to_string(ticket_price), production_date});


}




void addProduction(sqlite3* db) {
    string play, production_date, producer_fname, producer_lname;
    
    double base_ticket_price;
    fflush(stdin);
    cout << "Enter the name of the play for this production " << endl;
    getline(cin, play);
    cout << "Enter the first name of the producer " << endl;
    getline(cin, producer_fname);
    cout << "Enter the last name of the producer " << endl;
    getline(cin, producer_lname);
    cout << "Enter the base ticket price " << endl;
    cin >> base_ticket_price;
    cout << "Enter the date of the production in the format: (MM/DD/YYYY) " << endl;
    getline(cin, production_date);

    return;
}




  






void listProductions(sqlite3* db) {
    const char* sql =
        "SELECT production.production_id, play.play_title, play.play_author, "
        "member.member_fname, member.member_lname, "
        "production.production_ticket_price, production.production_date "
        "FROM production "
        "JOIN play ON production.play_id = play.play_id "
        "JOIN member ON production.producer_id = member.member_id;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << left << setw(5) << "ID"
         << setw(25) << "Play Title"
         << setw(20) << "Author"
         << setw(20) << "Producer"
         << setw(10) << "Price"
         << setw(15) << "Date" << endl;
    cout << string(95, '-') << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        string title = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        string author = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
        string fname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3));
        string lname = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));
        double price = sqlite3_column_double(stmt, 5);
        string date = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6));

        cout << left << setw(5) << id
             << setw(25) << title
             << setw(20) << author
             << setw(20) << (fname + " " + lname)
             << setw(10) << fixed << setprecision(2) << price
             << setw(15) << date << endl;
    }

    sqlite3_finalize(stmt);
}
