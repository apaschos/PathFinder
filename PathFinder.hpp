#pragma once

#include <queue>
#include <unordered_set>

#include "Node.hpp"
#include "Grid.hpp"
#include "ObjectPool.hpp"


struct NodeGreater {
	bool operator()(const Node* lhs, const Node* rhs) const {
		return (lhs->cost + lhs->heuristic) > (rhs->cost + rhs->heuristic);
	}
};

typedef std::priority_queue<Node*, std::vector<Node*>, NodeGreater> NodeQueue;
typedef std::unordered_set<Vec> VecHash;

class Pathfinder {
public:
	Pathfinder(const Grid& grid);
	std::vector<Node*> findPath(const Vec& start, const Vec& end);

	void resetPool();

private:
	const Grid& grid_;
	ObjectPool<Node> nodePool_;
	NodeQueue potentialNodes_;
	VecHash visitedNodes_;

	void checkNeighbour(Node* refNode, const Vec& newPoint, const Vec& dstPoint);
	bool isValid(const Vec& point) const;
	bool isObstacle(const Vec& point) const;
	static double heuristic(const Vec& a, const Vec& b);
};
