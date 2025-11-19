import csv
# This script converts a CSV file into a .sql file with SQL INSERT statements.
# Just run this script to generate the file! (python interpreter required obviously)

# -- !!! CHANGE THESE BEFORE RUNNING SCRIPT !!! --
TABLE_NAME = "sponsor_donations" # replace with other table name: like "member", "production", etc.
CSV_PATH   = "csv/sponsor_donations.csv" # name of the csv and path (exact name)
OUTPUT_SQL = "inserts/sponsor-donations_inserts.sql" # name of the output file you want created (exact name), i recommend this format: "tablename_inserts.sql"
# -- !!! CHANGE THESE BEFORE RUNNING SCRIPT !!! --

def escape_sql(value: str) -> str:
    """escape single quotes for use inside SQL string literals"""
    return value.replace("'", "''")


with open(CSV_PATH, newline="", encoding="utf-8") as csvfile:
    reader = csv.DictReader(csvfile)
    columns = reader.fieldnames  # column names taken from csv header

    if not columns:
        raise ValueError("CSV file has no header row (no column names).")

    col_list = ", ".join(columns)

    with open(OUTPUT_SQL, "w") as out:
        out.write(f"-- SQL INSERT statements for table `{TABLE_NAME}`\n")
        out.write(f"-- Auto Generated From CSV: {CSV_PATH}\n")
        out.write("BEGIN;\n")

        for row in reader:
            values_sql = []
            for col in columns:
                raw = (row[col] or "").strip()
                val = escape_sql(raw)
                values_sql.append(f"'{val}'")

            values_clause = ", ".join(values_sql)
            out.write(
                f"INSERT INTO {TABLE_NAME} ({col_list}) "
                f"VALUES ({values_clause});\n"
            )

        out.write("COMMIT;\n")
print(f"Done! Created '{OUTPUT_SQL}' into inserts folder from '{CSV_PATH}' for table '{TABLE_NAME}'.")
