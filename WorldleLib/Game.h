#pragma once

#include <memory.h>
#include <vector>

#include "FileLogger.h"
#include "Guess.h"
#include "Logger.h"
#include "Round.h"

using std::vector;

class Game
{
private:
	static std::shared_ptr<Logger> pLogger_;
	static std::shared_ptr<vector<string>> pSolutions_;
	static std::shared_ptr<vector<string>> pUniverse_;
	static size_t word_length_;
 
	static vector<string> ReadWordFile( string const& fileName );

	vector<Round> rounds_ = vector<Round>();
	string solution_ = "";
	int number_of_solutions_;
	bool initialized_ = false;

public:
	static string VALIDHINTS;

	Game() = default;

	static void info( string const& msg ) { pLogger_->info( msg ); }
	static void debug( string const& msg ) { pLogger_->debug( msg ); }
	static void error( string const& msg, exception const& e ) { pLogger_->error( msg, e ); }
	static void set_debug_mode(bool value) { pLogger_->set_debug_mode(value); }
	static void set_logger(std::shared_ptr<Logger> const& logger) { pLogger_ = logger; };

	static void set_solutions( vector<string> const& solutions = ReadWordFile( "Wordle solutions.txt" ) );
	static std::shared_ptr<vector<string>> get_pSolutions() { return pSolutions_; }
	
	static void set_universe( vector<string> const& universe = ReadWordFile( "Wordle universe.txt" ) );
	static std::shared_ptr<vector<string>> get_pUniverse() { return pUniverse_; }

	static size_t get_word_length() { return word_length_; }

	static bool validate_guess( string const& guess );
	static bool validate_hint( string const& hint );
	static string get_solution(int n) { return pSolutions_->at(n); }

	// Prepare to play
	// Ensures solution and universe vectors are set
	// Initializes first round
	// Returns true if ready to play
	bool init();

	// Enter a guess and hint for current round
	// Returns number of remaining solutions
	// If >= 1, pushes a new Round to rounds_ with pruned vector of solutions
	// If == 1, solution_ contains the solution
	// If <= 0, an error was encountered and no new Tound was pushed
	int play( string const& guess, string const& hint );

	// Finds the best guess for current round
	// Returns the best guess ( or nullptr on an error )
	std::shared_ptr<Guess> find_best_guess() const;

	int get_number_of_solutions() const { return number_of_solutions_;  }
	string get_solution() const { return solution_; }
	
};

