
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>

struct Day11 : public AoC::PuzzleBase<Day11>
{

	struct Data
	{
		struct OctoGrid
		{
			std::vector<uint8_t> Values;
			int Width = 0;
			int Height = 0;
		};

		OctoGrid grid;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			std::string temp;
			while(in.good())
			{
				std::getline(in, temp);

				if(out.grid.Width == 0) out.grid.Width = temp.length();
				++out.grid.Height;

				out.grid.Values.reserve(out.grid.Values.size() + out.grid.Width);

				for(char c : temp)
				{
					out.grid.Values.push_back(c - '0');
				}
			}

			return in;
		}
	};

	using Data_t = Data;
	using Result_t = int64_t;

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/11/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 11: Dumbo Octopus";
	}

	template<typename Callable>
	static void ForEachNeighbor(Data::OctoGrid& grid, size_t origin, Callable c)
	{
		auto DoInvoke = [&](int index, auto callable)
		{
			if(index >= 0 && index < static_cast<int>(grid.Values.size()))
			{
				callable(index);
			}
		};

		int x = origin % grid.Width;
		
		if(x != 0)
		{
			DoInvoke(origin - grid.Width - 1, c);
			DoInvoke(origin - 1, c);
			DoInvoke(origin + grid.Width - 1, c);
		}

		if(x != grid.Width - 1)
		{
			DoInvoke(origin - grid.Width + 1, c);
			DoInvoke(origin + 1, c);
			DoInvoke(origin + grid.Width + 1, c);
		}

		DoInvoke(origin - grid.Width, c);
		DoInvoke(origin + grid.Width, c);
	}

	static int64_t Tick(Data::OctoGrid& grid)
	{
		// First, increment all and track those that go above 9
		int64_t numFlashes = 0;
		std::vector<size_t> toFlashStack;
		std::vector<size_t> toZeroStack;

		size_t count = 0;
		for(uint8_t& val : grid.Values)
		{
			// First time we go over 9, only one flash per tick per octopus
			if(++val == 10)
			{
				toFlashStack.push_back(count);
			}
			++count;
		}

		// Perform flashes
		while(toFlashStack.size() > 0)
		{
			size_t index = toFlashStack.back();
			toFlashStack.pop_back();
			numFlashes++;

			ForEachNeighbor(grid, index, [&](int innerIndex)
			{
				// First time we go over 9, only one flash per tick per octopus
				if(++grid.Values[innerIndex] == 10)
				{
					toFlashStack.push_back(innerIndex);
				}
			});

			toZeroStack.push_back(index);
		}

		// Zero out all flashed octopi
		while(toZeroStack.size() > 0)
		{
			grid.Values[toZeroStack.back()] = 0;
			toZeroStack.pop_back();
		}

		return numFlashes;
	}

	static int64_t TickN(Data::OctoGrid& grid, int num)
	{
		int64_t result = 0;
		while(--num >= 0)
		{
			result += Tick(grid);
		}
		return result;
	}

	static int64_t FirstSync(Data::OctoGrid& grid)
	{
		int64_t count = 1;
		while(Tick(grid) != grid.Values.size())
		{
			++count;
		}
		return count;
	}

	static Result_t Part1(const Data_t& data)
	{
		Data::OctoGrid mutGrid = data.grid;

		return TickN(mutGrid, 100);
	}

	static Result_t Part2(const Data_t& data)
	{
		Data::OctoGrid mutGrid = data.grid;
		
		return FirstSync(mutGrid);
	}
};

