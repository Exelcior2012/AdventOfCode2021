
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <vector>

struct Day8 : public AoC::PuzzleBase<Day8>
{
	constexpr static uint8_t LetterBits[7] = {
		0b01000000, // a
		0b00100000, // b
		0b00010000, // c
		0b00001000, // d
		0b00000100, // e
		0b00000010, // f
		0b00000001, // g
	};

	struct Data
	{
		struct Entry
		{
			uint8_t Inputs[10] = {};
			uint8_t Outputs[4] = {};
		};

		std::vector<Entry> Entries;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			std::string temp;
			char delim;
			while(in.good())
			{
				out.Entries.emplace_back();
				Entry& entry = out.Entries.back();

				for(int i = 0; i < 10; ++i)
				{
					in >> temp;

					uint8_t newValue = 0;
					for(size_t j = 0; j < temp.size(); ++j)
					{
						newValue |= LetterBits[temp[j] - 'a'];
					}

					entry.Inputs[i] = newValue;
				}

				in >> delim;

				for(int i = 0; i < 4; ++i)
				{
					in >> temp;

					uint8_t newValue = 0;
					for(size_t j = 0; j < temp.size(); ++j)
					{
						newValue |= LetterBits[temp[j] - 'a'];
					}

					entry.Outputs[i] = newValue;
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

		std::ifstream file("Data/08/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 8: Seven Segment Search";
	}

	static Result_t Part1(const Data_t& data)
	{
		int count = 0;
		for(const Data::Entry& e : data.Entries)
		{
			for(uint8_t segment : e.Outputs)
			{
				const int bitCount = Util::CountBits(segment);

				if(bitCount == 2 || bitCount == 3 || bitCount == 4 || bitCount == 7)
				{
					++count;
				}
			}
		}

		return count;
	}

	static Result_t Part2(const Data_t& data)
	{
		int64_t total = 0;
		for(const Data::Entry& e : data.Entries)
		{
			std::array<uint8_t, 10> patterns;
			
			// Get the bit patterns of the unique digits (1/4/7/8)
			for(uint8_t p : e.Inputs)
			{
				const int bitCount = Util::CountBits(p);

				switch(bitCount)
				{
					case 2:
					{
						patterns[1] = p;
						break;
					}
					case 3:
					{
						patterns[7] = p;
						break;
					}
					case 4:
					{
						patterns[4] = p;
						break;
					}
					case 7:
					{
						patterns[8] = p;
						break;
					}
					default:
						break;
				}
			}

			// Use the unique patterns to calculate the others
			for(uint8_t p : e.Inputs)
			{
				const int bitCount = Util::CountBits(p);

				switch(bitCount)
				{
					case 5:
					{
						// could be 2/3/5
						if(Util::CountBits(patterns[4] & p) == 2)
						{
							patterns[2] = p;
						}
						else if((patterns[1] & p) == patterns[1])
						{
							patterns[3] = p;
						}
						else
						{
							patterns[5] = p;
						}
						break;
					}
					case 6:
					{
						// could be 6/9/0
						if((patterns[7] & p) != patterns[7])
						{
							patterns[6] = p;
						}
						else if((patterns[4] & p) == patterns[4])
						{
							patterns[9] = p;
						}
						else
						{
							patterns[0] = p;
						}
						break;
					}
					default:
						break;
				}
			}

			// Decode outputs
			int output = 0;
			for(uint8_t p : e.Outputs)
			{
				output *= 10;
				for(int i = 0; i < 10; ++i)
				{
					if(p == patterns[i])
					{
						output += i;
					}
				}
			}

			total += output;
		}

		return total;
	}
};

