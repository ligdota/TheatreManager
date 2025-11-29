import sqlite3
import json
import sys


# We can use this function to get balance sheet information for one production.
# We can also use a query and for loop to get balance sheets for all productions if needed.
def get_production_balance_sheet(production_id: int) -> dict:
    """
    **Stored-procedure:**
    Generates a full balance sheet breakdown for a given production.
    """
    
    conn = sqlite3.connect("theatre.db")
    cursor = conn.cursor()
    cursor.execute("""
        SELECT 
            tt.transaction_type_name,
            f.transaction_amount
        FROM finances f
        JOIN transaction_type tt
            ON f.transaction_type_id = tt.transaction_type_id
        WHERE f.production_id = ?
            AND tt.transaction_type_name <> 'member_dues';
    """, (production_id,))

    rows = cursor.fetchall()
    conn.close()

    # Classify all categories
    categories = {
        "ticket_sales": 0,
        "sponsor_ad_creds": 0,
        "sponsor_prod_creds": 0,
        "expenses": 0
    }

    # Fill in the amounts from the above query.
    for type, amount in rows:
        if type in categories:
            categories[type] += amount
        else:

            categories[type] = amount

    

    # Group and add all sources of income into total income excluding advertisement and production creds.
    ticket_sales = categories["ticket_sales"]
    total_income = ticket_sales + categories["sponsor_ad_creds"] + categories["sponsor_prod_creds"]
    # Calculates the net balance.

    # Returns the results in a dictionary format.
    return {
        **categories,
        "ticket_sales": ticket_sales,
        "sponsor_ad_creds": categories["sponsor_ad_creds"],
        "sponsor_prod_creds": categories["sponsor_prod_creds"],
        "total_income": total_income,
        "total_expenses": categories["expenses"],
    }




if __name__ == "__main__":
    prod_id = int(sys.argv[1])
    result = get_production_balance_sheet(prod_id)
    print(json.dumps(result, indent=4))