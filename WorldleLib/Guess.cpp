#include "pch.h"

#include <assert.h>

#include "Game.h"
#include "Guess.h"

string Guess::ComputeHint( string const& guess, string const& solution )
{
assert( guess.length() == Game::get_word_length() );
assert( solution.length() == Game::get_word_length() );

string _hint( Game::get_word_length(), 'B' );
string _solution = solution;
int i = 0;

for ( auto _letter : guess )
{
	if ( _letter == _solution[i] )
	{
		_hint[i] = 'G';
		_solution[i] = ' ';
	}
	i++;
}

i = 0;
for ( auto _letter : guess )
{
	if ( _hint[i] == 'B' )
	{
		auto pos = _solution.find( _letter );
		if ( pos != std::string::npos )
		{
			_hint[i] = 'Y';
			_solution[pos] = ' ';
		}
	}
	i++;
}

return _hint;
}

Guess::Guess(string const& guess, std::shared_ptr<vector<int>> const& pSolution_indices) : word_(guess)
{
	number_of_solutions_ = static_cast<double>( pSolution_indices->size() );

	for (int index : *pSolution_indices)
	{
		_hint_counts[ComputeHint(guess, Game::get_pSolutions()->at(index))]++;
	}

	entropy_ = ComputeEntropy();
	score_ = ComputeScore();
}

double Guess::ComputeEntropy() const
{
	double _entropy = 0.0;
	if (number_of_solutions_ > 0)
	{
		for(auto item : _hint_counts)
		{
			double p = item.second / number_of_solutions_;
			_entropy += p * -log2(p);
		}
	}

	return _entropy;
}

double Guess::ComputeScore() const
{
	double bonusFactor = 1.0;
	if ((number_of_solutions_ > 0) && _hint_counts.count(string(Game::get_word_length(), 'G')))
	{
		return entropy_ * pow(2.0, 1.0 / number_of_solutions_);
	}

	return entropy_;
}

