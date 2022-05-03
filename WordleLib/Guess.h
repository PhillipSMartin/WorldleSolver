#pragma once

#include <map>
#include <memory>
#include <string>

using std::map;
using std::string;
using std::vector;

class Guess
{
private:
	string word_;
	double entropy_;
	double score_;
	double number_of_solutions_;
	bool is_possible_ = false;
	map<string, int> _hint_counts = map<string, int>();

	double ComputeEntropy() const;
	double ComputeScore() const;

public:
	// take a guess and possible solutions and calculates an entropy and score
	Guess(string const& guess, std::shared_ptr<vector<int>> const& pSolution_indices);

	double get_entropy() const { return entropy_; }
	double get_score() const { return score_; }
	string get_guess() const { return word_; }
	bool get_is_possible() const { return is_possible_; }

	// for a given guess and solution, returns the hint 
	static string ComputeHint( string const& guess, string const& solution );
};

