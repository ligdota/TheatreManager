#include "production.h"

void editProductions(sqlite3 *db)
{
    // connect to the database, fetch production data, and allow editing
    // Implementation for editing productions
    cout << "press 1 to add a production" << endl;
    cout << "press 2 to delete a production" << endl;
    cout << "press 3 to update a production" << endl;
    cout << "press 4 to view all productions" << endl;
    cout << "Press 5 to return to the previous menu " << endl;
    char choice;
    cin >> choice;
    switch (choice)
    {
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
        // printProductions(db);
        listProductions(db);
        editProductions(db);
        break;

    default:
        cout << "Returning to previous menu..." << endl;
        return;
    }
    return;
    // system("CLS");
}

void deleteProduction(sqlite3 *db)
{
    char input;
    int prod_id;
    cout << "press 1 to view a list of productions" << endl;
    cout << "press 2 to delete a production by id " << endl;
    cin >> input;

    if (input == '1')
    {
        listProductions(db);
        deleteProduction(db);
    }
    cout << "Enter the id of the production to delete" << endl;
    cin >> prod_id;

    const char *sql = "DELETE FROM production WHERE production_id = ?;";
    executeSQLStatement(db, sql, {to_string(prod_id)});
    return;
}

void updateProduction(sqlite3 *db)
{
    listProductions(db);

    int production_id;
    cout << "Enter the ID of the production to update" << endl;
    cin >> production_id;
    string prodTable = "production";
    if (!exists(db, prodTable, production_id))
    {
        cerr << "Error: Production id doesnt exist " << endl;
        return;
    }

    auto result = executeSQLQuery(db, "SELECT play_id, producer_id, production_ticket_price, production_date FROM production WHERE production_id = ?",
                                  {to_string(production_id)});

    int play_id = stoi(result[0][0]);
    int producer_id = stoi(result[0][1]);
    double ticket_price = stoi(result[0][2]);
    string production_date = result[0][3];

    int update_choice;
    cout << "Press 1 to update the production play ID" << endl
         << "Press 2 to update the production producer ID" << endl
         << "Press 3 to update the production ticket price" << endl
         << "Press 4 to update the production date " << endl
         << "Press 5 to update all above choices " << endl;
    cin >> update_choice;

    switch (update_choice)
    {
    case 1:
    {
        printPlays(db);
        cout << "Enter new Play ID" << endl;
        cin >> play_id;
        break;
    }
    case 2:
    {
        printMembers(db);
        cout << "Enter new Producer ID " << endl;
        cin >> producer_id;
        break;
    }

    case 3:
    {
        cout << "Enter new ticket price " << endl;
        cin >> ticket_price;
        break;
    }
    case 4:
    {
        cout << "Enter new production date in the format (MM/DD/YYYY) " << endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, production_date);
        break;
    }
    case 5:
    {
        printPlays(db);
        cout << "Enter new play ID " << endl;
        cin >> play_id;
        string playTable = "play";

        printMembers(db);
        cout << "Enter new Producers ID  " << endl;
        cin >> producer_id;

        cout << "Enter new ticket price " << endl;
        cin >> ticket_price;

        cout << "Enter the new production date in the format (MM/DD/YYYY)" << endl;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        getline(cin, production_date);
        break;
    }
    default:
    {
        cout << "Invalid input entered. Returning to previous menu" << endl;
        return;
    }
    }

    string playTable = "play";
    string memberTable = "member";

    if (!exists(db, playTable, play_id) || !exists(db, memberTable, producer_id))
    {
        cerr << "Error: Invalid play id or invalid producer id.. Returning" << endl;
        return;
    }

    const char *sql = "UPDATE production SET play_id = ?, producer_id = ?, production_ticket_price = ?, production_date = ? "
                      "WHERE production_id = ?;";

    if (executeSQLStatement(db, sql,
                            {to_string(play_id),
                             to_string(producer_id),
                             to_string(ticket_price),
                             production_date,
                             to_string(production_id)}))
    {
        cout << "Production updated successfully " << endl;
    }
    else
    {
        cerr << "Error: Updating production please try again " << endl;
        return;
    }

    return;
}

void addProduction(sqlite3 *db)
{
    string production_date;
    int play_id, producer_id;
    double base_ticket_price;

    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    printPlays(db);
    cout << "Enter the ID of the play for this production " << endl;
    cin >> play_id;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    printMembers(db);
    cout << "Enter the ID of the member who will produce this production" << endl;
    cin >> producer_id;
    cout << "Enter the base ticket price " << endl;
    cin >> base_ticket_price;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    cout << "Enter the date of the production in the format: (MM/DD/YYYY) " << endl;
    getline(cin, production_date);

    // check if the play the user entered exists in the db
    // const char* sql = "SELECT EXISTS(SELECT 1 FROM play WHERE play_title = ?);";
    auto play = executeSQLQuery(db, "SELECT 1 FROM play WHERE play_id = ?;", {to_string(play_id)});
    if (play.empty())
    {
        cout << "Play doesnt exist in the DB. Please check play id and try again " << endl;
        return;
    }

    auto producer = executeSQLQuery(db, "SELECT 1 FROM member WHERE member_id = ?;", {to_string(producer_id)});
    if (producer.empty())
    {
        cout << "Producer ID doesnt exist in the database. Please check producers id and try again " << endl;
        return;
    }

    const char *sql = "INSERT INTO production (play_id, producer_id, production_ticket_price, production_date) VALUES (?, ?, ?, ?);";
    if (executeSQLStatement(db, sql, {to_string(play_id), to_string(producer_id), to_string(base_ticket_price), production_date}))
    {
        cout << "Production added " << endl;
    }
    else
    {
        cout << "Error adding production please try again " << endl;
        return;
    }

    return;
}

void listProductions(sqlite3 *db)
{
    const char *sql =
        "SELECT production_id, play_title, play_author, "
        "member_fname, member_lname, "
        "production_ticket_price, production_date "
        "FROM view_production_list;";

    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK)
    {
        cerr << "Error preparing statement: " << sqlite3_errmsg(db) << endl;
        return;
    }

    cout << left << setw(5) << "ID"
         << setw(40) << "Play Title"
         << setw(40) << "Author"
         << setw(20) << "Producer"
         << setw(10) << "Price"
         << setw(15) << "Date" << endl;
    cout << string(95, '-') << endl;

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int id = sqlite3_column_int(stmt, 0);
        string title = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));
        string author = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 2));
        string fname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 3));
        string lname = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 4));
        double price = sqlite3_column_double(stmt, 5);
        string date = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 6));

        cout << left << setw(5) << id
             << setw(40) << title
             << setw(40) << author
             << setw(20) << (fname + " " + lname)
             << setw(10) << fixed << setprecision(2) << price
             << setw(15) << date << endl;
    }

    sqlite3_finalize(stmt);
}
