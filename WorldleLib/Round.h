#pragma once

#include <memory>
#include <string>
#include <vector>

using std::string;
using std::vector;

class Round
{
private:
	std::shared_ptr<vector<int>> pSolution_indices_;
	string guess_;
	string hint_;

public:
	Round(vector<int>& solution_indices) : pSolution_indices_(std::make_shared<vector<int>>(solution_indices)) {}

	vector<int> apply_hint( string const& guess, string const& hint );
	std::shared_ptr<Guess> find_best_guess() const;
	std::shared_ptr<std::vector<string>> getPossibleSolutions() const;
	double evaluate_guess(string const word) const;
};

