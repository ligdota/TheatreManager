all: db

main.o:
	g++ -c main.cpp -o main.o

sqlite3.o:
	gcc -c sqlite3.c -o sqlite3.o

db: main.o sqlite3.o
	g++ main.o sqlite3.o -o db

clean:
	rm -f *.o 


reset:
	rm -f theatre.db
	sqlite3 theatre.db < schema.sql
	sqlite3 theatre.db < seed.sql
