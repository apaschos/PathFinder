#pragma once

#include <cassert>
#include <vector>

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

		return map_[row * cols_ + col];
	}

	int& at(int row, int col)
	{
		assert(row >= 0 && row < rows_);
		assert(col >= 0 && col < cols_);

		return map_[row * cols_ + col];
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