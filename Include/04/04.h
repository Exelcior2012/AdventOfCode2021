
#pragma once

#include "PuzzleFramework.h"

#include <fstream>

// Sets a bit in a 32 bit value
constexpr void SetBit(uint32_t& field, uint8_t bit)
{
	field |= (1 << bit);
}

// Gets a bit in a 32 bit value
constexpr bool GetBit(uint32_t field, uint8_t bit)
{
	return (field & (1 << bit)) != 0;
}

struct Day4 : public AoC::PuzzleBase<Day4>
{
	// Collection of possible winning states on a bingo board (no diagonals as per rules)
	// testing "(board & BingoLUT[n]) == BingoLUT[n]" will identify a win
	static constexpr uint32_t BingoLUT[10] =
	{
		0b1111100000000000000000000,
		0b0000011111000000000000000,
		0b0000000000111110000000000,
		0b0000000000000001111100000,
		0b0000000000000000000011111,
		0b1000010000100001000010000,
		0b0100001000010000100001000,
		0b0010000100001000010000100,
		0b0001000010000100001000010,
		0b0000100001000010000100001,
	};

	struct Board
	{
		uint8_t Values[25];
	};

	struct Data
	{
		std::vector<uint8_t> Numbers;
		std::vector<Board> Boards;

		friend std::istream& operator>>(std::istream& in, Data& out)
		{
			int number;
			do 
			{
				in >> number;
				out.Numbers.push_back(static_cast<uint8_t>(number));
			} while (in.get() == ',');
			
			while(in.good())
			{
				out.Boards.push_back({});
				Board& newBoard = out.Boards.back();
				for(int i = 0; i < 25; ++i)
				{
					in >> number;
					newBoard.Values[i] = static_cast<uint8_t>(number);
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

		std::ifstream file("Data/04/input.txt");

		std::string temp;
		if(file.good())
		{
			file >> data;
		}

		return data;
	}

	static std::string_view GetName()
	{
		return "Day 4: Giant Squid";
	}

	static uint32_t GetBoardTotal(const Board& board, uint32_t state)
	{
		uint32_t total = 0;
		for(uint8_t i = 0; i < 25; ++i)
		{
			if(!GetBit(state, i))
			{
				total += board.Values[i];
			}
		}
		return total;
	}

	static Result_t Part1(const Data_t& data)
	{
		std::vector<uint32_t> states(data.Boards.size(), 0);

		// "Calls" a number, marking the corresponding bits in each state
		auto CallNumber = [&data, &states](uint8_t number)
		{
			size_t curr = 0;
			for(const Board& board : data.Boards)
			{
				for(uint8_t i = 0; i < 25; ++i)
				{
					if(board.Values[i] == number)
					{
						SetBit(states[curr], i);
						break;
					}
				}

				++curr;
			}
		};

		// Call out four numbers, no lines possible until after the first 4
		CallNumber(data.Numbers[0]);
		CallNumber(data.Numbers[1]);
		CallNumber(data.Numbers[2]);
		CallNumber(data.Numbers[3]);
		
		int winner = -1;
		uint8_t lastCall = 0;
		const int numToCall = static_cast<int>(data.Numbers.size());
		for(int i = 4; i < numToCall; ++i)
		{
			lastCall = data.Numbers[i];
			CallNumber(lastCall);

			// Detect winning boards after this number was called
			size_t curr = 0;
			for(const uint32_t state : states)
			{
				for(int j = 0; j < 10; ++j)
				{
					if((state & BingoLUT[j]) == BingoLUT[j])
					{
						winner = static_cast<int>(curr);

						// To avoid an extra branch in the enclosing for loop, 
						// and then another in the enclosing while loop - just
						// jump straight out of the inner branch when we detect
						// the first win
						goto LABEL_WON;

						break;
					}
				}

				++curr;
			}
		}
LABEL_WON:
		
		return GetBoardTotal(data.Boards[winner], states[winner]) * lastCall;
	}

	static Result_t Part2(const Data_t& data)
	{
		// bitfields for states and a 0...n list of indices as we're going
		// to be filtering the list down as wins are encountered
		std::vector<uint32_t> states(data.Boards.size(), 0);
		std::vector<int> indices(data.Boards.size());
		std::iota(std::begin(indices), std::end(indices), 0);

		// end of the run of active board (active meaning not won)
		size_t end = states.size() - 1;

		// "Calls" a number, marking the corresponding bits in each state
		auto CallNumber = [&data, &states, &indices](uint8_t number, size_t end)
		{
			for(size_t i = 0; i < end + 1; i++)
			{
				const size_t boardIndex = indices[i];
				const Board& board = data.Boards[boardIndex];
				for(uint8_t j = 0; j < 25; ++j)
				{
					if(board.Values[j] == number)
					{
						SetBit(states[i], j);
						break;
					}
				}
			}
		};

		// Call out four numbers, no lines possible until after the first 4
		CallNumber(data.Numbers[0], end);
		CallNumber(data.Numbers[1], end);
		CallNumber(data.Numbers[2], end);
		CallNumber(data.Numbers[3], end);

		uint8_t lastCall = 0;
		const int numToCall = static_cast<int>(data.Numbers.size());
		for(int i = 4; i < numToCall; ++i)
		{
			lastCall = data.Numbers[i];
			CallNumber(lastCall, end);

			// Detect winning boards after this number was called
			size_t curr = 0;
			while(curr <= end)
			{
				const uint32_t state = states[curr];

				for(int j = 0; j < 10; ++j)
				{
					if((state & BingoLUT[j]) == BingoLUT[j])
					{
						// To avoid an extra branch in the enclosing for loop, 
						// and then another in the enclosing while loop - just
						// jump straight out of the inner branch when we detect
						// the final win
						if(end == 0)
						{
							goto LABEL_WON;
						}

						// if we're here then there's a win, but it isn't the
						// final win - so evict that board past the end of the
						// list, so 0-end is the list of remaining boards
						std::swap(indices[curr], indices[end]);
						std::swap(states[curr--], states[end--]);

						// Only one win is required on a board
						break;
					}
				}
				curr++;
			}
		}
LABEL_WON:

		return GetBoardTotal(data.Boards[indices[end]], states[end]) * lastCall;
	}
};
