#pragma once

// Force std::min/max instead for type consistency
#include <algorithm>

using std::min;
using std::max;

template <typename T>
constexpr T clamp(T value, T low, T high)
{
	return min(max(low, value), high);
}
