
#pragma once

#include "PuzzleFramework.h"

#include <fstream>
#include <array>

struct Day6 : public AoC::PuzzleBase<Day6>
{
	struct Data
	{
		using FishHistogram_t = std::array<uint64_t, 9>;
		FishHistogram_t Fish = {};
		
		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			int temp;
			char delim;
			while(in.good())
			{
				in >> temp >> delim;
				++(out.Fish[temp]);
			}

			return in;
		}
	};

	using Data_t = Data;
	using Result_t = int64_t;

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/06/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 6: Lanternfish";
	}

	static void Tick(Data::FishHistogram_t& fish, int n = 1)
	{
		for(int i = 0; i < n; ++i)
		{
			uint64_t temp = fish[0];

			std::memmove(&fish[0], &fish[1], sizeof(uint64_t) * 8);

			fish[6] += temp;
			fish[8] = temp;
		}
	}

	static Result_t Part1(const Data_t& data)
	{
		Data::FishHistogram_t fish = data.Fish;
		Tick(fish, 80);

		return std::accumulate(std::begin(fish), std::end(fish), 0ull);
	}

	static Result_t Part2(const Data_t& data)
	{
		Data::FishHistogram_t fish = data.Fish;
		Tick(fish, 256);

		return std::accumulate(std::begin(fish), std::end(fish), 0ull);
	}
};
