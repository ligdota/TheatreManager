#include "play.h"
#include "production.h"
#include "member.h"
#include "purchaseTickets.h"
#include "finances.h"
#include "execute.h"
#include <cctype>
#include <ctime>

void userMode(sqlite3 *db);
void manageMode(sqlite3 *db);
void createViews(sqlite3 *db);
void viewTheatreReports(sqlite3 *db);

void ticketSaleReport(sqlite3 *db);
void sponsorContributionReport(sqlite3 *db);
void productionFinanceReport(sqlite3 *db);
void productionCastReport(sqlite3 *db);
void memberReport(sqlite3 *db);
string getCurrentTime();

string getCurrentTime()
{
    time_t currentTime = time(NULL);

    struct tm *localTime = localtime(&currentTime);

    int year = localTime->tm_year + 1900;
    int month = localTime->tm_mon + 1;
    int day = localTime->tm_mday;

    string date = to_string(year) + "-" + to_string(month) + "-" + to_string(day);
    return date;
}

int main()
{

    sqlite3 *db;
    int dbStatus = sqlite3_open("theatre.db", &db);

    if (dbStatus)
    {
        cerr << "Error: Cannot open database" << sqlite3_errmsg(db) << endl;
        return 1;
    }
    else
    {
        cout << "Database connected" << endl;
    }

    createViews(db);

    int mode_choice;
    cout << "Welcome to the Bella SW Community Theatre Management System" << endl;
    cout << "1. for user mode" << endl;
    cout << "2. for management mode" << endl;
    cin >> mode_choice;
    if (mode_choice == 1)
    {
        userMode(db);
    }
    else if (mode_choice == 2)
    {
        manageMode(db);
    }
    else
    {
        cout << "Error: Invalid option entered. Shutting down." << endl;
        return -1;
    }

    sqlite3_close(db);
    return 0;
}

void createViews(sqlite3 *db)
{

    const char *viewProdList = R"(
CREATE VIEW IF NOT EXISTS view_production_list AS
SELECT
    p.production_id,
    pl.play_title,
    pl.play_author,
    m.member_fname,
    m.member_lname,
    p.production_ticket_price,
    p.production_date
FROM production p
JOIN play   pl ON p.play_id     = pl.play_id
JOIN member m  ON p.producer_id = m.member_id;
)";
    executeSQLStatement(db, viewProdList, {});

    /*
     * View #1: Ticket Sales
     * Provides details about ticket sales, combining ticket details, buyer information, seat assignments, and the related play.
     */
    const char *sql = R"(
CREATE VIEW IF NOT EXISTS view_ticket_sales AS
SELECT t.ticket_id,
       pl.play_title,
       pa.patron_fname || ' ' || pa.patron_lname AS buyer_name,
       pa.patron_email AS buyer_email,
       s.seat_row || s.seat_num AS seat,
       t.ticket_cost,
       t.purchase_date
FROM ticket t
JOIN patron pa ON t.patron_id = pa.patron_id
JOIN seat s ON t.seat_id = s.seat_id
JOIN production p ON t.production_id = p.production_id
JOIN play pl ON p.play_id = pl.play_id;
)";

    executeSQLStatement(db, sql, {});

    /*
     * View #2: Production Cast and Crew
     * Combines information from members, roles, productions, and plays to show who is part of the cast/crew for each production.
     * */
    const char *sql2 = R"(
CREATE VIEW IF NOT EXISTS view_production_cast_crew AS
SELECT p.production_id,
       pl.play_title,
       m.member_id,
       m.member_fname || ' ' || m.member_lname AS member_name,
       r.role_name
FROM member_role mr
JOIN member m ON mr.member_id = m.member_id
JOIN production p ON mr.production_id = p.production_id
JOIN play pl ON p.play_id = pl.play_id
JOIN role r ON mr.role_id = r.role_id;
)";

    executeSQLStatement(db, sql2, {});

    // executeSQLStatement(db, "DROP VIEW IF EXISTS view_member_dues;", {});

    // View for member details for unpaid members

    const char *sql3 = R"(CREATE VIEW  IF NOT EXISTS view_member_dues AS
SELECT member_id,
       member_fname || ' ' || member_lname AS member_name,
       member_email,
       member_phone,
       'Unpaid' AS dues_status
    FROM member
    WHERE member_dues_paid = 0;
)";

    executeSQLStatement(db, sql3, {});
    // View for sponsor donation information for each production
    const char *view4 = R"(CREATE VIEW IF NOT EXISTS view_sponsor_contributions AS
SELECT
    sd.sponsor_id,
    s.sponsor_name,
    s.sponsor_phone,
    s.sponsor_email,
    sd.production_id,
    pl.play_title,
    sd.donation_amount,
    sd.sponsor_ad_creds,
    sd.sponsor_prod_creds
FROM sponsor_donations sd
JOIN sponsor     s  ON sd.sponsor_id    = s.sponsor_id
JOIN production  p  ON sd.production_id = p.production_id
JOIN play        pl ON p.play_id        = pl.play_id;
)";
    executeSQLStatement(db, view4, {});

    // View for production finances for each production
    const char *view5 = R"(CREATE VIEW IF NOT EXISTS view_production_finances AS
SELECT
    p.production_id,
    pl.play_title,
    f.transaction_date,
    tt.transaction_type_name,
    SUM(f.transaction_amount) AS total_amount
FROM finances         f
JOIN transaction_type tt ON f.transaction_type_id = tt.transaction_type_id
JOIN production       p  ON f.production_id       = p.production_id
JOIN play             pl ON p.play_id             = pl.play_id
GROUP BY
    p.production_id,
    pl.play_title,
    f.transaction_date,
    tt.transaction_type_name;
)";
    executeSQLStatement(db, view5, {});

    string date = getCurrentTime();
    const char *member_dues_financial_trigger = R"(CREATE TRIGGER IF NOT EXISTS dues_trigger
AFTER INSERT ON member
WHEN NEW.member_dues_paid = 1
BEGIN
    INSERT INTO finances(transaction_type_id, transaction_amount, transaction_date)
    VALUES (1, 100, datetime('now'));
END;)";
    executeSQLStatement(db, member_dues_financial_trigger, {});
}

void viewTheatreReports(sqlite3 *db)
{
    int user_choice;
    cout << "1. Unpaid Members Report" << endl;
    cout << "2. Production Cast Report" << endl;
    cout << "3. Production Finance Report" << endl;
    cout << "4. Sponsor Contributions Report" << endl;
    cout << "5. Ticket Sale Reports" << endl;
    cin >> user_choice;

    switch (user_choice)
    {

    case 1:
    {
        memberReport(db);
        break;
    }
    case 2:
    {
        productionCastReport(db);
        break;
    }
    case 3:
    {
        productionFinanceReport(db);
        break;
    }
    case 4:
    {
        sponsorContributionReport(db);
        break;
    }
    case 5:
    {
        ticketSaleReport(db);
    }
    }
    return;
}

void manageSubscription(sqlite3 *db)
{
    int user_id;
    cout << "Enter your member id " << endl;
    cin >> user_id;
}

void userMode(sqlite3 *db)
{
    int choice;
    cout << "1. View upcoming productions" << endl;
    cout << "2. Buy tickets" << endl;
    cout << "3. Manage seat subscription" << endl;
    cout << "4. To Exit" << endl;
    cin >> choice;

    if (choice == 1)
    {
        listProductions(db);
        userMode(db);
    }
    else if (choice == 2)
    {
        purchaseTickets(db);
        userMode(db);
    }
    else if (choice == 3)
    {
        manageSubscription(db);
        return;
    }
    else if (choice == 4)
    {
        return;
    }
    else
    {
        cout << "Invalid option entered try again " << endl;
        userMode(db);
    }
}

void manageMode(sqlite3 *db)
{

    char c;

    while (c != 'Q' && c != 'q')
    {

        cout << "1. To Edit Productions" << endl;
        cout << "2. To Edit Plays" << endl;
        cout << "3. To Edit Members" << endl;
        cout << "4. To View Finance Reporting" << endl;
        cout << "5. To View Theatre Reports" << endl;
        cout << "q. To Quit" << endl;
        cin >> c;

        switch (c)
        {
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
            cout << "Editing members..." << endl;
            editMembers(db);
            // Add member editing logic here
            break;
        case '4':
            cout << "Viewing finance reporting..." << endl;
            viewFinanceReporting(db);
            // Add finance reporting logic here
            break;

        case '5':
            cout << "Viewing Theatre Reports..." << endl;
            viewTheatreReports(db);
            break;
        case 'Q':
        case 'q':
            cout << "Quitting the program." << endl;
            return;
        default:
            cout << "Invalid option, please try again." << endl;
            manageMode(db);
        }
    }
    return;
}

// 1) Ticket Sales Report – uses view_ticket_sales
void ticketSaleReport(sqlite3 *db)
{
    cout << "\n=== Ticket Sales Report ===\n";

    const char *ticket_sale_info =
        "SELECT play_title, buyer_name, buyer_email, seat, "
        "       ticket_cost, purchase_date "
        "FROM view_ticket_sales "
        "ORDER BY play_title, purchase_date;";

    auto rows = executeSQLQuery(db, ticket_sale_info, {});
    if (rows.empty())
    {
        cout << "No ticket sales found.\n";
        return;
    }

    for (const auto &row : rows)
    {
        cout << "Play Title   : " << row[0] << '\n';
        cout << "Buyer Name   : " << row[1] << '\n';
        cout << "Buyer Email  : " << row[2] << '\n';
        cout << "Seat         : " << row[3] << '\n';
        cout << "Ticket Cost  : " << row[4] << '\n';
        cout << "Purchase Date: " << row[5] << "\n";
        cout << "-----------------------------\n";
    }
}

// 2) Sponsor Contributions Report – uses view_sponsor_contributions
void sponsorContributionReport(sqlite3 *db)
{
    cout << "\n=== Sponsor Contributions Report ===\n";

    const char *sponsor_info =
        "SELECT sponsor_name, sponsor_phone, sponsor_email, "
        "       production_id, play_title, donation_amount, "
        "       sponsor_ad_creds, sponsor_prod_creds "
        "FROM view_sponsor_contributions "
        "ORDER BY production_id, sponsor_name;";

    auto rows = executeSQLQuery(db, sponsor_info, {});
    if (rows.empty())
    {
        cout << "No sponsor contributions found.\n";
        return;
    }

    int currentProd = -1;
    for (const auto &row : rows)
    {
        int production_id = stoi(row[3]);
        const string &play_title = row[4];
        const string &sponsor_name = row[0];
        const string &sponsor_phone = row[1];
        const string &sponsor_email = row[2];
        const string &donation = row[5];
        const string &ad_creds = row[6];
        const string &prod_creds = row[7];

        if (production_id != currentProd)
        {
            currentProd = production_id;
            cout << "\n--- Production " << production_id
                 << " ---" << play_title << " ---\n";
        }

        cout << "Sponsor Name : " << sponsor_name << '\n';
        cout << "Phone        : " << sponsor_phone << '\n';
        cout << "Email        : " << sponsor_email << '\n';
        cout << "Donation     : " << donation << '\n';
        cout << "Ad Credits   : " << ad_creds << '\n';
        cout << "Program Creds: " << prod_creds << "\n";
        cout << "-----------------------------\n";
    }
}

// 3) Production Finance Report – uses view_production_finances
void productionFinanceReport(sqlite3 *db)
{
    cout << "\n=== Production Finance Report ===\n";

    const char *fin_info =
        "SELECT production_id, play_title, transaction_date, "
        "       transaction_type_name, total_amount "
        "FROM view_production_finances "
        "ORDER BY production_id, transaction_date, transaction_type_name;";

    auto rows = executeSQLQuery(db, fin_info, {});
    if (rows.empty())
    {
        cout << "No finance records found.\n";
        return;
    }

    int currentProd = -1;
    string currentTitle;

    for (const auto &row : rows)
    {
        int production_id = stoi(row[0]);
        const string &play_title = row[1];
        const string &date = row[2];
        const string &type_name = row[3];
        const string &amount = row[4];

        if (production_id != currentProd)
        {
            currentProd = production_id;
            currentTitle = play_title;
            cout << "\n--- Production " << production_id
                 << " ---" << currentTitle << " ---\n";
        }

        cout << date << " | " << type_name
             << " | Amount: " << amount << '\n';
    }
}

// 4) Production Cast & Crew Report – uses view_production_cast_crew
void productionCastReport(sqlite3 *db)
{
    cout << "\n=== Production Cast & Crew Report ===\n";

    const char *production_cast =
        "SELECT production_id, play_title, member_id, "
        "       member_name, role_name "
        "FROM view_production_cast_crew "
        "ORDER BY production_id, role_name, member_name;";

    auto rows = executeSQLQuery(db, production_cast, {});
    if (rows.empty())
    {
        cout << "No cast/crew assignments found.\n";
        return;
    }

    int currentProd = -1;
    string currentTitle;

    for (const auto &row : rows)
    {
        int production_id = stoi(row[0]);
        const string &play_title = row[1];
        const string &member_id = row[2];
        const string &member = row[3];
        const string &role = row[4];

        if (production_id != currentProd)
        {
            currentProd = production_id;
            currentTitle = play_title;
            cout << "\n--- Production " << production_id
                 << " ---" << currentTitle << " ---\n";
        }

        cout << "Member ID: " << member_id
             << " | " << member
             << " | Role: " << role << '\n';
    }
}

// 5) Unpaid Member Dues Report – uses view_member_dues
void memberReport(sqlite3 *db)
{
    cout << "\n=== Unpaid Member Dues Report ===\n";

    const char *member_dues_info =
        "SELECT member_name, member_email, member_phone, dues_status "
        "FROM view_member_dues "
        "ORDER BY dues_status, member_name;";

    auto rows = executeSQLQuery(db, member_dues_info, {});
    if (rows.empty())
    {
        cout << "All member dues are paid.\n";
        return;
    }

    for (const auto &row : rows)
    {
        cout << "Member Name : " << row[0] << '\n';
        cout << "Email       : " << row[1] << '\n';
        cout << "Phone       : " << row[2] << '\n';
        cout << "Dues Status : " << row[3] << "\n";
        cout << "-----------------------------\n";
    }
}

vector<string> suggestBlockSeats(bool availableMap[26][51], int numTickets)
{
    vector<string> result;

    // Scan rows from front (A) to back (Z)
    for (int rowIdx = 0; rowIdx < 26; ++rowIdx)
    {
        int consecutive = 0;
        int startSeat = 1;

        for (int seat = 1; seat <= 50; ++seat)
        {
            if (availableMap[rowIdx][seat])
            {
                if (consecutive == 0)
                    startSeat = seat;
                consecutive++;

                if (consecutive == numTickets)
                {
                    // Found a block
                    char rowChar = 'A' + rowIdx;
                    for (int s = startSeat; s < startSeat + numTickets; ++s)
                    {
                        result.push_back(string(1, rowChar) + to_string(s));
                    }
                    return result;
                }
            }
            else
            {
                consecutive = 0;
            }
        }
    }

    // If no contiguous block exists, just take any numTickets seats
    for (int rowIdx = 0; rowIdx < 26 && (int)result.size() < numTickets; ++rowIdx)
    {
        for (int seat = 1; seat <= 50 && (int)result.size() < numTickets; ++seat)
        {
            if (availableMap[rowIdx][seat])
            {
                char rowChar = 'A' + rowIdx;
                result.push_back(string(1, rowChar) + to_string(seat));
            }
        }
    }

    return result; // Will be non-empty as long as enough seats exist
}
