
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

	void Log(const std::string& toLog)
	{
		OutputDebugStringA(toLog.c_str());
		std::cout << toLog << "\n";
	}
}