
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <tuple>

struct Day13 : public AoC::PuzzleBase<Day13>
{

	struct Data
	{
		struct Point
		{
			int X = 0;
			int Y = 0;

			void ReflectX(int at)
			{
				X += (at - X) * 2;
			}

			void ReflectY(int at)
			{
				Y += (at - Y) * 2;
			}

			std::strong_ordering operator<=>(const Point& other) const = default;
		};

		std::vector<Point> Points;
		std::vector<Point> Folds;
		int Width = 0;
		int Height = 0;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;
			string temp;
			char discard;
			
			while(in.good())
			{
 				getline(in, line);
				stringstream ss(line);

				// Skip empty lines
				if(line == "") continue;

				// If theres a comma then it's a point
				if(line.find(',') != std::string::npos)
				{
					Point newPoint;
					ss >> newPoint.X >> discard >> newPoint.Y;
					out.Points.push_back(newPoint);

					out.Width = max(out.Width, newPoint.X + 1);
					out.Height = max(out.Height, newPoint.Y + 1);
				}
				else
				{
					// Otherwise a fold
					Point newFold;
					ss >> temp >> temp >> discard;
					
					if(discard == 'x')
					{
						ss >> discard >> newFold.X;
					}
					else
					{
						ss >> discard >> newFold.Y;
					}

					out.Folds.push_back(newFold);
				}
			}

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/13/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 13: Transparent Origami";
	}

	static void PrintBoard(const std::vector<Data::Point>& points, int width, int height)
	{
		std::vector<char> board(width * height, '.');

		for(const Data::Point& p : points)
		{
			const size_t index = p.Y * width + p.X;
			board[index] = '#';
		}

		size_t idx = 0;
		for(int y = 0; y < height; ++y)
		{
			for(int x = 0; x < width; ++x)
			{
				Util::Log<false>(board[idx++]);
			}
			Util::Log("");
		}
		Util::Log("");
	}

	static void DoFold(std::vector<Data::Point>& points, const Data::Point& fold)
	{
		// Reflect all points above the fold in the correct direction
		if(fold.X > 0)
		{
			for(Data::Point& p : points)
			{
				if(p.X > fold.X)
				{
					p.ReflectX(fold.X);
				}
			}
		}
		else
		{
			for(Data::Point& p : points)
			{
				if(p.Y> fold.Y)
				{
					p.ReflectY(fold.Y);
				}
			}
		}
	}

	static int64_t Part1(const Data& data)
	{
		int width = data.Width;
		int height = data.Height;

		std::vector<Data::Point> copy = data.Points;
		
		// Only care about the first fold for part 1
		const Data::Point& fold = data.Folds[0];

		DoFold(copy, fold);

		if(fold.X > 0)
		{
			width = fold.X;
		}
		else
		{
			height = fold.Y;
		}

		// sort/unique will give us a subset that contains only unique elements
		std::sort(std::begin(copy), std::end(copy));
		auto newEnd = std::unique(std::begin(copy), std::end(copy));

		// Return length of the unique set
		return std::distance(std::begin(copy), newEnd);
	}

	// Need a >> operator to print out the board, so wrap in a type to handle that
	struct BoardResult
	{
		std::vector<Data::Point> Points;
		int Width = 0;
		int Height = 0;

		friend std::ostream& operator<<(std::ostream& out, const BoardResult& data)
		{
			out << "\n";

			std::vector<char> board(data.Width * data.Height, '.');

			for(const Data::Point& p : data.Points)
			{
				const size_t index = p.Y * data.Width + p.X;
				board[index] = '#';
			}

			size_t idx = 0;
			for(int y = 0; y < data.Height; ++y)
			{
				out << "\t\t";
				for(int x = 0; x < data.Width; ++x)
				{
					out << board[idx++];
				}
				out << "\n";
			}
			out << "\n";
			
			return out;
		}
	};

	static BoardResult Part2(const Data& data)
	{
		int width = data.Width;
		int height = data.Height;

		std::vector<Data::Point> copy = data.Points;

		// Do all folds this time
		for(const Data::Point& fold : data.Folds)
		{
			DoFold(copy, fold);

			if(fold.X > 0)
			{
				width = fold.X;
			}
			else
			{
				height = fold.Y;
			}
		}

		// Build result
		BoardResult r;
		r.Points = std::move(copy);
		r.Width = width;
		r.Height = height;

		return r;
	}
};
