// WordleSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <numeric>

#include "../WorldleLib/Game.h"
#include <algorithm>

using std::cin;

Game game = Game();

std::ostream& operator<< (std::ostream& os, vector<std::pair<string, std::pair<int, string>>> v)
{
	for (auto line : v)
	{
		os << line.first << ": " << line.second.first << " solutions, best guess = " << line.second.second << endl;
	}

	return os;
}

std::ostream& operator<<(std::ostream& os, const vector<string>& v)
{
	std::cout << "[";
	std::for_each(v.begin(), v.end(), [](const string& item)
		{
			std::cout << item << "," << std::endl;
		});
	std::cout << "]" << std::endl;

	return os;
}

void Calculate()
{
	vector<string> _hints;
	// maps hints to the number of possible solutions and the guess you should use next
	map<string, std::pair<int, string>> _hint_map;

	for (char a : Game::VALIDHINTS)
	{
		string hint(5, a);
		for (char b : Game::VALIDHINTS)
		{
			hint[1] = b;
			for (char c : Game::VALIDHINTS)
			{
				hint[2] = c;
				for (char d : Game::VALIDHINTS)
				{
					hint[3] = d;
					for (char e : Game::VALIDHINTS)
					{
						hint[4] = e;
						_hints.push_back(hint);
					}
				}
			}
		}
	}


	vector<int> solution_indices(Game::get_pSolutions()->size());
	std::iota(solution_indices.begin(), solution_indices.end(), 0);
	Round round(solution_indices);

	for (string const& _hint : _hints)
	{
		vector<int> _new_solutions = round.apply_hint("soare", _hint);
		int _number_of_solutions = static_cast<int>(_new_solutions.size());
		if (_number_of_solutions > 0)
		{
			string _guess;
			if (_number_of_solutions == 1)
			{
				_guess = game.get_solution( _new_solutions[0] );
			}
			else
			{
				Round _round2(_new_solutions);
				_guess = _round2.find_best_guess()->get_guess();
			}

			cout << "Best guess for hint " + _hint << " is " << _guess << std::endl;
			_hint_map[_hint] = std::pair<int, string>(_number_of_solutions, _guess);
		}
	}

	vector <std::pair<string, std::pair<int, string>>> _hint_vector;
	for (auto item : _hint_map)
	{
		_hint_vector.push_back(item);
	}

	std::sort(_hint_vector.begin(), _hint_vector.end(), [](std::pair<string, std::pair<int, string>> const& a, std::pair<string, std::pair<int, string>> const& b)
		{
			return a.second.first > b.second.first;
		});

	std::ofstream _hint_file;
	_hint_file.open("Hints.txt");
	_hint_file << _hint_vector;
	_hint_file.close();


}

void Play()
{
	int _number_of_solutions = 0;
	while (_number_of_solutions != 1)
	{
		string _guess;
		while ( _guess == "" )
		{
			cout << "Enter guess, B to calculate best guess, or S to print possible solutions ";
			cin >> _guess;
			if (_guess == "Q")
			{
				exit( 0 );
			}
			if (_guess == "S")
			{
				_guess = "";
				cout << "Possible solutions:" << std::endl << *game.getPossibleSolutions();
				continue;
			}
			if ( _guess == "B" )
			{
				auto _g = game.find_best_guess();
				_guess = _g->get_guess();
				cout << "Guessing " << _guess << " expectation = " << _g->get_score() << " guesses" << std::endl;
				continue;
			}
			if ( !Game::validate_guess( _guess ) )
			{
				_guess = "";
			}
			else
			{
				cout << "Guessing " << _guess << " entropy = " << game.evaluate_guess(_guess) << std::endl;
			}
		}

		string _hint;
		while (_hint == "")
		{
			cout << "Enter hint ";
			cin >> _hint;
			if (_hint == "Q")
			{
				exit(0);
			}
			if ( !Game::validate_hint( _hint ) )
			{
				_hint = "";
			}
		}

		_number_of_solutions = game.play(_guess, _hint);
		cout << "Number of solutions = " << _number_of_solutions << std::endl;
	}

	cout << "Solution = " << game.get_solution() << std::endl;


}

int main()
{
	string line;
	game.set_solutions();
	game.set_universe();

	cout << "Enter previous solution: ";
	cin >> line;
	game.init(line);

	game.set_debug_mode(true);
	Play();

	cout << "Press any key to quit ";
	cin >> line;

}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
