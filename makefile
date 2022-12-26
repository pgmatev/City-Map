OBJ = main.o city.o program.o
CC = g++
CFLAGS = -g -Wall -pedantic

main: $(OBJ)
	$(CC) $(CFLAGS) -o main $(OBJ)

main.o: main.cpp
	$(CC) -c ./main.cpp
program.o: program.cpp program.hh
	$(CC) -c ./program.cpp
city.o: city.cpp city.hh
	$(CC) -c ./city.cpp