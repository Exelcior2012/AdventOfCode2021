
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <queue>

struct Day15 : public AoC::PuzzleBase<Day15>
{

	struct Point
	{
		int X;
		int Y;

		auto operator<=>(const Point& other) const = default;

		Point& operator-=(const Point& other)
		{
			X -= other.X;
			Y -= other.Y;
			return *this;
		}

		Point& operator+=(const Point& other)
		{
			X += other.X;
			Y += other.Y;
			return *this;
		}

		friend Point operator-(const Point& A, const Point& B)
		{
			Point temp = A;
			temp -= B;
			return temp;
		}

		friend Point operator+(const Point& A, const Point& B)
		{
			Point temp = A;
			temp += B;
			return temp;
		}
	};

	template<typename T>
	struct TGrid2D
	{
		friend struct Data;
		friend std::istream& operator>>(std::istream& in, Data& out);

		using Elem_t = T;

		struct index_t
		{
			size_t index;
			int xTile;
			int yTile;
		};

		std::vector<Elem_t> Values;

		int Width;
		int Height;
		int TilesX = 1;
		int TilesY = 1;

		int FullWidth() const
		{
			return Width * TilesX;
		}

		int FullHeight() const
		{
			return Height * TilesY;
		}

		bool IsValid(const Point& p) const
		{
			return p.X >= 0 && p.X < FullWidth() && p.Y >= 0 && p.Y < FullHeight();
		}

		size_t FullIndexAt(const Point& p) const
		{
			return p.Y* FullWidth() + p.X;
		}

		Elem_t ValueAt(size_t index) const
		{
			int x = index % FullWidth();
			int y = index / FullWidth();
			int tileX = x / Width;
			int tileY = y / Height;

			Elem_t value = static_cast<Elem_t>(Values[(y % Height) * Width + (x % Width)] + tileX + tileY);
			return 1 + (value-1) % 9;
		}

		Elem_t ValueAt(const Point& p) const
		{
			int tileX = p.X / Width;
			int tileY = p.Y / Height;

			Elem_t value = static_cast<Elem_t>(Values[(p.Y % Height) * Width + (p.X % Width)] + tileX + tileY);
			return 1 + (value - 1) % 9;
		}

		template<typename Callable>
		void ForEachNeighbor(const Point& p, Callable c) const
		{
			// no diagonal movement, only cardinal
			static constexpr Point Neighbors[] = {
				Point{-1,0}, Point{0,-1},
				Point{1,0}, Point{0,1}
			};

			for(const Point& n : Neighbors)
			{
				Point neighborPoint = p + n;
				if(IsValid(neighborPoint))
				{
					c(neighborPoint, FullIndexAt(neighborPoint));
				}
			}
		}
	};

	struct Data
	{
		TGrid2D<uint8_t> Grid;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;

			out.Grid.Height = 0;

			while(in.good())
			{
				getline(in, line);
				out.Grid.Width = line.length();
				++out.Grid.Height;

				for(char c : line)
				{
					out.Grid.Values.push_back(c - '0');
				}
			}

			return in;
		}
	};

	template<typename T>
	static int AStar(const TGrid2D<T>& grid, Point a, Point b)
	{
		auto Heuristic = [&](const Point& p) -> int
		{
			// Manhattan distance
			Point temp = b - p;
			return Util::Abs(temp.X + temp.Y);
		};

		const size_t numEntries = grid.FullWidth() * grid.FullHeight();

		std::vector<bool> inOpenSet(numEntries, false);
		std::vector<int> knownScores(numEntries, std::numeric_limits<int>::max());
		std::vector<int> possibleScores(numEntries, std::numeric_limits<int>::max());

		auto Priority = [&](const Point& a, const Point& b)
		{
			return possibleScores[grid.FullIndexAt(a)] > possibleScores[grid.FullIndexAt(b)];
		};

		std::priority_queue<Point, std::vector<Point>, decltype(Priority)> openSet(Priority);

		const size_t startIndex = grid.FullIndexAt(a);
		openSet.push(a);
		inOpenSet[startIndex] = true;
		knownScores[startIndex] = 0;
		possibleScores[startIndex] = Heuristic(a);

		Point current;
		while(!openSet.empty())
		{
			current = openSet.top();
			openSet.pop();

			const size_t currentIndex = grid.FullIndexAt(current);
			if(current == b)
			{
				// Found the path
				return knownScores[currentIndex];
			}

			grid.ForEachNeighbor(current, [&](const Point& p, size_t nextIndex)
			{
				int possibleScore = knownScores[currentIndex] + grid.ValueAt(p);
				if(possibleScore < knownScores[nextIndex])
				{
					knownScores[nextIndex] = possibleScore;
					possibleScores[nextIndex] = possibleScore + Heuristic(p);

					if(!inOpenSet[nextIndex])
					{
						inOpenSet[nextIndex] = true;
						openSet.push(p);
					}
				}
			});
		}

		// failed
		return -1;
	}

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/15/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 15: Chiton";
	}

	static int64_t Part1(const Data& data)
	{
		return AStar(data.Grid, {0, 0}, {data.Grid.FullWidth() - 1, data.Grid.FullHeight() - 1});
	}

	static int64_t Part2(const Data& data)
	{
		Data copy = data;
		copy.Grid.TilesX = 5;
		copy.Grid.TilesY = 5;
		
		return AStar(copy.Grid, {0, 0}, {copy.Grid.FullWidth() - 1, copy.Grid.FullHeight() - 1});
	}
};
