
#pragma once

struct Vec3i
{
	Vec3i() = default;

	Vec3i(int v)
		: X(v)
		, Y(v)
		, Z(v)
	{}

	Vec3i(int x, int y, int z)
		: X(x)
		, Y(y)
		, Z(z)
	{}

	int X;
	int Y;
	int Z;

	static Vec3i Limit_Max;
	static Vec3i Limit_Min;

	int64_t Product() const
	{
		return static_cast<int64_t>(X) * static_cast<int64_t>(Y) * static_cast<int64_t>(Z);
	}

	Vec3i& operator -=(const Vec3i& other)
	{
		X -= other.X;
		Y -= other.Y;
		Z -= other.Z;
		return *this;
	}

	Vec3i& operator +=(const Vec3i& other)
	{
		X += other.X;
		Y += other.Y;
		Z += other.Z;
		return *this;
	}

	friend std::strong_ordering operator<=>(const Vec3i& a, const Vec3i& b) = default;

	friend Vec3i operator-(const Vec3i& a, const Vec3i& b)
	{
		Vec3i temp = a;
		temp -= b;
		return temp;
	}

	friend Vec3i operator+(const Vec3i& a, const Vec3i& b)
	{
		Vec3i temp = a;
		temp += b;
		return temp;
	}

	friend std::istream& operator>>(std::istream& in, Vec3i& out)
	{
		char commaDiscard;
		in >> out.X >> commaDiscard >> out.Y >> commaDiscard >> out.Z;
		return in;
	}

	friend std::ostream& operator<<(std::ostream& out, const Vec3i& in)
	{
		out << in.X << ',' << in.Y << ',' << in.Z;
		return out;
	}
};

Vec3i Vec3i::Limit_Max = Vec3i(std::numeric_limits<int>::max());
Vec3i Vec3i::Limit_Min = Vec3i(-std::numeric_limits<int>::lowest());