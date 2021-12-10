
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>

struct Day10 : public AoC::PuzzleBase<Day10>
{

	struct Data
	{
		std::vector<std::string> Lines;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			std::string temp;
			while(in.good())
			{
				std::getline(in, temp);
				out.Lines.push_back(temp);
			}

			return in;
		}
	};

	using Data_t = Data;
	using Result_t = int64_t;

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/10/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 10: Syntax Scoring";
	}

	static int GetIllegalScore(char in)
	{
		switch(in)
		{
			case ')': return 3;
			case ']': return 57;
			case '}': return 1197;
			case '>': return 25137;
			default: return 0;
		}
	}

	static int GetClosingScore(char in)
	{
		switch(in)
		{
			case ')': return 1;
			case ']': return 2;
			case '}': return 3;
			case '>': return 4;
			default: return 0;
		}
	}

	static bool IsOpening(char in)
	{
		return in == '(' || in == '[' || in == '<' || in == '{';
	}

	static char GetClosing(char in)
	{
		switch(in)
		{
			case '(': return in + 1;
			default: return in + 2;
		}
	}

	static char InterpretLine(std::string_view s, std::vector<char>& symbolStack)
	{
		symbolStack.clear();

		for(char c : s)
		{
			if(IsOpening(c))
			{
				symbolStack.push_back(c);
			}
			else if(c == GetClosing(symbolStack.back()))
			{
				symbolStack.pop_back();
			}
			else
			{
				return c;
			}
		}
		return 0;
	}

	static Result_t Part1(const Data_t& data)
	{
		int score = 0;
		std::vector<char> symbolStack;
		for(const std::string& s : data.Lines)
		{
			const char illegalChar = InterpretLine(s, symbolStack);

			if(illegalChar != 0)
			{
				score += GetIllegalScore(illegalChar);
			}
		}

		return score;
	}

	static Result_t Part2(const Data_t& data)
	{
		std::vector<char> symbolStack;
		std::vector<int64_t> lineScores;
		std::string closingSuffix = "";
		for(const std::string& s : data.Lines)
		{
			const char illegalChar = InterpretLine(s, symbolStack);

			if(illegalChar == 0) // valid line
			{
				int64_t score = 0;
				while(symbolStack.size() > 0)
				{
					score *= 5;
					score += GetClosingScore(GetClosing(symbolStack.back()));
					symbolStack.pop_back();
				}
				lineScores.push_back(score);
			}
		}

		std::sort(std::begin(lineScores), std::end(lineScores));

		return lineScores[lineScores.size() / 2];
	}
};

