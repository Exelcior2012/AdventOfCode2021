
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>

struct Point_t
{
	int X;
	int Y;

	Point_t& operator+=(const Point_t& other)
	{
		X += other.X;
		Y += other.Y;
		return *this;
	}

	friend Point_t operator +(const Point_t& a, const Point_t& b)
	{
		Point_t temp = a;
		temp += b;
		return temp;
	}

	bool operator==(const Point_t& other) const
	{
		return X == other.X && Y == other.Y;
	}
};

constexpr static Point_t neighbors[] = {
				{-1, 0}, {1, 0}, {0, -1}, {0, 1},
};

struct Day9 : public AoC::PuzzleBase<Day9>
{

	struct Data
	{
		size_t IndexAt(int x, int y) const
		{
			return y * Width + x;
		}

		uint8_t At(int x, int y) const
		{
			if(x < 0 || x >= Width || y < 0 || y >= Height)
			{
				return std::numeric_limits<uint8_t>::max();
			}

			return Values[y * Width + x];
		}

		bool IsMinima(int x, int y) const
		{
			const uint8_t value = At(x, y);

			for(const Point_t n : neighbors)
			{
				Point_t current{x, y};
				current += n;

				const uint8_t neighbor = At(current.X, current.Y);
				if(value >= neighbor)
				{
					return false;
				}
			}

			return true;
		}

		std::vector<uint8_t> Values;
		int Width = 0;
		int Height = 0;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			std::string temp;
			while(in.good())
			{
				std::getline(in, temp);

				if(out.Width == 0) out.Width = temp.length();
				++out.Height;
				
				out.Values.reserve(out.Values.size() + out.Width);

				for(char c : temp)
				{
					out.Values.push_back(c - '0');
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

		std::ifstream file("Data/09/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 9: Smoke Basin";
	}

	static Result_t Part1(const Data_t& data)
	{
		int risk = 0;
		for(int y = 0; y < data.Height; ++y)
		{
			for(int x = 0; x < data.Width; ++x)
			{
				if(data.IsMinima(x, y))
				{
					risk += data.At(x, y) + 1;
				}
			}
		}
		return risk;
	}

	static Result_t Part2(const Data_t& data)
	{
		auto ExpandFrom = [&](int x, int y) -> int
		{
			std::vector<Point_t> basin;
			std::vector<Point_t> queue;
			queue.push_back({x, y});
			basin.push_back({x, y});

			while(queue.size() > 0)
			{
				Point_t pt = queue.back();
				queue.pop_back();

				for(const Point_t n : neighbors)
				{
					Point_t current{pt.X, pt.Y};
					current += n;

					if(current.X >= 0 && current.X < data.Width && current.Y >= 0 && current.Y < data.Height)
					{
						uint8_t value = data.At(current.X, current.Y);

						if(value != 9 && std::find(std::begin(basin), std::end(basin), current) == std::end(basin))
						{
							basin.push_back(current);
							queue.push_back(current);
						}
					}
				}
			}

			return basin.size();
		};

		std::vector<int> basinSizes;

		for(int y = 0; y < data.Height; ++y)
		{
			for(int x = 0; x < data.Width; ++x)
			{
				if(data.IsMinima(x, y))
				{
					basinSizes.push_back(ExpandFrom(x, y));
				}
			}
		}

		std::sort(std::begin(basinSizes), std::end(basinSizes));
		const size_t lastIndex = basinSizes.size() - 1;

		return basinSizes[lastIndex - 2] * basinSizes[lastIndex - 1] * basinSizes[lastIndex];
	}
};

