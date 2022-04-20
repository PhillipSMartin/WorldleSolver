// Simulation.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <numeric>

#include "../WorldleLib/Game.h"
#include <algorithm>

using std::cin;

Game game = Game();

std::ostream& operator<< (std::ostream& os, map<int, int> m)
{
	for (auto const& item : m)
	{
		os << item.first << " guesses: " << item.second << " games" << endl;
	}

	return os;
}


int Solve(string solution, string firstGuess)
{
	int _number_of_guesses = 1;
	int _number_of_solutions = 999;
	string _guess = firstGuess;
	string _hint;
	game.init("");
	while (_number_of_solutions > 1)
	{
		_hint = Guess::ComputeHint(_guess, solution);
		_number_of_solutions = game.play(_guess, _hint);
		if (_number_of_solutions == 1)
		{
			if (game.get_solution() == _guess)
			{
				break;
			}
		}
		else
		{
			auto _g = game.find_best_guess();
			_guess = _g->get_guess();
			cout << "Guessing " << _guess << " entropy = " << game.evaluate_guess(_guess) << ", expectation = " << _g->get_score() << " guesses" << std::endl;
		}
		_number_of_guesses++;
	}

cout << "Solved " << solution << " in " << _number_of_guesses << " guesses" << std::endl;
	return _number_of_guesses;
}

void Play(string firstGuess)
{
	int _number_of_guesses = 0;
	int _number_of_games = 0;
	auto _record = map<int, int>();

	for (string solution : *game.get_pSolutions())
	{
		_number_of_games++;
		int n = Solve(solution, firstGuess);
		_number_of_guesses += n;
		if (_record.find(n) != _record.end())
		{
			_record[n] += 1;
		}
		else
		{
			_record[n] = 1;
		}
	}

	cout << "Guess " << firstGuess << ": average guesses = " << static_cast<double>(_number_of_guesses) / _number_of_games << std::endl;
	cout << _record;
}

int main()
{
	string line;
	game.set_solutions();
	game.set_universe();

	Play("soare");



}
