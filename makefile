CXX = g++
CC = gcc
CXXFLAGS = -g -O0 -Wall

OBJ = main.o play.o member.o production.o execute.o sqlite3.o
TARGET = db.exe


all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $(TARGET)

main.o: main.cpp
	$(CXX) $(CXXFLAGS) -c main.cpp -o main.o

play.o: play.cpp
	$(CXX) $(CXXFLAGS) -c play.cpp -o play.o

member.o: member.cpp
	$(CXX) $(CXXFLAGS) -c member.cpp -o member.o

production.o: production.cpp
	$(CXX) $(CXXFLAGS) -c production.cpp -o production.o

execute.o: execute.cpp
	$(CXX) $(CXXFLAGS) -c execute.cpp -o execute.o

sqlite3.o: sqlite3.c
	$(CC) -c sqlite3.c -o sqlite3.o




clean:
	rm -f *.o $(TARGET)


reset:
	rm -f theatre.db
	sqlite3 theatre.db < schema.sql
	sqlite3 theatre.db < seed.sql
