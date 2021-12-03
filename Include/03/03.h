
#pragma once

#include "PuzzleFramework.h"

#include <fstream>

struct Day3 : public AoC::PuzzleBase<Day3>
{
	template<typename Int_t> requires std::integral<Int_t>
	static Int_t FromBitString(std::string_view bits)
	{
		Int_t result = 0;
		const size_t bitsLen = bits.size();

		for(size_t i = 0; i < bitsLen; ++i)
		{
			result |= bits[i] == '1' ? 1 : 0;
			
			if(i < bitsLen - 1)
			{
				result <<= 1;
			}
		}

		return result;
	}

	struct Data
	{
		size_t NumBits = 0;
		std::vector<uint32_t> Values;
	};

	using Data_t = Data;
	using Result_t = int64_t;

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/03/input.txt");

		std::string temp;
		while(file.good())
		{
			file >> temp;

			if(data.NumBits == 0)
			{
				data.NumBits = temp.length();
			}

			data.Values.push_back(FromBitString<uint32_t>(temp));
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 3: Binary Diagnostic";
	}

	static Result_t Part1(const Data_t& data)
	{
		std::vector<size_t> onesCount(data.NumBits, 0);

		// build histogram of set bits
		for(uint32_t value : data.Values)
		{
			for(size_t i = 0; i < data.NumBits; ++i)
			{
				onesCount[data.NumBits - 1 - i] += (value & 1) ? 1 : 0;
				value >>= 1;
			}
		}

		// mask for the bits we care about
		const uint32_t mask = (1 << data.NumBits) - 1;

		uint32_t gamma = 0;
		for(size_t i = 0; i < data.NumBits; ++i)
		{
			gamma |= onesCount[i] * 2 > data.Values.size() ? 1 : 0;
			if(i < data.NumBits - 1)
			{
				gamma <<= 1;
			}
		}

		// epsilon is the inverse of gamma, need to mask off higher bits
		return gamma * (~gamma & mask);
	}

	static Result_t Part2(const Data_t& data)
	{
		// builds a histogram of the number of bits set in each bit position for the given data range
		auto CalculateHistogram = [](auto& container, size_t bitLength, size_t begin, const size_t end)
		{
			std::vector<size_t> hist(bitLength, 0);
			while(begin < end)
			{
				auto value = container[begin];
				for(size_t i = 0; i < bitLength; ++i)
				{
					hist[bitLength - 1 - i] += (value & 1) ? 1 : 0;
					value >>= 1;
				}
				++begin;
			}

			return hist;
		};

		auto GetValue = [&data, &CalculateHistogram](bool flip)
		{
			// need a copy so we can manipulate the list
			std::vector<uint32_t> valueCopy = data.Values;
			
			size_t end = data.Values.size();
			size_t begin = 0;

			for(size_t i = 0; i < data.NumBits; ++i)
			{
				begin = 0;

				// get bit counts for the current list
				const std::vector<size_t> counts = CalculateHistogram(valueCopy, data.NumBits, begin, end);

				const uint32_t mask = 1 << (data.NumBits - 1 - i);
				const uint32_t accept = (flip != (counts[i] * 2 >= end)) ? mask : 0;

				// sort the list so 0 -> end after this will be the filtered values we want
				while(begin < end)
				{
					// if we have the correct bit set or unset this value is on the list
					if((valueCopy[begin] & mask) == accept)
					{
						begin++;
					}
					else
					{
						// otherwise remove it
						std::swap(valueCopy[begin], valueCopy[end - 1]);
						--end;
					}
				}
			}

			// final item is in the first slot
			return valueCopy[0];
		};

		uint32_t oxygen = GetValue(false);
		uint32_t co2 = GetValue(true);

		return oxygen * co2;
	}
};