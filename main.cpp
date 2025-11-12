#include <iostream>
#include "sqlite3.h"
#include <string>
#include <vector>
#include <cstdlib>
#include <iomanip>
using namespace std;


void addProduction(sqlite3* db);
void editProductions(sqlite3* db);
void deleteProduction(sqlite3* db);
void editPlays(sqlite3* db);
void viewFinanceReporting(sqlite3* db);
void editMembers(sqlite3* db);
void printPlays(sqlite3* db);
void addPlays(sqlite3* db);
void updateMember(sqlite3* db);
void deleteMember(sqlite3* db);
void addMember(sqlite3* db);
void listProductions(sqlite3* db);








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
            return 0;
        default:
            cout << "Invalid option, please try again." << endl;
    }
}
sqlite3_close(db);
return 0;
}


void addProduction(sqlite3* db) {
    string play, production_date, producer_fname, producer_lname;
    
    double base_ticket_price;
    cout << "Enter the name of the play for this production " << endl;
    cin.ignore();
    getline(cin, play);
    cout << "Enter the first name of the producer " << endl;
    getline(cin, producer_fname);
    cout << "Enter the last name of the producer " << endl;
    getline(cin, producer_lname);
    cout << "Enter the base ticket price " << endl;
    cin >> base_ticket_price;
    cout << "Enter the date of the production " << endl;
    cin.ignore();
    getline(cin, production_date);

    int play_id = -1;
    int producer_id = -1;

    

    const char* sql_query_plays = "SELECT play_id FROM play WHERE play_title = ?;";
    sqlite3_stmt* stmt;
   
    if (sqlite3_prepare_v2(db, sql_query_plays, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error: Failed to prepare statement" << sqlite3_errmsg(db) << endl;
        return;
    }


    sqlite3_bind_text(stmt, 1, play.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        play_id = sqlite3_column_int(stmt, 0);
    } else {
        cerr << "Error: Play doesnt exist in the database" << endl;
        sqlite3_finalize(stmt);
        return;
    }
    sqlite3_finalize(stmt);

 



const char* sql_query_names = "SELECT COUNT(*) FROM member where member_fname = ? AND member_lname = ?;";



if (sqlite3_prepare_v2(db, sql_query_names, -1, &stmt, nullptr) != SQLITE_OK) {
    cerr << "Error: Failed to prepare statement " << sqlite3_errmsg(db) << endl;
    return;
}

sqlite3_bind_text(stmt, 1, producer_fname.c_str(), -1, SQLITE_STATIC);
sqlite3_bind_text(stmt, 2, producer_lname.c_str(), -1, SQLITE_STATIC);


if (sqlite3_step(stmt) == SQLITE_ROW) {
    producer_id = sqlite3_column_int(stmt, 0);
} else {
    cerr << "Error producer is not a member in the database" << endl;
    sqlite3_finalize(stmt);
    return;
}




const char* sql_insert_prod = "INSERT INTO production (play_id, producer_id, production_ticket_price, production_date) VALUES (?, ?, ?, ?);";

if (sqlite3_prepare_v2(db, sql_insert_prod, -1, &stmt, nullptr) != SQLITE_OK){
    cerr << "Error preparing statement " << sqlite3_errmsg(db) << endl;
    return;
}

sqlite3_bind_int(stmt, 1, play_id);
sqlite3_bind_int(stmt, 2, producer_id);
sqlite3_bind_int(stmt, 3, base_ticket_price);
sqlite3_bind_text(stmt, 4, production_date.c_str(), -1, SQLITE_STATIC);

if (sqlite3_step(stmt) == SQLITE_DONE) {
    cout << "Production added successfully" << endl;
} else {
    cerr << "Error inserting production" << sqlite3_errmsg(db) << endl;
}
sqlite3_finalize(stmt);


return;
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
void viewFinanceReporting(sqlite3* db) {
    // Implementation for viewing finance reporting

    //system("CLS");
}


/*
EditMembers()
Purpose:
Input:
Output:

*/



void editMembers(sqlite3* db) {
    // Implementation for editing members
    char userinput;
    cout << "Press 1 to add a new member" << endl;
    cout << "Press 2 to delete a member" << endl;
    cout << "Press 3 to update a members information" << endl;
    cout << "Press 4 to return to the previous menu" << endl;
    cin >> userinput;


    switch (userinput) {
        case '1':
            cout << "adding member..." << endl;
            addMember(db);
            break;
        case '2':
            cout << "deleting a member.." << endl;
            deleteMember(db);
            break;
        case '3':
            cout << "update member.." << endl;
            updateMember(db);
            break;
        case '4':
            cout << "returning..." << endl;
            system("CLS");
            break;

    }
    return;

    //system("CLS");
}


void updateMember(sqlite3* db) {
    int choice;
    cout << "Update member by:\n"
         << "1. Member ID\n"
         << "2. First and last name\n> ";
    cin >> choice;

    int id;
    string fname, lname;
    bool searchById = false;

    if (choice == 1) {
        cout << "Enter member ID: ";
        cin >> id;
        searchById = true;
    } else if (choice == 2) {
        cout << "Enter member first name: ";
        cin >> fname;
        cout << "Enter member last name: ";
        cin >> lname;
    } else {
        cout << "Invalid option.\n";
        return;
    }

    cout << "\nWhat would you like to update?\n"
         << "1. Name\n"
         << "2. Email\n"
         << "3. Phone Number\n"
         << "4. Paid Dues\n"
         << "5. All fields\n> ";

    int updateChoice;
    cin >> updateChoice;

    string sql = "UPDATE member SET ";
    string newFname, newLname, newEmail, newPhone;
    int paidDues;

    switch (updateChoice) {
        case 1:
            cout << "Enter new first name: ";
            cin >> newFname;
            cout << "Enter new last name: ";
            cin >> newLname;
            sql += "member_fname = ?, member_lname = ?";
            break;
        case 2:
            cout << "Enter new email: ";
            cin >> newEmail;
            sql += "member_email = ?";
            break;
        case 3:
            cout << "Enter new phone number: ";
            cin >> newPhone;
            sql += "member_phone = ?";
            break;
        case 4:
            cout << "Has the member paid dues? (1 for yes, 0 for no): ";
            cin >> paidDues;
            sql += "paid_dues = ?";
            break;
        case 5:
            cout << "Enter new first name: ";
            cin >> newFname;
            cout << "Enter new last name: ";
            cin >> newLname;
            cout << "Enter new email: ";
            cin >> newEmail;
            cout << "Enter new phone: ";
            cin >> newPhone;
            cout << "Has the member paid dues? (1 for yes, 0 for no): ";
            cin >> paidDues;
            sql += "member_fname = ?, member_lname = ?, member_email = ?, member_phone = ?, paid_dues = ?";
            break;
        default:
            cout << "Invalid choice.\n";
            return;
    }

    sql += searchById ? " WHERE member_id = ?;" : " WHERE member_fname = ? AND member_lname = ?;";

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error preparing SQL: " << sqlite3_errmsg(db) << endl;
        return;
    }

    int bindIndex = 1;

    switch (updateChoice) {
        case 1:
            sqlite3_bind_text(stmt, bindIndex++, newFname.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, bindIndex++, newLname.c_str(), -1, SQLITE_STATIC);
            break;
        case 2:
            sqlite3_bind_text(stmt, bindIndex++, newEmail.c_str(), -1, SQLITE_STATIC);
            break;
        case 3:
            sqlite3_bind_text(stmt, bindIndex++, newPhone.c_str(), -1, SQLITE_STATIC);
            break;
        case 4:
            sqlite3_bind_int(stmt, bindIndex++, paidDues);
            break;
        case 5:
            sqlite3_bind_text(stmt, bindIndex++, newFname.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, bindIndex++, newLname.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, bindIndex++, newEmail.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_text(stmt, bindIndex++, newPhone.c_str(), -1, SQLITE_STATIC);
            sqlite3_bind_int(stmt, bindIndex++, paidDues);
            break;
    }

   
    if (searchById) {
        sqlite3_bind_int(stmt, bindIndex++, id);
    } else {
        sqlite3_bind_text(stmt, bindIndex++, fname.c_str(), -1, SQLITE_STATIC);
        sqlite3_bind_text(stmt, bindIndex++, lname.c_str(), -1, SQLITE_STATIC);
    }

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        cout << "member updated successfully.\n";
    } else {
        cerr  << sqlite3_errmsg(db) << endl;
    }

    sqlite3_finalize(stmt);
}




void addMember(sqlite3* db) {
     const char* sql = "INSERT INTO member (member_fname, member_lname, member_email, member_phone, member_dues_paid) VALUES (?, ?, ?, ?, ?);";
     sqlite3_stmt* stmt;

     if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error: Failed to prepare statement" << sqlite3_errmsg(db) << endl;
        return;
     }

     string fname, lname, email, phone;
     char input_dues;
     bool member_paid_dues;
     cout << "Enter members first name" << endl;
     cin >> ws;
     getline(cin, fname);
     cout << "Enter members last name" << endl;
     getline(cin, lname);
     cout << "Enter members email" << endl;
     getline(cin, email);
     cout << "Enter members phone number" << endl;
     getline(cin, phone);
     while (true) {
     cout << "Enter a 1 if member has paid dues, or 0 if not ";
     cin >> ws;
     input_dues = getchar();
     if (input_dues == '1') {
        member_paid_dues = 1;
        break;
     } else if (input_dues == '0') {
        member_paid_dues = false;
        break;
     } else {
        cout << "Invalid input. Please enter a 1 or a 0" << endl;
     }
     }


    sqlite3_bind_text(stmt, 1, fname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, email.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, phone.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, member_paid_dues ? 1 : 0);

     if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Member added successfully." << endl;
    }
    sqlite3_finalize(stmt);



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
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error: Failed to prepare statement" << sqlite3_errmsg(db) << endl;
        return;
    }

    sqlite3_bind_int(stmt, 1, prod_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Error: Failed to execute production deletion" << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Production deleted successfully" << endl;
    }
    sqlite3_finalize(stmt);
    return;

}




void deleteMember(sqlite3* db) {
    string fname, lname;
    cout << "Enter the first name of the member to delete" << endl;
    cin >> fname;
    cout << "Enter the last name of the member to delete" << endl;
    cin >> lname; 

    const char* sql = "DELETE FROM member WHERE member_fname = ? AND member_lname = ?;";
    sqlite3_stmt* stmt;


    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error: Failed to prepare statement" << sqlite3_errmsg(db) << endl;
        return;
     }

    sqlite3_bind_text(stmt, 1, fname.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, lname.c_str(), -1, SQLITE_STATIC);

      if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Failed to execute statement: " << sqlite3_errmsg(db) << endl;
    } else {
        cout << "Member deleted successfully." << endl;
    }
    sqlite3_finalize(stmt);
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
