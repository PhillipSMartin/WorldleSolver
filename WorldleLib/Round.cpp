#include "pch.h"

#include <algorithm>

#include "Game.h"
#include "Guess.h"
#include "Round.h"

vector<int> Round::apply_hint( string const& guess, string const& hint )
{
	guess_ = guess;
	hint_ = hint;

	vector<int> _new_solution_indices = vector<int>(pSolution_indices_->size());
	auto it = std::copy_if(pSolution_indices_->begin(), pSolution_indices_->end(), _new_solution_indices.begin(), [&guess, &hint](int index)
		{
			return Guess::ComputeHint(guess, Game::get_pSolutions()->at(index)) == hint;
		});
	_new_solution_indices.resize(std::distance(_new_solution_indices.begin(), it));
	return _new_solution_indices;
}

std::shared_ptr<Guess> Round::find_best_guess() const
{
	std::shared_ptr<Guess> _best_guess = nullptr;
	for (string& word : *Game::get_pUniverse())
	{
		Guess g = Guess(word, pSolution_indices_);
		double n = _best_guess ? _best_guess->get_score() : -1;
		if ( ( _best_guess ? _best_guess->get_score() : -1.0 ) < g.get_score() )
		{
			_best_guess = std::make_shared<Guess>(g);
			Game::debug( "Best guess  = " + word + " entropy = " + std::to_string( g.get_entropy() ) + " score = " + std::to_string( g.get_score() ) );
		}
	}

	return _best_guess;
}
