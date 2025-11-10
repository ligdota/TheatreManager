all: prog

main.o:
	g++ -c main.cpp -o main.o

sqlite3.o:
	gcc -c sqlite3.c -o sqlite3.o

prog: main.o sqlite3.o
	g++ main.o sqlite3.o -o prog

clean:
	rm -f *.o prog
