# noashalom5@gmail.com
CC = clang++
CFLAGS = -Wall -Wextra -std=c++11
OFILES = main.o SquareMat.o

Main: main run

main: $(OFILES)
	$(CC) $(CFLAGS) -o main $(OFILES)

main.o: main.cpp SquareMat.hpp
	$(CC) $(CFLAGS) -c main.cpp

SquareMat.o: SquareMat.cpp SquareMat.hpp
	$(CC) $(CFLAGS) -c SquareMat.cpp

run: main
	./main

test_exe: test.o SquareMat.o
	$(CC) $(CFLAGS) -o test test.o SquareMat.o

test: test_exe
	./test

test.o: test.cpp SquareMat.hpp doctest.hpp
	$(CC) $(CFLAGS) -c test.cpp

valgrind: main test_exe
	valgrind --leak-check=full --show-leak-kinds=all ./main
	valgrind --leak-check=full --show-leak-kinds=all ./test

clean:
	rm -f *.o main test

.PHONY: Main run clean valgrind