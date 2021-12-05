
#pragma once

#include "PuzzleFramework.h"

#include <fstream>

struct Point
{
	int X = 0;
	int Y = 0;

	friend std::istream& operator >>(std::istream& in, Point& out)
	{
		char temp;
		in >> out.X >> temp >> out.Y;

		return in;
	}
};

struct Line
{
	Point A;
	Point B;

	bool IsOrthogonal() const
	{
		return A.X == B.X || A.Y == B.Y;
	}

	Point Min() const
	{
		return {std::min(A.X, B.X), std::min(A.Y, B.Y)};
	}

	Point Max() const
	{
		return {std::max(A.X, B.X), std::max(A.Y, B.Y)};
	}

	friend std::istream& operator >>(std::istream& in, Line& out)
	{
		char temp;
		in >> out.A >> temp >> temp >> out.B;

		return in;
	}
};

template<typename T>
T Abs(T&& value)
{
	return value < 0 ? -value : value;
}

void PlotLine(std::vector<uint8_t>& state, int width, const Point& origin, const Line& line)
{
	auto [x0, y0] = line.A;
	auto [x1, y1] = line.B;

	// Offset to put everything into grid-space
	x0 -= origin.X;
	x1 -= origin.X;
	y0 -= origin.Y;
	y1 -= origin.Y;

	const int dx = Abs(x1 - x0);
	const int signx = x0 < x1 ? 1 : -1;
	const int dy = -Abs(y1 - y0);
	const int signy = y0 < y1 ? 1 : -1;
	
	int error = dx + dy;

	for(;;)
	{
		// Draw onto the grid
		state[y0 * width + x0]++;

		// Once we reach the end we're done
		if(x0 == x1 && y0 == y1) break;

		const int doubleError = 2 * error;

		if(doubleError >= dy)
		{
			error += dy;
			x0 += signx;
		}

		if(doubleError <= dx)
		{
			error += dx;
			y0 += signy;
		}
	}
}

struct Day5 : public AoC::PuzzleBase<Day5>
{
	struct Data
	{
		std::vector<Line> Lines;
		Point Min;
		Point Max;
		
		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			while(in.good())
			{
				Line temp;
				in >> temp;
				out.Lines.push_back(temp);

				if(out.Lines.size() == 1)
				{
					out.Min = out.Lines[0].Min();
					out.Max = out.Lines[0].Max();
				}
				else
				{
					out.Min = {std::min(out.Min.X, temp.Min().X), std::min(out.Min.Y, temp.Min().Y)};
					out.Max = {std::max(out.Max.X, temp.Max().X), std::max(out.Max.Y, temp.Max().Y)};
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

		std::ifstream file("Data/05/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 5: Hydrothermal Venture";
	}

	enum class ELineType
	{
		OrthogonalOnly,
		All
	};

	template<ELineType DrawType>
	static auto CountLineIntersections(const Data_t& data)
	{
		const int width = data.Max.X - data.Min.X + 1;
		const int height = data.Max.Y - data.Min.Y + 1;
		std::vector<uint8_t> state(width * height, 0);

		for(const Line& line : data.Lines)
		{
			if constexpr(DrawType == ELineType::OrthogonalOnly)
			{
				if(!line.IsOrthogonal())
				{
					continue;
				}
			}

			PlotLine(state, width, data.Min, line);
		}

		return std::count_if(std::begin(state), std::end(state), [](auto v) { return v > 1; });
	}

	static Result_t Part1(const Data_t& data)
	{
		return CountLineIntersections<ELineType::OrthogonalOnly>(data);
	}

	static Result_t Part2(const Data_t& data)
	{
		return CountLineIntersections<ELineType::All>(data);
	}
};
