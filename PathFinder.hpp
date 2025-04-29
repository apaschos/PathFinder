#pragma once

#include <queue>

#include "Node.hpp"
#include "Grid.hpp"
#include "ObjectPool.hpp"


class Pathfinder {
public:
	Pathfinder(const Grid& grid);
	std::vector<Node*> findPath(const Vec& start, const Vec& end);

	void resetPool();

private:
	const Grid& grid_;
	ObjectPool<Node> nodePool;

	bool isValid(const Vec& point) const;
	bool isObstacle(const Vec& point) const;
	static double heuristic(const Vec& a, const Vec& b);
};
