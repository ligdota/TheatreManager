rows = [chr(i) for i in range(ord('A'), ord('Z') + 1)]
seats_per_row = 50


with open("seed.sql", "w") as f:
    for row in rows:
        for num in range(1, seats_per_row + 1):
            f.write(f"INSERT INTO seat (seat_row, seat_num) VALUES ('{row}', {num});\n")

        