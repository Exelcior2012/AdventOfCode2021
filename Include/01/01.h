
#pragma once

#include <stdint.h>
#include <string_view>
#include <iostream>

#include "PuzzleFramework.h"

struct Day1 : public AoC::PuzzleBase<Day1>
{
	struct Data
	{
	};

	using Data_t = Data;
	using Result_t = int64_t;

	static Data_t Setup()
	{
		return Data_t();
	}

	static std::string_view GetName()
	{
		return "Day1";
	}

	static Result_t Part1(const Data_t& data)
	{
		//std::cout << "Part1" << std::endl;

		return 1;
	}

	static Result_t Part2(const Data_t& data)
	{
		//std::cout << "Part2" << std::endl;
		return 2;
	}
};