import sqlite3

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
        SELECT transaction_type_name, total_amount
        FROM view_all_transaction_types
        WHERE production_id = ?;
    """, (production_id,))

    rows = cursor.fetchall()
    conn.close()

    # Classify all categories
    categories = {
        "ticket_sales": 0,
        "sponsor_prod_creds": 0,
        "sponsor_ad_creds": 0,
        "member_dues": 0,
        "expenses": 0
    }

    # Fill in the amounts from the above query.
    for type, amount in rows:
        categories[type] = amount

    # Group and add all sources of income into total income.
    total_income = (categories["ticket_sales"] +
        categories["sponsor_prod_creds"] +
        categories["sponsor_ad_creds"] +
        categories["member_dues"]
    )

    # Calculates the net balance.
    net_balance = total_income - categories["expenses"]

    # Returns the results in a dictionary format.
    return {
        **categories,
        "total_income": total_income,
        "total_expenses": categories["expenses"],
        "net_balance": net_balance
    }
