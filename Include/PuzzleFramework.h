
#pragma once

#include <iostream>
#include <stdint.h>
#include <chrono>
#include <string_view>
#include <concepts>

namespace AoC
{
	template<typename Puzzle_t>
	concept CPuzzle = requires(Puzzle_t puzzle, const typename Puzzle_t::Data_t& d)
	{
		{ Puzzle_t::Setup() } -> std::same_as<typename Puzzle_t::Data_t>;
		{ Puzzle_t::GetName() } -> std::same_as<std::string_view>;
		{ Puzzle_t::Part1(d) } -> std::same_as<typename Puzzle_t::Result_t>;
		{ Puzzle_t::Part2(d) } -> std::same_as<typename Puzzle_t::Result_t>;
	};

	// Helper CRTP struct to enforce CPuzzle constraints on a class if inherited
	template<typename T>
	struct PuzzleBase
	{
		PuzzleBase() { static_assert(CPuzzle<T>); }
	};

	enum class EPuzzlePart
	{
		One = 0,
		Two
	};

	template<CPuzzle Puzzle_t, EPuzzlePart part>
	typename Puzzle_t::Result_t RunPart(const typename Puzzle_t::Data_t& data, const int32_t numIters)
	{
		using namespace std;
		typename Puzzle_t::Result_t result{};

		auto taskStartTime = chrono::steady_clock::now();

		for(int i = 0; i < numIters; ++i)
		{
			if constexpr(part == EPuzzlePart::One)
			{
				result = Puzzle_t::Part1(data);
			}
			else
			{
				result = Puzzle_t::Part2(data);
			}
		}
		
		auto taskEndTime = chrono::steady_clock::now();

		auto fullDuration = (taskEndTime - taskStartTime);
		auto avgTimeMicroSeconds = chrono::duration_cast<chrono::microseconds>(fullDuration / numIters);
		
		cout << "\tSolution: " << static_cast<underlying_type_t<EPuzzlePart>>(part);
		cout << "\n\t\tOutput: " << result;
		cout << "\n\t\tTiming: ";
		
		if(avgTimeMicroSeconds.count() > 1000)
		{
			auto avgTime_fp = chrono::template duration<double, milli>(fullDuration / numIters);
			cout << avgTime_fp.count() << "ms\n\n";
		}
		else
		{
			auto avgTime_fp = chrono::template duration<double, micro>(fullDuration / numIters);
			cout << avgTime_fp.count() << "us\n\n";
		}

		return result;
	}

	template<CPuzzle Puzzle_t>
	void Run(const int32_t timingIters)
	{
		using namespace std;

		cout << "\nPuzzle:  " << Puzzle_t::GetName() << "\n\n";

		auto prepareStartTime = chrono::steady_clock::now();
		
		typename Puzzle_t::Data_t preparedData = Puzzle_t::Setup();
		
		auto prepareEndTime = chrono::steady_clock::now();
		auto prepareDuration = (prepareEndTime - prepareStartTime);

		auto avgPrepTimeMicroSeconds = chrono::duration_cast<chrono::microseconds>(prepareDuration);

		cout << "\tPrepared data in  ";

		if(avgPrepTimeMicroSeconds.count() > 1000)
		{
			auto avgTime_fp = chrono::duration<double, milli>(prepareDuration);
			cout << avgTime_fp.count() << "ms\n\n";
		}
		else
		{
			auto avgTime_fp = chrono::duration<double, micro>(prepareDuration);
			cout << avgTime_fp.count() << "us\n\n";
		}

		RunPart<Puzzle_t, EPuzzlePart::One>(preparedData, timingIters);
		RunPart<Puzzle_t, EPuzzlePart::Two>(preparedData, timingIters);
	}
}