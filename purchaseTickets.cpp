#include "purchaseTickets.h"
#include "execute.h"

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <cmath>

using namespace std;

/*
The code in this file was created using ChatGPT 5.1
*/

struct Seat
{
    int seatId;
    char row;
    int num;

    string code() const
    {
        return string(1, row) + to_string(num); // e.g. "A10"
    }
};

int seatDistance(const Seat &a, const Seat &b)
{
    int ra = a.row - 'A';
    int rb = b.row - 'A';
    return std::abs(ra - rb) + std::abs(a.num - b.num);
}

Seat parseSeatCodeDummy(const string &code)
{
    Seat s;
    s.seatId = -1;
    s.row = code[0];
    s.num = stoi(code.substr(1));
    return s;
}

void purchaseTickets(sqlite3 *db)
{
    int production_id;
    int patron_id;
    int num_tickets;

    // 1. Ask for production
    cout << "Enter the ID of the production you want to buy tickets for: ";
    cin >> production_id;

    string prodTable = "production";
    if (!exists(db, prodTable, production_id))
    {
        cout << "Error: Production with ID " << production_id
             << " does not exist. Aborting purchase.\n";
        return;
    }

    // 2. Ask for patron
    cout << "Enter your Patron ID: ";
    cin >> patron_id;

    string patronTable = "patron";
    if (!exists(db, patronTable, patron_id))
    {
        cout << "Error: Patron with ID " << patron_id
             << " does not exist. Aborting purchase.\n";
        return;
    }

    // 3. Fetch ticket price once
    double ticketPrice = 0.0;
    {
        const string priceSql =
            "SELECT production_ticket_price "
            "FROM production "
            "WHERE production_id = ?;";

        vector<vector<string>> priceRows =
            executeSQLQuery(db, priceSql, {to_string(production_id)});

        if (priceRows.empty())
        {
            cout << "Error: Could not find ticket price for this production.\n";
            return;
        }

        ticketPrice = stod(priceRows[0][0]); // store as double; we’ll stringify later
    }

    // 4. Number of tickets
    cout << "Enter the number of tickets you want to buy: ";
    cin >> num_tickets;
    if (num_tickets <= 0)
    {
        cout << "Error: Number of tickets must be positive.\n";
        return;
    }

    // 5. Get desired seats
    vector<string> requestedSeatCodes;
    requestedSeatCodes.reserve(num_tickets);

    for (int i = 0; i < num_tickets; ++i)
    {
        char row;
        int num;

        while (true)
        {
            cout << "Enter seat ROW (A-Z) for ticket #" << (i + 1) << ": ";
            cin >> row;
            row = toupper(row);
            if (row >= 'A' && row <= 'Z')
                break;
            cout << "Error: Invalid row. Please enter a letter from A to Z.\n";
        }

        while (true)
        {
            cout << "Enter seat NUMBER (1-50) for ticket #" << (i + 1) << ": ";
            cin >> num;
            if (num >= 1 && num <= 50)
                break;
            cout << "Error: Invalid seat number. Please enter 1 to 50.\n";
        }

        string code = string(1, row) + to_string(num); // e.g. "A10"
        requestedSeatCodes.push_back(code);
    }

    // 6. Load all available seats for this production
    const string sqlSeats =
        "SELECT s.seat_id, s.seat_row, s.seat_num "
        "FROM seat s "
        "WHERE NOT EXISTS ("
        "    SELECT 1 FROM ticket t "
        "    WHERE t.seat_id = s.seat_id AND t.production_id = ?"
        ") "
        "ORDER BY s.seat_row, s.seat_num;";

    vector<vector<string>> available_rows =
        executeSQLQuery(db, sqlSeats, {to_string(production_id)});

    if (available_rows.empty())
    {
        cout << "No seats are available for this production.\n";
        return;
    }

    vector<Seat> availableSeats;
    availableSeats.reserve(available_rows.size());
    unordered_map<string, int> codeToIdx; // "A10" -> index

    for (const auto &row : available_rows)
    {
        Seat s;
        s.seatId = stoi(row.at(0)); // seat_id
        s.row = row.at(1)[0];       // seat_row
        s.num = stoi(row.at(2));    // seat_num
        string code = s.code();
        codeToIdx[code] = static_cast<int>(availableSeats.size());
        availableSeats.push_back(s);
    }

    if (availableSeats.size() < static_cast<size_t>(num_tickets))
    {
        cout << "Not enough seats are available for this production.\n";
        return;
    }

    // 7. Determine which requested seats are free
    vector<Seat> confirmedSeats;         // requested & available
    vector<string> unavailableCodes;     // requested but not free
    unordered_set<string> reservedCodes; // avoid double-using seats (for alternates)

    for (const string &code : requestedSeatCodes)
    {
        auto it = codeToIdx.find(code);
        if (it != codeToIdx.end())
        {
            const Seat &s = availableSeats[it->second];
            confirmedSeats.push_back(s);
            reservedCodes.insert(code);
        }
        else
        {
            unavailableCodes.push_back(code);
        }
    }

    // INSERT template: no more subquery inside
    const char *insertTicketSql =
        "INSERT INTO ticket "
        "(production_id, patron_id, seat_id, ticket_cost, purchase_date) "
        "VALUES (?, ?, ?, ?, datetime('now'));";

    string ticketPriceStr = to_string(ticketPrice); // bind as text

    // ------------------------------------------------------------------
    // Case 1: all requested seats are available
    // ------------------------------------------------------------------
    if (unavailableCodes.empty())
    {
        cout << "\nAll requested seats are available:\n";
        for (const auto &s : confirmedSeats)
        {
            cout << "  " << s.code() << '\n';
        }

        cout << "\nDo you want to complete this purchase? (Y/N): ";
        char confirm;
        cin >> confirm;
        confirm = toupper(confirm);

        if (confirm != 'Y')
        {
            cout << "Purchase cancelled. No tickets were created.\n";
            return; // <-- no INSERTs happened
        }

        // Now insert tickets
        for (const auto &seat : confirmedSeats)
        {
            bool ok = executeSQLStatement(
                db,
                insertTicketSql,
                {to_string(production_id),
                 to_string(patron_id),
                 to_string(seat.seatId),
                 ticketPriceStr});

            if (!ok)
            {
                cerr << "Error inserting ticket for seat " << seat.code() << ".\n";
                return;
            }
        }

        cout << "\n=== Tickets ===\n";
        for (const auto &seat : confirmedSeats)
        {
            cout << "Ticket - Production ID: " << production_id
                 << ", Patron ID: " << patron_id
                 << ", Seat: " << seat.code() << '\n';
        }
        cout << "================\n";
        return;
    }

    // ------------------------------------------------------------------
    // Case 2: some requested seats are NOT available → suggest alternates
    // ------------------------------------------------------------------
    cout << "\nThe following requested seats are NOT available:\n";
    for (const auto &code : unavailableCodes)
    {
        cout << "  " << code << '\n';
    }

    vector<Seat> alternateSeats;
    alternateSeats.reserve(unavailableCodes.size());
    bool anyMissing = false;

    for (const auto &badCode : unavailableCodes)
    {
        Seat target = parseSeatCodeDummy(badCode);

        bool found = false;
        Seat best{};
        int bestDist = 0;

        for (const auto &cand : availableSeats)
        {
            string candCode = cand.code();
            if (reservedCodes.count(candCode) > 0)
                continue;

            int dist = seatDistance(target, cand);
            if (!found || dist < bestDist)
            {
                found = true;
                best = cand;
                bestDist = dist;
            }
        }

        if (found)
        {
            alternateSeats.push_back(best);
            reservedCodes.insert(best.code());
        }
        else
        {
            alternateSeats.push_back(Seat{-1, '?', -1});
            anyMissing = true;
        }
    }

    cout << "\nSuggested alternate seats:\n";
    for (size_t i = 0; i < unavailableCodes.size(); ++i)
    {
        if (alternateSeats[i].seatId == -1)
        {
            cout << "  Requested " << unavailableCodes[i]
                 << " -> No alternate found\n";
        }
        else
        {
            cout << "  Requested " << unavailableCodes[i]
                 << " -> Suggested " << alternateSeats[i].code() << '\n';
        }
    }

    if (anyMissing)
    {
        cout << "\nNot all requested seats could be replaced with alternates.\n"
             << "Please try a different selection.\n";
        return; // <-- still no INSERTs
    }

    cout << "\nDo you want to accept these alternate seats and complete purchase? (Y/N): ";
    char choice;
    cin >> choice;
    choice = toupper(choice);

    if (choice != 'Y')
    {
        cout << "Purchase cancelled. No tickets were created.\n";
        return; // <-- still no INSERTs
    }

    // Final list: confirmed requested seats + alternates
    vector<Seat> finalSeats = confirmedSeats;
    finalSeats.insert(finalSeats.end(), alternateSeats.begin(), alternateSeats.end());

    // Insert tickets now
    for (const auto &seat : finalSeats)
    {
        bool ok = executeSQLStatement(
            db,
            insertTicketSql,
            {to_string(production_id),
             to_string(patron_id),
             to_string(seat.seatId),
             ticketPriceStr});

        if (!ok)
        {
            cerr << "Error inserting ticket for seat " << seat.code() << ".\n";
            return;
        }
    }

    cout << "\n=== Tickets ===\n";
    for (const auto &seat : finalSeats)
    {
        cout << "Ticket - Production ID: " << production_id
             << ", Patron ID: " << patron_id
             << ", Seat: " << seat.code() << '\n';
    }
    cout << "================\n";
}
