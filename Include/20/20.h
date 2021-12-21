
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <string>

struct Day20 : public AoC::PuzzleBase<Day20>
{
	struct Data
	{
		std::string Lookup = "";
		std::vector<std::string> Image;
		int Width = 0;
		int Height = 0;

		Data CopyExpanded(int expansion) const
		{
			Data newData;
			newData.Width = Width + expansion * 2;
			newData.Height = Height + expansion * 2;
			std::string fullExpansion(newData.Width, 'i');
			std::string partExpansion(expansion, 'i');

			for(int i = 0; i < expansion; ++i)
			{
				newData.Image.push_back(fullExpansion);
			}

			for(int i = 0; i < Height; ++i)
			{
				newData.Image.push_back(partExpansion + Image[i] + partExpansion);
			}

			for(int i = 0; i < expansion; ++i)
			{
				newData.Image.push_back(fullExpansion);
			}

			return newData;
		}

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;
			getline(in, out.Lookup);
			getline(in, line);
			assert(line == "");
			while(in.good())
			{
				getline(in, line);
				out.Width = line.size();
				out.Image.push_back(line);
				out.Height++;
			}

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/20/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 20: Trench Map";
	}

	static void PrintImage(const std::vector<std::string>& image)
	{
		for(auto& s : image)
		{
			Util::Log(s);
		}
	}

	static int64_t DoEnhance(const Data& baseData, int iterations)
	{
		// A copy with the minimal border of the infinite field to handle
		// the number of iterations.
		Data copy = baseData.CopyExpanded(iterations + 1);

		// Get a char at a point while handling the further infinite values
		char infChar = '.';
		auto at = [&](int x, int y)
		{
			const char c = copy.Image[y][x];
			return c == 'i' ? infChar : c;
		};

		std::vector<std::string> nextImage = copy.Image;

		for(int i = 0; i < iterations; ++i)
		{
			// Convolve the 3x3 enhancement kernel over the image
			for(int y = iterations - i; y < copy.Height - iterations + i; ++y)
			{
				for(int x = iterations - i; x < copy.Width - iterations + i; ++x)
				{
					uint16_t value = 0;
					value |= (at(x - 1, y - 1) == '#' ? 1 : 0) << 8;
					value |= (at(x    , y - 1) == '#' ? 1 : 0) << 7;
					value |= (at(x + 1, y - 1) == '#' ? 1 : 0) << 6;
					value |= (at(x - 1, y    ) == '#' ? 1 : 0) << 5;
					value |= (at(x    , y    ) == '#' ? 1 : 0) << 4;
					value |= (at(x + 1, y    ) == '#' ? 1 : 0) << 3;
					value |= (at(x - 1, y + 1) == '#' ? 1 : 0) << 2;
					value |= (at(x    , y + 1) == '#' ? 1 : 0) << 1;
					value |= (at(x + 1, y + 1) == '#' ? 1 : 0) << 0;

					nextImage[y][x] = baseData.Lookup[value];
				}
			}
			
			// Push back to the copied image
			copy.Image = nextImage;

			// Handle the case of Lookup[0] being '#' which will mean everthing
			// in the larger image will flip-flop between '.' and '#' between
			// each iteration
			if(infChar == '.')
			{
				infChar = baseData.Lookup[0];
			}
			else
			{
				infChar = '.';
			}
		}

		// Count up the lit cells
		int64_t count = 0;
		for(auto& s : copy.Image)
		{
			count += std::count(s.begin(), s.end(), '#');
		}

		return count;
	}

	static int64_t Part1(const Data& data)
	{
		return DoEnhance(data, 2);
	}

	static int64_t Part2(const Data& data)
	{
		return DoEnhance(data, 50);
	}
};

