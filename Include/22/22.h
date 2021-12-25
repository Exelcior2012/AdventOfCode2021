
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <string>
#include <regex>

const static std::regex inputPattern("(on|off) x=(\\d+)\\.\\.(\\d+),y=(\\d+)\\.\\.(\\d+),z=(\\d+)\\.\\.(\\d+)");

struct Day22 : public AoC::PuzzleBase<Day22>
{
	struct Data
	{
		struct ReactorRule
		{
			enum class EState { On, Off };
			EState State = EState::On;
			Box3Di Box;
		};

		std::vector<ReactorRule> Rules;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;

			while(in.good())
			{
				getline(in, line);

				std::smatch m;
				assert(std::regex_match(line, m, inputPattern));
				assert(m.size() == 8);

				out.Rules.emplace_back();
				ReactorRule& rule = out.Rules.back();

				if(m[1] == "off")
				{
					rule.State = ReactorRule::EState::Off;
				}

				rule.Box.Min = Vec3i(stoi(m[2]), stoi(m[4]), stoi(m[6]));
				rule.Box.Max = Vec3i(stoi(m[3]) + 1, stoi(m[5]) + 1, stoi(m[7]) + 1);
			}

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/22/testcase.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 22: Reactor Reboot";
	}

	static int64_t Part1(const Data& data)
	{
		const Box3Di fieldArea({-50, -50, -50}, {50, 50, 50});

		int64_t numEnabled = 0;
		for(const Data::ReactorRule& r : data.Rules)
		{
			if(!fieldArea.Intersects(r.Box)) continue;

			if(r.State == Data::ReactorRule::EState::On)
			{
				numEnabled += r.Box.Volume();
			}
		}

		return 0;
	}

	static int64_t Part2(const Data& data)
	{
		return 0;
	}
};

