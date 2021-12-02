
#pragma once

#include "PuzzleFramework.h"

#include <fstream>
#include <numeric>

struct Day2 : public AoC::PuzzleBase<Day2>
{
	struct Move
	{
		int Horizontal = 0;
		int Vertical = 0;
		int Aim = 0;

		int Product() const
		{
			return Horizontal * Vertical;
		}

		void AddSimple(const Move& other)
		{
			Horizontal += other.Horizontal;
			Vertical += other.Vertical;
		}

		void AddWithAim(const Move& other)
		{
			Aim += other.Vertical;
			Horizontal += other.Horizontal;
			Vertical += Aim * other.Horizontal;
		}

		friend std::istream& operator >>(std::istream& in, Move& out)
		{
			std::string dir;
			int val;
			out = Move();

			in >> dir;
			in >> val;

			if(dir == "forward")
			{
				out.Horizontal = val;
			}
			else if(dir == "up")
			{
				out.Vertical = -val;
			}
			else
			{
				out.Vertical = val;
			}

			return in;
		}
	};

	struct Data
	{
		std::vector<Move> Moves;
	};

	using Data_t = Data;
	using Result_t = int64_t;

	static Data Setup()
	{
		Data data;

		std::ifstream file("../Include/02/input.txt");

		Move temp;
		while(file.good())
		{
			file >> temp;
			data.Moves.push_back(temp);
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 2: Dive!";
	}

	static Result_t Part1(const Data_t& data)
	{
		Move total;
		for(const Move& move : data.Moves)
		{
			total.AddSimple(move);
		}

		return total.Product();
	}

	static Result_t Part2(const Data_t& data)
	{
		Move total;
		for(const Move& move : data.Moves)
		{
			total.AddWithAim(move);
		}

		return total.Product();
	}
};