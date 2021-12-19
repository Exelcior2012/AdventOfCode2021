
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <string>
#include <regex>

static const std::regex InputRegex("[xy]=(-?\\d+)\\.\\.(-?\\d+)");

struct Day17 : public AoC::PuzzleBase<Day17>
{
	struct Data
	{
		int MinX = -1;
		int MaxX = -1;
		int MinY = -1;
		int MaxY = -1;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;
			getline(in, line);
			
// 			//std::smatch match;
// 			auto start = std::begin(line);
// 			auto end = std::end(line);
// 			std::match_results<decltype(start)> match;
// 			if(std::regex_search(line.begin(), line.end(), match, InputRegex))
// 			{
// 				const size_t numMatches = match.size();
// 
// 				std::stringstream ss(match.str(1));
// 				ss >> out.MinX;
// 				ss.str(match.str(2));
// 				ss >> out.MaxX;
// 
// 				
// 			}
// 
// 			line = match.suffix().str();
// 			start = std::begin(line);
// 			end = std::end(line);
// 			if(std::regex_search(line.begin(), line.end(), match, InputRegex))
// 			{
// 				std::stringstream ss(match.str(1));
// 				ss >> out.MinY;
// 
// 				ss.str(match.str(2));
// 				ss >> out.MaxY;
// 			}

			int values[4] = {-1, -1, -1, -1};
			size_t count = 0;
			for(auto i = std::sregex_iterator(line.begin(), line.end(), InputRegex);
				i != std::sregex_iterator();
				++i)
			{
				std::smatch m = *i;
				std::stringstream ss(m.str(1));
				ss >> values[count++];
				ss.str(m.str(2));
				ss.clear();
				ss >> values[count++];
			}

			out.MinX = values[0];
			out.MaxX = values[1];
			out.MinY = values[2];
			out.MaxY = values[3];

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/17/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 17: Trick Shot";
	}

	static int64_t Part1(const Data& data)
	{
		// Closed form, assuming the target is always in the +x/-y quadrant
		// and there's a triangular number between minX and maxX
		return data.MinY * (data.MinY + 1) / 2;
	}

	static int64_t Part2(const Data& data)
	{
		int boundXMin = static_cast<int>(sqrt(data.MinX * 2.0f));
		int boundXMax = data.MaxX + 1;
		int boundYMin = data.MinY;
		int boundYMax = -data.MinY;

		// Doesn't seem to be a closed form solution for p2, but we can reasonably
		// restrict search bounds for the problem to limit the cost
		int count = 0;
		for(int xi = boundXMin; xi < boundXMax; ++xi)
		{
			for(int yi = boundYMin; yi < boundYMax; ++yi)
			{
				int x = 0;
				int y = 0;
				int xVel = xi;
				int yVel = yi;

				while(x <= data.MaxX && y >= data.MinY)
				{
					if(x >= data.MinX && y <= data.MaxY)
					{
						++count;
						break;
					}

					// advance simulation
					x += xVel;
					y += yVel;
					--yVel;
					xVel = std::max(0, xVel - 1);

				}
			}
		}

		return count;
	}
};
