#include "finances.h"

std::string runPythonBalanceSheet(int productionID)
{
    std::string cmd =
        "C:/msys64/mingw64/bin/python.exe balance_sheet_procedure.py " +
        std::to_string(productionID);

    std::array<char, 256> buffer{};
    std::string result;

    FILE *pipe = popen(cmd.c_str(), "r");
    if (!pipe)
        return "ERROR: popen failed";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
    {
        result += buffer.data();
    }

    pclose(pipe);
    return result;
}

void parseJSON(int production_id)
{
    std::string jsonText = runPythonBalanceSheet(production_id);

    auto data = nlohmann::json::parse(jsonText);
    std::cout << "Total Production Income " << data["total_income"] << "\n";
    std::cout << "Total Production Expenses " << data["total_expenses"] << "\n";
    std::cout << "Ticket Sales: " << data["ticket_sales"] << "\n";
    std::cout << "Sponsorship Production Credits: " << data["sponsor_prod_creds"] << "\n";
    std::cout << "Sponsorship Advertisement Credits: " << data["sponsor_ad_creds"] << "\n";
    std::cout << "Total Expenses: " << data["expenses"] << "\n";
    return;
}

void viewFinanceReporting(sqlite3 *db)
{
    int user_choice;
    cout << "Press 1 to view total income and spending" << endl;
    cout << "Press 2 to view production finances" << endl;
    cout << "Press 3 to view total member dues" << endl;
    cout << "Press 4 to return to previous menu " << endl;
    cin >> user_choice;

    switch (user_choice)
    {
    case 1:
    {
        totalFinances(db);
        break;
    }

    case 2:
    {
        productionFinances(db);
        break;
    }

    case 3:
    {
        memberDuesFinances(db);
        break;
    }

    case 4:
    {
        return;
    }

    default:
    {
        cerr << "Error: Invalid option entered. Returning to previous screen " << endl;
        return;
    }
    }
}

// will return a vector of <collected, to_collect>
vector<int> getMemberDues(sqlite3 *db)
{
    vector<int> result;
    const char *sql = "SELECT COUNT(*) FROM member WHERE member_dues_paid = ?";
    auto paid = executeSQLQuery(db, sql, {"1"});
    int num_paid = stoi(paid[0][0]);
    int collected = num_paid * 100;
    const char *sql2 = "SELECT COUNT(*) FROM member;"; // get total number of members
    auto rows = executeSQLQuery(db, sql2, {});
    int num_members = stoi(rows[0][0]);
    int num_unpaid = num_members - num_paid;
    int to_collect = num_unpaid * 100;
    result.push_back(collected);
    result.push_back(to_collect);
    return result;
}

void memberDuesFinances(sqlite3 *db)
{
    const char *sql = "SELECT COUNT(*) FROM member;"; // get total number of members
    auto rows = executeSQLQuery(db, sql, {});
    cout << "Total Members: " << rows[0][0] << endl;

    const char *sql2 = "SELECT COUNT(*) FROM member WHERE member_dues_paid = ?";
    auto paid = executeSQLQuery(db, sql2, {"1"});
    cout << "Total Members Paid: " << paid[0][0] << endl;
    cout << "Total Members Unpaid: " << (stoi(rows[0][0]) - stoi(paid[0][0])) << endl;
    return;
}
// its going to return a vector which contains <total_income, total_costs>

vector<int> getAllProductionFinances(sqlite3 *db)
{
    int total_expense = 0;
    int total_income = 0;
    vector<int> result;
    const char *sql = "SELECT production_id FROM production;";
    auto rows = executeSQLQuery(db, sql, {});
    for (const auto &row : rows)
    {
        int prod_id = stoi(row[0]);
        std::string jsonText = runPythonBalanceSheet(prod_id);
        auto data = nlohmann::json::parse(jsonText);
        total_income += data["total_income"].get<int>();
        total_expense += data["expenses"].get<int>();
    }
    result.push_back(total_income);
    result.push_back(total_expense);
    return result;
}

void productionFinances(sqlite3 *db)
{
    int user_choice;
    int prod_id;
    cout << "Production Finance Report" << endl;
    cout << "Press 1 to view all production finance reports" << endl;
    cout << "Press 2 to search finance reports by production ID" << endl;
    cout << "Press 3 to return to the previous screen " << endl;
    cin >> user_choice;

    switch (user_choice)
    {
    case 1:
    {
        // grab all production_ids;
        const char *sql = "SELECT production_id, production_date, play_title "
                          "FROM view_production_list;";

        auto productions = executeSQLQuery(db, sql, {});
        for (const auto &row : productions)
        {
            int production_id = stoi(row[0]);
            string prod_date = row[1];
            string title = row[2];
            cout << "Production ID " << production_id << "\n"
                 << "Play title " + title << "\n"
                 << "Production Date " << prod_date << endl;
            parseJSON(production_id);
            cout << endl;
        }

        break;
        // create a function that loops through all current production ids, and calls the python balance on it
    }

    case 2:
    {
        cout << "Enter production ID" << endl;
        cin >> prod_id;
        parseJSON(prod_id);
        break;
        // call the function normally
    }

    case 3:
    {
        return;
    }

    default:
    {
        cerr << "Error: Invalid input entered. Try again" << endl;
        productionFinances(db);
    }
    }
}
void totalFinances(sqlite3 *db)
{
    // what do we need to do to calculate total finances? add up all production total_incomes, add all member_dues,
    auto prodFinances = getAllProductionFinances(db);
    auto memberDues = getMemberDues(db);
    int total_income = prodFinances[0] + memberDues[0];
    int total_expenses = prodFinances[1];
    cout << "Total Income (member dues + production income): " << total_income << endl;
    cout << "Total Expenses: " << total_expenses << endl;
    cout << "Balance: " << total_income - total_expenses << endl;
    return;
}