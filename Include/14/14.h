
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <tuple>

struct Day14 : public AoC::PuzzleBase<Day14>
{

	struct Data
	{
		struct InsertionRule
		{
			// String is unfortunate here - but hashes easily for unordered_map
			// Fast enough for sub ms run time for the puzzle, but could be just
			// two chars with manual hash
			std::string Pair;
			char Insert;
		};

		std::string Template;
		std::vector<InsertionRule> Rules;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string temp;
			
			if(in.good())
			{
				in >> out.Template;
			}

			while(in.good())
			{
				InsertionRule rule;
				in >> rule.Pair >> temp >> rule.Insert;

				if(rule.Pair != "")
				{
					out.Rules.push_back(rule);
				}
			}

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/14/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 14: Extended Polymerization";
	}

	// A character to insert matched with a count to track state during iters
	struct InsertWithCount
	{
		char Insert;
		uint64_t Count;
	};

	static int64_t DoNSteps(const Data& data, int steps)
	{
		// Double buffer of state, as we need to fully process n to calculate
		// n + 1 we can't do this in place
		using map_t = std::unordered_map<std::string_view, InsertWithCount>;
		map_t pairs[2];
		size_t currentIdx = 0;
		auto Current = [&]() -> map_t& { return pairs[currentIdx]; };
		auto Next = [&]() -> map_t& { return pairs[(currentIdx + 1) % 2]; };
		auto Flip = [&]() { currentIdx = (currentIdx + 1) % 2; };

		// Build the buckets for all pairs
		for(const Data::InsertionRule& rule : data.Rules)
		{
			InsertWithCount value{rule.Insert, 0};
			Current()[rule.Pair] = value;
			Next()[rule.Pair] = value;
		}

		// Prime the state with the template string
		const size_t templateSize = data.Template.size();
		for(size_t i = 1; i < templateSize; ++i)
		{
			std::string_view s{&data.Template[i - 1], 2};
			Current()[s].Count++;
		}

		for(int i = 0; i < steps; ++i)
		{
			// Prepare next state
			Next() = Current();

			// For each pair, if there are any present we remove them and then
			// increment the pair of p[0]+I and I+p[1] in the next state
			for(const std::pair<std::string_view, InsertWithCount>& p : Current())
			{
				const std::string_view currentPair = std::get<0>(p);
				const InsertWithCount& currentInsert = std::get<1>(p);

				if(currentInsert.Count > 0)
				{
					std::string firstNewPair = std::string(currentPair.substr(0, 1)) + currentInsert.Insert;
					std::string secondNewPair = currentInsert.Insert + std::string(currentPair.substr(1, 1));

					Next()[currentPair].Count -= currentInsert.Count;
					Next()[firstNewPair].Count += currentInsert.Count;
					Next()[secondNewPair].Count += currentInsert.Count;
				}
			}

			// Swap buffers for next step
			Flip();
		}

		// Count up all the characters, we add all of the first characters, then
		// we add the last character again as it never changes. The first character
		// also never changes but that's caught by adding together all the first
		// pair letters
		std::array<uint64_t, 26> counts = {0};
		for(const std::pair<std::string_view, InsertWithCount>& p : Current())
		{
			const std::string_view currentPair = std::get<0>(p);
			const InsertWithCount& currentInsert = std::get<1>(p);

			if(currentInsert.Count == 0) continue;

			const size_t countIndex = currentPair[0] - 'A';
			uint64_t& count = counts[countIndex];
			count += currentInsert.Count;
		}
		counts[data.Template.back() - 'A']++;

		uint64_t least = std::numeric_limits<uint64_t>::max();
		uint64_t most = 0;
		for(uint64_t count : counts)
		{
			if(count == 0)
			{
				continue;
			}

			least = std::min(count, least);
			most = std::max(count, most);
		}

		return most - least;
	}

	static int64_t Part1(const Data& data)
	{
		return DoNSteps(data, 10);
	}

	static int64_t Part2(const Data& data)
	{
		return DoNSteps(data, 40);
	}
};
