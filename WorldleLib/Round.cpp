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

double Round::evaluate_guess(string const word) const
{
	Guess g = Guess(word, pSolution_indices_);
	return g.get_entropy();
}

std::shared_ptr<Guess> Round::find_best_guess() const
{
	std::shared_ptr<Guess> _best_guess = nullptr;
	for (string& word : *Game::get_pUniverse())
	{
		double n;
		Guess g = Guess(word, pSolution_indices_);
		if (_best_guess == nullptr)
		{
			_best_guess = std::make_shared<Guess>(g);
			n = _best_guess->get_score();
			Game::debug("Best guess  = " + word + " entropy = " + std::to_string(g.get_entropy()) + " score = " + std::to_string(g.get_score()));
		}
		else if ((_best_guess->get_score() > g.get_score()) || ((!_best_guess->get_is_possible()) && g.get_is_possible() && _best_guess->get_score() == g.get_score()))
		{
			_best_guess = std::make_shared<Guess>(g);
			Game::debug("Best guess  = " + word + " entropy = " + std::to_string(g.get_entropy()) + " score = " + std::to_string(g.get_score()));
		}
	}

	return _best_guess;
};
std::shared_ptr<std::vector<string>> Round::getPossibleSolutions() const
{
	std::vector<string> _solutions(pSolution_indices_->size());
	auto _pSolutions = std::make_shared< std::vector<string>>(_solutions);
	std::transform(pSolution_indices_->begin(), pSolution_indices_->end(), _pSolutions->begin(), [](int n)
		{
			return Game::get_pSolutions()->at(n);
		});
	return _pSolutions;
}
