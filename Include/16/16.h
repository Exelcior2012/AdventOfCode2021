
#pragma once

#include "Util.h"
#include "PuzzleFramework.h"

#include <fstream>
#include <queue>

struct Day16 : public AoC::PuzzleBase<Day16>
{
	static constexpr uint8_t Nibbles[] = {
		0b0000, // 0
		0b0001, // 1
		0b0010, // 2
		0b0011, // 3
		0b0100, // 4
		0b0101, // 5
		0b0110, // 6
		0b0111, // 7
		0b1000, // 8
		0b1001, // 9
		0b1010, // A
		0b1011, // B
		0b1100, // C
		0b1101, // D
		0b1110, // E
		0b1111, // F	
	};

	static uint8_t DecodeNibble(char c)
	{
		if(c > '9')
		{
			c = (char)toupper(c);
			return Nibbles[10 + (c - 'A')];
		}
		else
		{
			return Nibbles[c - '0'];
		}
	}

	struct Biterator
	{
		Biterator() = delete;
		Biterator(const uint8_t* ptr, size_t size)
			: m_basePtr(ptr)
			, m_size(size)
			, m_at(0)
		{}

		uint8_t operator[](const size_t i) const
		{
			size_t byte = (m_at + i) / 8;
			uint8_t bit = (m_at + i) % 8;
			return (m_basePtr[byte] & (0b10000000 >> bit)) >> (8 - bit - 1);
		}

		template<size_t Num>
		typename Util::BitStorage<Num>::type Consume()
		{
			using Out_t = Util::BitStorage<Num>::type;
			Out_t result = 0;
			for(int i = 0; i < Num; ++i)
			{
				result |= (*this)[i];
				if(i < Num - 1) result <<= 1;
			}
			m_at += Num;
			return result;
		}

		template<size_t Num>
		Util::BitStorage<Num>::type Peek()
		{
			using Out_t = Util::BitStorage<Num>::type;
			Out_t result = 0;
			for(int i = 0; i < Num; ++i)
			{
				result |= (*this)[i] << i;
			}
			return result;
		}

		template<size_t align>
		bool IsAligned() const
		{
			return m_at % align == 0;
		}

		void SkipToNextNibble()
		{
			m_at += 4 - (m_at % 4);
		}

		void SkipToNextNibbleUnaligned()
		{
			if(!IsAligned<4>())
			{
				m_at += 4 - (m_at % 4);
			}
		}

		size_t Tell() const
		{
			return m_at;
		}

	private:
		const uint8_t* m_basePtr;
		size_t m_size;
		size_t m_at;
	};

	struct Data
	{
		std::vector<uint8_t> Bytes;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			using namespace std;

			string line;

			while(in.good())
			{
				getline(in, line);

				const int length = line.length();
				for(int i = 0; i < length; i+=2)
				{
					out.Bytes.push_back((DecodeNibble(line[i]) << 4) | DecodeNibble(line[i + 1]));
				}
			}

			return in;
		}
	};

	static Data Setup()
	{
		Data data;

		std::ifstream file("Data/16/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 16: Packet Decoder";
	}

	static bool IsOperator(uint8_t type)
	{
		return type != 4;
	}

	static size_t SkipPacket(Biterator& bits, uint32_t& versionAccumulator)
	{
		size_t beginSize = bits.Tell();

		versionAccumulator += bits.Consume<3>();
		uint8_t type = bits.Consume<3>();

		if(!IsOperator(type))
		{
			// Literal
			while(bits.Consume<5>() >> 4 == 1) {}
		}
		else
		{
			// Operator
			const bool isPacketCount = bits.Consume<1>() == 1;

			if(isPacketCount)
			{
				uint16_t numPackets = bits.Consume<11>();
				for(uint16_t i = 0; i < numPackets; ++i)
				{
					SkipPacket(bits, versionAccumulator);
				}
			}
			else
			{
				uint16_t numBits = bits.Consume<15>();
				while(numBits > 0)
				{
					const auto packetBits = static_cast<uint16_t>(SkipPacket(bits, versionAccumulator));
					numBits -= packetBits;
				}
			}
		}

		return bits.Tell() - beginSize;
	}

	static uint64_t InitialValue(uint8_t op)
	{
		switch(op)
		{
			case 1: return 1;
			case 2: return std::numeric_limits<uint64_t>::max();
			default:
				break;
		}
		return 0;
	}

	static bool IsArithOp(uint8_t op)
	{
		return op < 4;
	}

	static void DoArithOp(uint8_t op, uint64_t& value, uint64_t operand)
	{
		switch(op)
		{
			case 0: value += operand; break;
			case 1: value *= operand; break;
			case 2: value = std::min(value, operand); break;
			case 3: value = std::max(value, operand); break;
			default:
				break;
		}
	}

	static uint64_t ProcessPacket(Biterator& bits)
	{
		bits.Consume<3>(); // discard version
		uint8_t type = bits.Consume<3>();

		if(!IsOperator(type))
		{
			// Literal
			uint64_t value = 0;
			while(true)
			{
				uint8_t nibble = bits.Consume<5>();
				bool bLast = (nibble & 0b10000) == 0;
				nibble &= 0b01111;
				value += nibble;

				if(!bLast)
				{
					value <<= 4;
				}
				else
				{
					break;
				}
			}
			return value;
		}
		else
		{
			// Operator
			uint64_t value = InitialValue(type);
			const bool isPacketCount = bits.Consume<1>() == 1;

			if(isPacketCount)
			{
				uint16_t numPackets = bits.Consume<11>();
				if(IsArithOp(type))
				{
					for(uint16_t i = 0; i < numPackets; ++i)
					{
						DoArithOp(type, value, ProcessPacket(bits));
					}
				}
				else
				{
					value = DoLogicOp(type, bits);
				}
			}
			else
			{
				uint16_t numBits = bits.Consume<15>();

				if(IsArithOp(type))
				{
					while(numBits > 0)
					{
						size_t before = bits.Tell();
						DoArithOp(type, value, ProcessPacket(bits));
						numBits -= static_cast<uint16_t>(bits.Tell() - before);
					}
				}
				else
				{
					value = DoLogicOp(type, bits);
				}
			}

			return value;
		}
	}

	static uint8_t DoLogicOp(uint8_t op, Biterator& bits)
	{
		uint64_t a = ProcessPacket(bits);
		uint64_t b = ProcessPacket(bits);
		
		switch(op)
		{
			case 5: return a > b ? 1 : 0;
			case 6: return a < b ? 1 : 0;
			case 7: return a == b ? 1 : 0;
			default:
				break;
		}

		return 0;
	}

	static int64_t Part1(const Data& data)
	{
		Biterator bits(data.Bytes.data(), data.Bytes.size());
		uint32_t versions = 0;
		SkipPacket(bits, versions);

		return versions;
	}

	static int64_t Part2(const Data& data)
	{
		Biterator bits(data.Bytes.data(), data.Bytes.size());
		return ProcessPacket(bits);
	}
};
