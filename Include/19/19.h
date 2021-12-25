
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <string>
#include <limits>

#include "Util/Vec3.h"
#include "Util/Box3.h"

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
