
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <string>
#include <map>
#include <tuple>

// The possible rolls that can occur, and how many times that result can appear.
// There are 27 possible combinations of triple roll results, however only
// seven distinct results possible from those combinations. to correctly
// calculate the final number of branches we need to track how many branches
// would have been created for each result.
static const int Rolls3_reduce[] = {3, 4, 5, 6, 7, 8, 9};
static const int Rolls3_occurs[] = {1, 3, 6, 7, 6, 3, 1};

struct Day21 : public AoC::PuzzleBase<Day21>
{
	struct Data
	{
		int StartLocations[2] = {0, 0};

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;
			string discard;
			int value;

			getline(in, line);
			stringstream ss(line);
			ss >> discard >> value >> discard >> discard >> value;
			out.StartLocations[0] = value;

			getline(in, line);
			ss.clear();
			ss.str(line);
			ss >> discard >> value >> discard >> discard >> value;
			out.StartLocations[1] = value;

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/21/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 21: Dirac Dice";
	}

	static int64_t Part1(const Data& data)
	{
		int roll = 1;
		int rolls = 0;
		int loser = -1;
		int scores[2] = {0, 0};
		int spaces[2] = {data.StartLocations[0], data.StartLocations[1]};

		// Rolls a d100
		auto DoRoll = [&roll, &rolls]()
		{
			++rolls;
			const auto thisRoll = roll;
			roll = Util::WrapRange(1, 100, roll + 1);
			return thisRoll;
		};

		// Keep playing the game until we reach a score over 1000
		while(true)
		{
			int p1 = DoRoll() + DoRoll() + DoRoll();
			spaces[0] = Util::WrapRange(1, 10, spaces[0] + p1);
			scores[0] += spaces[0];

			if(scores[0] >= 1000)
			{
				loser = 1;
				break;
			}

			int p2 = DoRoll() + DoRoll() + DoRoll();
			spaces[1] = Util::WrapRange(1, 10, spaces[1] + p2);
			scores[1] += spaces[1];

			if(scores[1] >= 1000)
			{
				loser = 0;
				break;
			}
		}

		return scores[loser] * rolls;
	}

	struct Result
	{
		int64_t Player1Wins;
		int64_t Player2Wins;

		void Swap()
		{
			std::swap(Player1Wins, Player2Wins);
		}
	};

	using Cache_t = std::map<std::tuple<int, int, int, int>, Result>;

	// Recursively calculates the quantum game variant results with memoization to reduce computation
	static Result QuantumPlay(int pos0, int pos1, int score0, int score1, Cache_t& memo)
	{
		std::tuple state{pos0, pos1, score0, score1};
		auto foundIt = memo.find(state);

		if(foundIt != memo.end())
		{
			return foundIt->second;
		}

		// Base cases, if either score is a winner, return a win for that side
		if(score0 >= 21)
		{
			return {1, 0};
		}

		if(score1 >= 21)
		{
			return {0, 1};
		}

		int64_t wins0 = 0;
		int64_t wins1 = 0;

		// Process each possible result, branching off all other games of this roll
		int count = 0;
		for(int roll : Rolls3_reduce)
		{
			int occurances = Rolls3_occurs[count++];
			int newPosition = Util::WrapRange(1, 10, pos0 + roll);
			int newScore = score0 + newPosition;

			// Note swapped params, other player is playing the next turn
			Result subResult = QuantumPlay(pos1, newPosition, score1, newScore, memo);
 
			// make sure to multiply by occurances of this result as we aren't 
			// considering every possible combination
			wins0 += subResult.Player2Wins * occurances;
			wins1 += subResult.Player1Wins * occurances;
		}

		// Cache our result
		memo[state] = {wins0, wins1};
		return {wins0, wins1};
	}
	
	static int64_t Part2(const Data& data)
	{
		Cache_t cache;
		Result gameResult = QuantumPlay(data.StartLocations[0], data.StartLocations[1], 0, 0, cache);
		return std::max(gameResult.Player1Wins, gameResult.Player2Wins);
	}
};

