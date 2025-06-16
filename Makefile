# noashalom5@gmail.com

CC = clang++
CFLAGS = -Wall -Wextra -std=c++11
# Source files
OFILES = main.o game.o player.o baron.o general.o governor.o judge.o spy.o
# Headers
HEADERS = baron.hpp general.hpp governor.hpp judge.hpp spy.hpp merchant.hpp
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

Main: main run

main: $(OFILES)
	$(CC) $(CFLAGS) -o main $(OFILES) $(LDFLAGS)

main.o: main.cpp game.hpp player.hpp $(HEADERS)
	$(CC) $(CFLAGS) -c main.cpp

game.o: game.cpp game.hpp player.hpp
	$(CC) $(CFLAGS) -c game.cpp

player.o: player.cpp player.hpp $(HEADERS)
	$(CC) $(CFLAGS) -c player.cpp

baron.o: baron.cpp baron.hpp player.hpp game.hpp
	$(CC) $(CFLAGS) -c baron.cpp

general.o: general.cpp general.hpp player.hpp game.hpp
	$(CC) $(CFLAGS) -c general.cpp

governor.o: governor.cpp governor.hpp player.hpp game.hpp
	$(CC) $(CFLAGS) -c governor.cpp

judge.o: judge.cpp judge.hpp player.hpp game.hpp
	$(CC) $(CFLAGS) -c judge.cpp

spy.o: spy.cpp spy.hpp player.hpp game.hpp
	$(CC) $(CFLAGS) -c spy.cpp

# ------------------- Tests -------------------

test_player: test_player.o game.o player.o baron.o general.o governor.o judge.o spy.o
	$(CC) $(CFLAGS) -o test_player $^

test_game: test_game.o game.o player.o baron.o general.o governor.o judge.o spy.o
	$(CC) $(CFLAGS) -o test_game $^

test_player.o: test_player.cpp game.hpp player.hpp doctest.hpp $(HEADERS)
	$(CC) $(CFLAGS) -c test_player.cpp

test_game.o: test_game.cpp game.hpp player.hpp doctest.hpp $(HEADERS)
	$(CC) $(CFLAGS) -c test_game.cpp

# ------------------- Run targets -------------------

run: main
	./main

test: test_player test_game
	./test_player
	./test_game

valgrind: main test_player test_game
	valgrind --leak-check=full --show-leak-kinds=all ./main
	valgrind --leak-check=full --show-leak-kinds=all ./test_player
	valgrind --leak-check=full --show-leak-kinds=all ./test_game

clean:
	rm -f *.o main test_player test_game

.PHONY: Main run test valgrind clean
