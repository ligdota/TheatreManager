#include "execute.h"

using namespace std;
bool executeSQLStatement(sqlite3* db, const string& sql, const vector<string>& sqlParams) {

    sqlite3_stmt* stmt;
    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error: Preparing SQL statement" << sqlite3_errmsg(db) << endl;
        return false;
    }

    for (size_t i = 0; i < sqlParams.size(); i++) {
        sqlite3_bind_text(stmt, i + 1, sqlParams[i].c_str(), -1, SQLITE_TRANSIENT);
    }

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        cerr << "Error: Invalid SQL statement, ensure you are providing appropriate types for each field " << endl;
        sqlite3_finalize(stmt);
        return false;
    }
    sqlite3_finalize(stmt);
    return true;
    
}

vector<vector<string>> executeSQLQuery(sqlite3* db, const string& sql, const vector<string>& params) {
    sqlite3_stmt* stmt;
    vector<vector<string>> rows;


    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error: Preparing SQL statement" << sqlite3_errmsg(db) << endl;
        return rows;
    }


    for (int i = 0; i < params.size(); i++) {
        sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_TRANSIENT);
    }

    int colCount = sqlite3_column_count(stmt);

    while (true) {
        int rc = sqlite3_step(stmt);

        if (rc == SQLITE_ROW) {
            vector<string> row;
            row.reserve(colCount);

            for (int col = 0; col < colCount; col++) {
                const unsigned char* text = sqlite3_column_text(stmt, col);
                row.push_back(text ? reinterpret_cast<const char*>(text) : "");
            }
            rows.push_back(row);
        }
        else if (rc == SQLITE_DONE) {
            break;
        }
        else {
            cerr << "Error executing query" << sqlite3_errmsg(db) << endl;
            break;
        }
    }
    sqlite3_finalize(stmt);
    return rows;

}



/*
function to check whether a given ID exists in a table
*/
bool exists(sqlite3* db, string& table, int id) {

    string sql = "SELECT COUNT(*) FROM " + table + " WHERE " + table + "_id = ?;";

    sqlite3_stmt* stmt;

    if (sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        cerr << "Error: preparing database statement" << sqlite3_errmsg(db) << endl;
        return false;
    }

    sqlite3_bind_int(stmt, 1, id);

    int exists = 0;

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        exists = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    return exists > 0;



}
