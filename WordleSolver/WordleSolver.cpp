// WordleSolver.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include "../WorldleLib/Game.h"

using std::cin;

int main()
{
	Game game = Game();
	game.set_debug_mode( true );
	game.set_solutions();
	game.set_universe();
	game.init();

	int _number_of_solutions = 0;
	while (_number_of_solutions != 1)
	{
		string _guess;
		while ( _guess == "" )
		{
			cout << "Enter guess or B to calculate best guess ";
			cin >> _guess;
			if ( _guess == "B" )
			{
				auto _g = game.find_best_guess();
				_guess = _g->get_guess();
				cout << "Guessing " << _guess << " entropy = " << _g->get_entropy() << std::endl;
			}
			if ( !Game::validate_guess( _guess ) )
			{
				_guess = "";
			}
		}

		string _hint;
		while (_hint == "")
		{
			cout << "Enter hint ";
			cin >> _hint;
			if ( !Game::validate_hint( _hint ) )
			{
				_hint = "";
			}
		}

		_number_of_solutions = game.play(_guess, _hint);
		cout << "Number of solutions = " << _number_of_solutions << std::endl;
	}

	cout << "Solution = " << game.get_solution();
	string s;
	std::cin >> s;

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
