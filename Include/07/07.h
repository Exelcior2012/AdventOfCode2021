
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <vector>

struct Day7 : public AoC::PuzzleBase<Day7>
{
	struct Data
	{
		std::vector<int> Positions;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			int temp;
			char delim;
			while(in.good())
			{
				in >> temp >> delim;
				out.Positions.push_back(temp);
			}

			return in;
		}
	};

	using Data_t = Data;
	using Result_t = int64_t;

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/07/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 7: The Treachery of Whales";
	}

	static Result_t Part1(const Data_t& data)
	{
		auto dataCopy = data.Positions;

		std::sort(std::begin(dataCopy), std::end(dataCopy));
		int bestPosition = dataCopy[dataCopy.size() / 2];

		int fuelSum = 0;
		for(int p : dataCopy)
		{
			fuelSum += Util::Abs(bestPosition - p);
		}

		return fuelSum;
	}

	static Result_t Part2(const Data_t& data)
	{
		int bestPosition = std::accumulate(std::begin(data.Positions), std::end(data.Positions), 0) / data.Positions.size();

		int fuelSum = 0;
		for(int p : data.Positions)
		{
			fuelSum += Util::TriangularNumber(Util::Abs(bestPosition - p));
		}

		return fuelSum;
	}
};
