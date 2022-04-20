#include "pch.h"

#include <algorithm>
#include <fstream>
#include <numeric>
#include <random>

#include "Game.h"

std::shared_ptr<Logger> Game::pLogger_ = std::make_shared<Logger>();
std::shared_ptr<vector<string>> Game::pSolutions_ = std::make_shared<vector<string>>();
std::shared_ptr<vector<string>> Game::pUniverse_ = std::make_shared<vector<string>>();
size_t Game::word_length_ = 0;
string Game::VALIDHINTS = "BYG";

vector<string> Game::ReadWordFile( string const& fileName )
{
	std::ifstream _file;
	vector<string> _word_list = vector<string>();
	try
	{
		_file.open( fileName );

		if ( !_file.is_open() )
		{
			info( "Unable to open file " + fileName );
		}
		else
		{
			string word;
			while ( _file >> word )
			{
				_word_list.push_back( word );
			}

			debug( "Read " + std::to_string( _word_list.size() ) + " words from " + fileName );
		}

	}
	catch ( std::exception& e )
	{
		_word_list.clear();
		error( "Error reading words from file " + fileName, e );
	}
	_file.close();

	return _word_list;
}

void Game::set_solutions( vector<string> const& solutions )
{
	pSolutions_->insert( pSolutions_->end(), solutions.begin(), solutions.end() );
	pUniverse_->insert( pUniverse_->end(), solutions.begin(), solutions.end() );
	word_length_ = get_solution( 0 ).length();
}

void Game::set_universe( vector<string> const& universe )
{
	pUniverse_->insert( pUniverse_->end(), universe.begin(), universe.end() );
}

bool Game::validate_guess( string const& guess )
{
	return find( pUniverse_->begin(), pUniverse_->end(), guess ) != pUniverse_->end();
}
bool Game::validate_hint( string const& hint )
{
	return ( hint.length() == word_length_ ) && all_of( hint.begin(), hint.end(), []( char const c ) {
		return VALIDHINTS.find( c ) != string::npos;
		} );
}

// Prepare to play
// Ensures solution and universe vectors are set
// Initializes first round
bool Game::init(string const last_solution)
{
	if ( pSolutions_->size() <= 0 )
	{
		info( "Must load possible solutions before playing" );
		return false;
	}
	if ( pUniverse_->size() <= 0 )
	{
		info( "Must load possible guesses before playing" );
		return false;
	}

	number_of_solutions_ = static_cast<int>(pSolutions_->size());
	rounds_.clear();

	int starting_index = 0;
	if (last_solution != "")
	{
		auto it = std::find(pSolutions_->begin(), pSolutions_->end(), last_solution);
		if (it != pSolutions_->end())
		{
			starting_index =static_cast<int>(it - pSolutions_->begin()) + 1;
		}
	}
	vector<int> _solution_indices(pSolutions_->size() - starting_index);
	std::iota( _solution_indices.begin(), _solution_indices.end(), starting_index);
	std::random_device _random_device;
	std::mt19937 _generator(_random_device());
	std::shuffle(_solution_indices.begin(), _solution_indices.end(), _generator);

	rounds_.push_back( Round( _solution_indices ) );

	initialized_ = true;
	return true;
}

// Enter a guess and hint for current round
// Returns number of remaining solutions
// If >= 1, pushes a new Round to rounds_ with pruned vector of solutions
// If == 1, solution_ contains the solution
// If <= 0, an error was encountered and no new Round was pushed
int Game::play( const string& guess, const string& hint )
{
	if ( !validate_guess( guess ) )
	{
		info( "Invalid guess " + guess );
		return 0;
	}
	if ( !validate_hint( hint ) )
	{
		info( "Invalid hint " + hint );
		return 0;
	}

	vector<int> new_solutions = rounds_.back().apply_hint( guess, hint );
	int _number_of_solutions = static_cast<int>( new_solutions.size() );
	if (_number_of_solutions > 0 )
	{
		rounds_.push_back( Round( new_solutions ) );
		number_of_solutions_ = _number_of_solutions;
		if (_number_of_solutions == 1)
		{
			solution_ = get_solution(new_solutions[0]);
		}
	}
	else
	{
		info( "Hint " + hint + " is incompatible with current state" );
	}

	return _number_of_solutions;
}

// Finds the best guess for current round
// Returns the best guess ( or nulptr on an error )
std::shared_ptr<Guess> Game::find_best_guess() const
{
	return rounds_.back().find_best_guess();
}

double Game::evaluate_guess(string const word) const
{
	return rounds_.back().evaluate_guess(word);
}


