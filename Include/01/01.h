
#pragma once

#include "PuzzleFramework.h"

#include <fstream>
#include <numeric>

struct Day1 : public AoC::PuzzleBase<Day1>
{
	struct Data
	{
		std::vector<int> Depths;
	};

	using Data_t = Data;
	using Result_t = int64_t;

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/01/input.txt");

		int temp;
		while(file.good())
		{
			file >> temp;
			data.Depths.push_back(temp);
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 1: Sonar Sweep";
	}

	static Result_t Part1(const Data_t& data)
	{
		const size_t numDepths = data.Depths.size();
		int32_t increasedCount = 0;

		for(size_t i = 1; i < numDepths; ++i)
		{
			if(data.Depths[i] > data.Depths[i - 1])
			{
				++increasedCount;
			}
		}

		return increasedCount;
	}

	static Result_t Part2(const Data_t& data)
	{
		using namespace std;

		constexpr size_t windowSize = 3;
		const size_t numDepths = data.Depths.size();
		int32_t increasedCount = 0;

		auto GetWindowSum = [](const auto& container, size_t window, size_t start)
		{
			int32_t sum = 0;
			const size_t end = start + window;
			for(size_t i = start; i < end; ++i)
			{
				sum += container[i];
			}
			return sum;
		};

		int32_t lastSum = GetWindowSum(data.Depths, windowSize, 0);

		for(size_t i = 1; i <= numDepths - windowSize; ++i)
		{
			const int32_t compareSum = lastSum;
			lastSum -= data.Depths[i - 1];
			lastSum += data.Depths[i + (windowSize - 1)];

			if(lastSum > compareSum)
			{
				++increasedCount;
			}
		}

		return increasedCount;
	}
};