#include "member.h"

void editMembers(sqlite3* db) {
    // Implementation for editing members
    char userinput;
    cout << "Press 1 to add a new member" << endl;
    cout << "Press 2 to delete a member" << endl;
    cout << "Press 3 to update a members information" << endl;
    cout << "Press 4 to print information for all members" << endl;
    cout << "Press 5 to return to the previous menu" << endl;
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
            printMembers(db);
            editMembers(db);

        case '5':
            return;

        default:
            cout << "Invalid input entered returning to previous menu" << endl;
            return;

    }
    return;

    //system("CLS");
}



void updateMember(sqlite3* db) {
    int choice;
    printMembers(db);
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
         << "1. First name\n"
         << "2. Last name\n"
         << "3. Email\n"
         << "4. Phone Number\n"
         << "5. Paid Dues\n"
         << "6. All fields\n> ";

    int updateChoice;
    cin >> updateChoice;

    string sql = "UPDATE member SET ";
    vector<string> executeParams;
    string newFname, newLname, newEmail, newPhone;
    int paidDues;

    switch (updateChoice) {
        case 1:
            cout << "Enter new first name: ";
            cin >> newFname;
            sql += "member_fname = ?";
            executeParams.push_back(newFname);
            break;
            case 2:
            cout << "Enter new last name: ";
            cin >> newLname;
            sql += "member_lname = ?";
            executeParams.push_back(newLname);
            break;
            case 3:
            cout << "Enter new email: ";
            cin >> newEmail;
            sql += "member_email = ?";
            executeParams.push_back(newEmail);
            break;
            case 4:
            cout << "Enter new phone number: ";
            cin >> newPhone;
            sql += "member_phone = ?";
            executeParams.push_back(newPhone);
            break;
            case 5:
            cout << "Has the member paid dues? (1 for yes, 0 for no): ";
            cin >> paidDues;
            sql += "paid_dues = ?";
            executeParams.push_back(to_string(paidDues));
            break;
            case 6:
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
            sql += "member_fname = ?, member_lname = ?, member_email = ?, member_phone = ?, member_dues_paid = ?";
            executeParams.push_back(newFname);
            executeParams.push_back(newLname);
            executeParams.push_back(newEmail);
            executeParams.push_back(newPhone);
            executeParams.push_back(to_string(paidDues));
            break;

            default:
            cout << "Invalid choice.\n";
            return;
    }

    if (searchById) {
        sql += " WHERE member_id = ?;";
        executeParams.push_back(to_string(id));
    } else {
        sql += " WHERE member_fname = ? AND member_lname = ?;";
        executeParams.push_back(fname);
        executeParams.push_back(lname);
    
    }

    if (executeSQLStatement(db, sql, executeParams)) {
        cout << "Member updated successfully.\n";
    } else {
        cerr << "Failed to update member.\n";
    }
    return;
}



void deleteMember(sqlite3* db) {
    int user_choice, delete_id;
    string sql = "DELETE FROM member WHERE ";
    printMembers(db);
    fflush(stdin);

    string fname, lname;
    cout << "1. Delete by id" << endl
         << "2. Delete by first and last name" << endl;

    cin >> user_choice;
    fflush(stdin);
    if (user_choice == 1) {
        cout << "Enter the id of the member to delete" << endl;
        cin >> delete_id;
        sql += " member_id = ?;";
        fflush(stdin);
        if (executeSQLStatement(db, sql.c_str(), {to_string(delete_id)})) {
            cout << "Member " << delete_id << " deleted " << endl;
        } else {
            cout << "Failed to delete member " << delete_id << endl;
        }
    } else if (user_choice == 2) {
    
    cout << "Enter the first name of the member to delete" << endl;
    getline(cin, fname);
    cout << "Enter the last name of the member to delete" << endl;
    getline(cin, lname);
    sql += " member_fname = ? AND member_lname = ?;";
    executeSQLStatement(db, sql.c_str(), {fname, lname});
    } else {
        cout << "Invalid option entered.. Returning to the previous screen" << endl;
    }
    
    return;

}



void addMember(sqlite3* db) {
    
     string fname, lname, email, phone;
     int dues_paid;
    
     cout << "Enter members first name" << endl;
     cin >> ws;
     getline(cin, fname);
     cout << "Enter members last name" << endl;
     getline(cin, lname);
     cout << "Enter members email" << endl;
     getline(cin, email);
     cout << "Enter members phone number" << endl;
     getline(cin, phone);
     cout << "Enter (1/0) if member has paid dues" << endl;
     cin >> dues_paid;

     const char* sql = "INSERT INTO member (member_fname, member_lname, member_email, member_phone, member_dues_paid) VALUES (?, ?, ?, ?, ?);";
    executeSQLStatement(db, sql, {fname, lname, email, phone, to_string(dues_paid)});

    if (dues_paid == 1) {
        const char* sql = "INSERT INTO finances (production_id, transaction_type_id, transaction_amount, transaction_date)";
        // insert a row into financialsTable

    }



    return;

}



void printMembers(sqlite3* db) {
    const int w_id = 5;
    const int w_name = 20;
    const int w_email = 20;
    const int w_phone = 15;
    const int w_dues = 5;

    cout << left;
    cout << setw(w_id) << "ID"
         << setw(w_name) << "First Name"
         << setw(w_name) << "Last Name"
         << setw(w_email) << "Email"
         << setw(w_phone) << "Phone"
         << setw(w_dues) << "Dues Paid"
         << endl;

         cout << string(5 + 20 + 20 + 25 + 15 + 5, '-') << endl;


    const char* sql = "SELECT member_id, member_fname, member_lname, member_email, member_phone, member_dues_paid FROM member;";
    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Failed to prepare statement: " << sqlite3_errmsg(db) << endl;
        return;
    }



    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id = sqlite3_column_int(stmt, 0);
        const unsigned char* fname = sqlite3_column_text(stmt, 1);
        const unsigned char* lname = sqlite3_column_text(stmt, 2);
        const unsigned char* email = sqlite3_column_text(stmt, 3);
        const unsigned char* phone = sqlite3_column_text(stmt, 4);
        int dues_paid = sqlite3_column_int(stmt, 5);
        string int_to_bool;
        if (dues_paid == 1) {
            int_to_bool = "True";
        }
        else {
            int_to_bool = "False";
        }
        cout << left
             << setw(w_id)    << id
             << setw(w_name)  << fname
             << setw(w_name)  << lname
             << setw(w_email) << email
             << setw(w_phone) << phone
             << setw(w_dues)  << dues_paid
             << endl;
    }
    sqlite3_finalize(stmt);
}



