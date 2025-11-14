# This script converts a CSV file into SQL INSERT statements.
# Just run this file to generate the sql file! (python interpreter required obviously)
import csv

# Replace with another csv here if needed
csv_path = "plays.csv"

# This is the output sql file that will contain all the INSERT statements
output_sql = "seed_play.sql"

with open(csv_path, newline='', encoding="utf-8") as csvfile:
    reader = csv.DictReader(csvfile)
    with open(output_sql, "w", encoding="utf-8") as out:
        out.write(" -- SQL INSERT statements for the 'play' table\n")
        out.write("-- Auto Generated from play_inserts.py script\n")
        out.write("BEGIN;\n")

        for row in reader:
            title = row["play_title"].replace("'", "''")
            author = row["play_author"].replace("'", "''")
            genre = row["play_genre"].replace("'", "''")
            acts = row["play_num_acts"]
            out.write(
                f"INSERT INTO play (play_title, play_author, play_genre, play_num_acts) "
                f"VALUES ('{title}', '{author}', '{genre}', {acts});\n"
            )

        out.write("COMMIT;\n")
        out.write("-- Example query:\n-- SELECT * FROM play LIMIT 10;\n")

print(f"SQL file '{output_sql}' created from '{csv_path}'.")
