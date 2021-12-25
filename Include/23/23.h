
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <string>
#include <regex>

struct Day23 : public AoC::PuzzleBase<Day23>
{
	struct Data
	{
		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;

			while(in.good())
			{
				getline(in, line);
			}

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/23/testcase.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 23: Amphipod";
	}

	static int64_t Part1(const Data& data)
	{
		return 0;
	}

	static int64_t Part2(const Data& data)
	{
		return 0;
	}
};

