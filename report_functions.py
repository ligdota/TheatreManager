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
