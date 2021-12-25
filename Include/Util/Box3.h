
#pragma once

#include "Util/Vec3.h"

struct Box3Di
{
	Vec3i Min = Vec3i::Limit_Max;
	Vec3i Max = Vec3i::Limit_Min;

	Box3Di() = default;
	Box3Di(const Vec3i& min, const Vec3i& max)
		: Min(min)
		, Max(max)
	{}

	bool IsValid() const
	{
		return Max > Min;
	}

	void GrowToInclude(const Vec3i& vec)
	{
		Min.X = std::min(Min.X, vec.X);
		Min.Y = std::min(Min.Y, vec.Y);
		Min.Z = std::min(Min.Z, vec.Z);

		Max.X = std::max(Max.X, vec.X);
		Max.Y = std::max(Max.Y, vec.Y);
		Max.Z = std::max(Max.Z, vec.Z);
	}

	Vec3i Extent() const
	{
		return Max - Min;
	}

	int64_t Volume() const
	{
		return Extent().Product();
	}

	Box3Di GetIntersection(const Box3Di& other) const
	{
		Box3Di newBox;
		newBox.Min.X = std::max(Min.X, other.Min.X);
		newBox.Min.Y = std::max(Min.Y, other.Min.Y);
		newBox.Min.Z = std::max(Min.Z, other.Min.Z);
		
		newBox.Max.X = std::min(Max.X, other.Max.X);
		newBox.Max.Y = std::min(Max.Y, other.Max.Y);
		newBox.Max.Z = std::min(Max.Z, other.Max.Z);
		return newBox;
	}

	bool Intersects(const Box3Di& other) const
	{
		return GetIntersection(other).IsValid();
	}
};