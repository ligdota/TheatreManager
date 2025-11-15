import sqlite3
from typing import Any


def ticket_sales_report() -> list[Any] | None:
    """Generates a report of ticket sales.."""
    conn = sqlite3.connect('theatre.db')
    cursor = conn.cursor()
    cursor.execute("""
        SELECT play_title, buyer_name, buyer_email, seat, ticket_cost, purchase_date
        FROM view_ticket_sales
        ORDER BY play_title;
    """)
    rows = cursor.fetchall()
    conn.close()
    return rows # Returns one or multiple rows of list data type or None if no data

def production_cast_crew_report() -> list[Any] | None:
    """Generates a report of production cast and crew."""
    conn = sqlite3.connect('theatre.db')
    cursor = conn.cursor()
    cursor.execute("""
        SELECT production_id, play_title, member_name, role_name
        FROM view_production_cast_crew
        ORDER BY production_id, role_name, member_name;
    """)
    rows = cursor.fetchall()
    conn.close()
    return rows

def member_dues_report() -> list[Any] | None:
    """Generates a report of member dues status."""
    conn = sqlite3.connect('theatre.db')
    cursor = conn.cursor()
    cursor.execute("""
        SELECT member_name, member_email, member_phone, dues_status
        FROM view_member_dues
        ORDER BY dues_status, member_name;
    """)
    rows = cursor.fetchall()
    conn.close()
    return rows

def sponsor_contributions_report() -> list[Any] | None:
    """Generates a report of sponsor contributions for each production."""
    conn = sqlite3.connect('theatre.db')
    cursor = conn.cursor()
    cursor.execute("""
        SELECT sponsor_name, sponsor_email, play_title,
               donation_amount, sponsor_ad_creds, sponsor_prod_creds
        FROM view_sponsor_contributions
        ORDER BY play_title, sponsor_name;
    """)
    rows = cursor.fetchall()
    conn.close()
    return rows

def financial_summary_report() -> list[Any] | None:
    """Generates a balance sheet style financial summary for each production."""
    conn = sqlite3.connect('theatre.db')
    cursor = conn.cursor()
    cursor.execute("""
        SELECT production_id, play_title, transaction_type_name, total_amount
        FROM view_production_finances
        ORDER BY production_id, transaction_type_name;
    """)
    rows = cursor.fetchall()
    conn.close()
    return rows

def get_production_total_revenue(production_id: int) -> float:
    """
    Stored-procedure-equivalent function.
    Calculates total revenue for a production from ticket sales + sponsor donations.
    """

    conn = sqlite3.connect("theatre.db")
    cursor = conn.cursor()

    # Ticket revenue
    cursor.execute("""
        SELECT COALESCE(SUM(ticket_cost), 0)
        FROM ticket
        WHERE production_id = ?;
    """, (production_id,))
    ticket_total = cursor.fetchone()[0]

    # Sponsor donations
    cursor.execute("""
        SELECT COALESCE(SUM(donation_amount), 0)
        FROM sponsor_donations
        WHERE production_id = ?;
    """, (production_id,))
    sponsor_total = cursor.fetchone()[0]

    conn.close()

    return ticket_total + sponsor_total
