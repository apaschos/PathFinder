#pragma once

#include <cassert>
#include <vector>


//#define ZCURVE 1
#ifdef ZCURVE
namespace
{
	// Interleave bits to create Z-order index (32-bit version)
	inline uint32_t z_encode(uint16_t x, uint16_t y) {
		uint32_t z = 0;
		for (int i = 0; i < 16; i++) {
			z |= (x & 1U << i) << i | (y & 1U << i) << (i + 1);
		}
		return z;
	}

	// Decode Z-order to original coordinates
	inline void z_decode(uint32_t z, uint16_t& x, uint16_t& y) {
		x = 0; y = 0;
		for (int i = 0; i < 16; i++) {
			x |= (z >> (2 * i) & 1) << i;
			y |= (z >> (2 * i + 1) & 1) << i;
		}
	}
}
#endif

class Grid
{
public:
	Grid(int rows, int cols)
		: rows_(rows)
		, cols_(cols)
		, map_(rows* cols)
	{
	}

	int at(int row, int col) const
	{
		assert(row >= 0 && row < rows_);
		assert(col >= 0 && col < cols_);

#ifdef ZCURVE
		const int zIndex = z_encode(row, col);
		return map_[zIndex];
#else
		return map_[row * cols_ + col];
#endif
	}

	int& at(int row, int col)
	{
		assert(row >= 0 && row < rows_);
		assert(col >= 0 && col < cols_);

#ifdef ZCURVE
		const int zIndex = z_encode(row, col);
		return map_[zIndex];
#else
		return map_[row * cols_ + col];
#endif
	}

	int rows() const
	{
		return rows_;
	}

	int cols() const
	{
		return cols_;
	}

private:
	int rows_;
	int cols_;
	std::vector<int> map_;
};