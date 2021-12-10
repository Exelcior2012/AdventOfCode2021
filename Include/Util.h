
#pragma once

#include <string>
#include <iostream>

#define _X86_
#define NOMINMAX
#include <debugapi.h>

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
		std::cout << toLog << "\n";
	}

	template<typename... Args>
	void Log(Args... args)
	{
		std::stringstream ss;

		(ss << ... << args);
		ss << '\n';

		LogStr(ss.str());
	}
}