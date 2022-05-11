

PROGRAMS := bin/WordleSolver bin/Simulation

CXX = g++
CXXFLAGS = -g -pthread -m64 -Wall

INCLUDES = -I WordleSolver -I WordleLib -I Simulation
#VPATH = WorldelSolver WordleLib Simulation



all: bin lib $(PROGRAMS)

bin:
	mkdir -p bin
	cp "WordleSolver/Wordle solutions.txt" bin/ -u
	cp "WordleSolver/Wordle universe.txt" bin/ -u

lib:
	mkdir -p lib

bin/WordleSolver: WordleSolver/WordleSolver.o lib/libWordle.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -Llib -lWordle

bin/Simulation: Simulation/Simulation.o lib/libWordle.a
	$(CXX) $(CXXFLAGS) -o $@ $^ -Llib -lWordle

lib/libWordle.a: WordleLib/FileLogger.o WordleLib/Game.o WordleLib/Guess.o WordleLib/Hint.o WordleLib/Logger.o WordleLib/pch.o WordleLib/Round.o
	$(AR) $(ARFLAGS) $@ $?


	


	

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(PROGRAMS)
	cd WordleSolver; rm -f *.o
	cd ../Simulation; rm -f *.o
	cd ../WordleLib; rm -f *.o

.PHONY: bin lib all clean


