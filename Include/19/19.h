
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <string>
#include <limits>

struct Vec3i
{
	Vec3i() = default;

	Vec3i(int v)
		: X(v)
		, Y(v)
		, Z(v)
	{}

	Vec3i(int x, int y, int z)
		: X(x)
		, Y(y)
		, Z(z)
	{}

	int X;
	int Y;
	int Z;

	static Vec3i Limit_Max;
	static Vec3i Limit_Min;

	int64_t Product() const
	{
		return static_cast<int64_t>(X) * static_cast<int64_t>(Y) * static_cast<int64_t>(Z);
	}

	Vec3i& operator -=(const Vec3i& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}

	Vec3i& operator +=(const Vec3i& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}

	friend std::strong_ordering operator<=>(const Vec3i& a, const Vec3i& b) = default;

	friend Vec3i operator-(const Vec3i& a, const Vec3i& b)
	{
		Vec3i temp = a;
		temp -= b;
		return temp;
	}

	friend Vec3i operator+(const Vec3i& a, const Vec3i& b)
	{
		Vec3i temp = a;
		temp += b;
		return temp;
	}

	friend std::istream& operator>>(std::istream& in, Vec3i& out)
	{
		char commaDiscard;
		in >> out.X >> commaDiscard >> out.Y >> commaDiscard >> out.Z;
		return in;
	}

	friend std::ostream& operator<<(std::ostream& out, const Vec3i& in)
	{
		out << in.X << ',' << in.Y << ',' << in.Z;
		return out;
	}
};

Vec3i Vec3i::Limit_Max = Vec3i(std::numeric_limits<int>::max());
Vec3i Vec3i::Limit_Min = Vec3i(-std::numeric_limits<int>::lowest());

struct Box3Di
{
	Vec3i Min = Vec3i::Limit_Max;
	Vec3i Max = Vec3i::Limit_Min;

	bool IsValid() const
	{
		return Max > Min;
	}

	void GrowToInclude(const Vec3i& vec)
	{
		Min.X = std::min(Min.X, vec.X);
		Min.Y = std::min(Min.Y, vec.Y);
		Min.Z = std::min(Min.Z, vec.Z);

		Max.X = std::max(Max.X, vec.X);
		Max.Y = std::max(Max.Y, vec.Y);
		Max.Z = std::max(Max.Z, vec.Z);
	}

	Vec3i Extent() const
	{
		return Max - Min;
	}

	int64_t Volume() const
	{
		return Extent().Product();
	}
};

struct Day19 : public AoC::PuzzleBase<Day19>
{
	struct Data
	{
		struct ScannerReport
		{
			std::vector<Vec3i> Beacons;
			Box3Di SearchArea;
		};
		std::vector<ScannerReport> Reports;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;
			while(in.good())
			{
				getline(in, line);
				assert(line[0] == '-'); // should be a name line
				getline(in, line);

				out.Reports.emplace_back();
				ScannerReport& report = out.Reports.back();

				do
				{
					std::stringstream ss(line);
					Vec3i newBeacon;
					ss >> newBeacon;

					report.Beacons.push_back(newBeacon);
					report.SearchArea.GrowToInclude(newBeacon);

					getline(in, line);
				}
				while(!line.empty() && in.good());

				Util::Log("Beacon ", out.Reports.size() - 1, " Volume: ", out.Reports.back().SearchArea.Volume(), ", Extent:", out.Reports.back().SearchArea.Extent());
			}

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/19/testcase.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 19: Beacon Scanner";
	}

	static int64_t Part1(const Data& data)
	{
		return 0;
	}

	static int64_t Part2(const Data& data)
	{
		return 0;
	}
};
