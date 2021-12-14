
#pragma once

#include <string>
#include <iostream>

//#define _X86_
//#define _AMD64_
//#define NOMINMAX
//#include <debugapi.h>

#define NOMINMAX
#include <Windows.h>

namespace Util
{
	template<typename T>
	T Abs(T&& value)
	{
		return value < 0 ? -value : value;
	}

	template<typename T>
	T TriangularNumber(T index)
	{
		return (index * (index + 1)) / 2;
	}

	template<typename T>
	int CountBits(T in)
	{
		int count;
		for(count = 0; in; count++)
		{
			in &= in - 1;
		}
		return count;
	}

	void LogStr(const std::string& toLog)
	{
		OutputDebugStringA(toLog.c_str());
		std::cout << toLog;
	}

	template<bool WithNewLine = true, typename... Args>
	void Log(Args... args)
	{
		std::stringstream ss;

		(ss << ... << args);

		if constexpr(WithNewLine)
		{
			ss << "\n";
		}

		LogStr(ss.str());
	}

	// 64-bit FNV-1a hash for bytes
	// see: https://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function
	uint64_t Fnv1aHash(uint8_t* data, size_t size)
	{
		static constexpr uint64_t offsetBasis = 0xcbf29ce484222325;
		static constexpr uint64_t fnvPrime = 0x100000001b3;

		uint64_t hash = offsetBasis;

		for(size_t i = 0; i < size; i++)
		{
			hash ^= data[i];
			hash *= fnvPrime;
		}

		return hash;
	}
}