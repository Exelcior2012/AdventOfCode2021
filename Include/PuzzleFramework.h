
#pragma once

#include <iostream>
#include <stdint.h>
#include <chrono>
#include <string_view>
#include <concepts>
#include <type_traits>
#include <thread>

namespace AoC
{
	template<typename Callable, typename... Args>
	using TReturnType = std::invoke_result_t<Callable&&, Args&&...>;

	template<typename Puzzle_t>
	using TPuzzleData = TReturnType<decltype(&Puzzle_t::Setup)>;

	template<typename Puzzle_t>
	using TPuzzleResult1 = TReturnType<decltype(&Puzzle_t::Part1), TPuzzleData<Puzzle_t>&>;

	template<typename Puzzle_t>
	using TPuzzleResult2 = TReturnType<decltype(&Puzzle_t::Part2), TPuzzleData<Puzzle_t>&>;

	template<typename Puzzle_t>
	concept CPuzzle = requires(Puzzle_t puzzle, const TPuzzleData<Puzzle_t>& d)
	{
		{ Puzzle_t::Setup() } -> std::same_as<TPuzzleData<Puzzle_t>>;
		{ Puzzle_t::GetName() } -> std::same_as<std::string_view>;
		{ Puzzle_t::Part1(d) } -> std::same_as<TPuzzleResult1<Puzzle_t>>;
		{ Puzzle_t::Part2(d) } -> std::same_as<TPuzzleResult2<Puzzle_t>>;
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
	void RunPart(const TPuzzleData<Puzzle_t>& data, const int32_t numIters, bool redact)
	{
		using namespace std;
		TPuzzleResult1<Puzzle_t> result1{};
		TPuzzleResult2<Puzzle_t> result2{};
		volatile int loopEnsure = 0;;

		auto taskStartTime = chrono::steady_clock::now();

		for(int i = 0; i < numIters; ++i)
		{
			if constexpr(part == EPuzzlePart::One)
			{
				++loopEnsure;
				result1 = Puzzle_t::Part1(data);
			}
			else
			{
				++loopEnsure;
				result2 = Puzzle_t::Part2(data);
			}
		}
		
		auto taskEndTime = chrono::steady_clock::now();

		auto fullDuration = (taskEndTime - taskStartTime);
		auto avgTimeMicroSeconds = chrono::duration_cast<chrono::microseconds>(fullDuration / numIters);
		
		cout << "\tSolution: " << static_cast<underlying_type_t<EPuzzlePart>>(part);
		cout << "\n\t\tOutput: ";

		if(redact)
		{
			cout << "-----";
		}
		else
		{
			if constexpr(part == EPuzzlePart::One)
			{
				cout << result1;
			}
			else
			{
				cout << result2;
			}
		}

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
	}

	template<CPuzzle Puzzle_t>
	void Run(const int32_t timingIters, bool redact)
	{
		using namespace std;

		cout << "\nPuzzle:  " << Puzzle_t::GetName() << "\n\n";

		auto prepareStartTime = chrono::steady_clock::now();
		
		TPuzzleData<Puzzle_t> preparedData = Puzzle_t::Setup();
		
		auto prepareEndTime = chrono::steady_clock::now();
		auto prepareDuration = (prepareEndTime - prepareStartTime);

		auto avgPrepTimeMicroSeconds = chrono::duration_cast<chrono::microseconds>(prepareDuration);

		if(!redact)
		{
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
		}

		RunPart<Puzzle_t, EPuzzlePart::One>(preparedData, timingIters, redact);
		RunPart<Puzzle_t, EPuzzlePart::Two>(preparedData, timingIters, redact);
	}
}