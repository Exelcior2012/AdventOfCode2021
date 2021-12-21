
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

	template<typename T>
	T WrapRange(T min, T max, T value)
	{
		static_assert(std::is_integral_v<T>);
		return min + (value - min) % (max + 1 - min);
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

	template<size_t numBits>
	struct BitStorageBase{};
	template <> struct BitStorageBase<1> { using type = uint8_t; };
	template <> struct BitStorageBase<2> { using type = uint8_t; };
	template <> struct BitStorageBase<4> { using type = uint8_t; };
	template <> struct BitStorageBase<8> { using type = uint8_t; };
	template <> struct BitStorageBase<16> { using type = uint16_t; };
	template <> struct BitStorageBase<32> { using type = uint32_t; };
	template <> struct BitStorageBase<64> { using type = uint64_t; };

	template<size_t numBits>
	struct BitStorage : public BitStorageBase<std::bit_ceil(numBits)> {};

}