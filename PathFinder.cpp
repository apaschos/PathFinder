#include <cmath>
#include <unordered_set>
#include <iostream>

#include "PathFinder.hpp"


constexpr size_t POOL_SIZE = 1000000;


Pathfinder::Pathfinder(const Grid& grid)
	: grid_(grid)
	, nodePool(POOL_SIZE)
{
}

bool Pathfinder::isObstacle(const Vec& point) const
{
	return grid_.at(point.x, point.y) == 1;
}

bool Pathfinder::isValid(const Vec& point) const
{
	return point.x >= 0 && point.y >= 0
		&& point.x < grid_.rows() && point.y < grid_.cols()
		&& !isObstacle(point);
}

double Pathfinder::heuristic(const Vec& a, const Vec& b)
{
	// Use squared euclidean distance for now.
	// No need to sqrt since we just compare.
	double total = 0.0;
	if (a.length() != b.length())
	{
		return -1.0;
	}

	for (int i = 0; i < a.length(); ++i)
	{
		double d = a.data[i] - b.data[i];
		total += d * d;
	}

	return total;
}

struct NodeGreater {
	bool operator()(const Node* lhs, const Node* rhs) const {
		return *lhs > *rhs;
	}
};

std::vector<Node*> Pathfinder::findPath(const Vec& start, const Vec& end)
{
	std::priority_queue<Node*, std::vector<Node*>, NodeGreater> open;
	std::unordered_set<Vec> visited;

	double cost = 0.0;
	Node* newNode = nodePool.acquire();
	*newNode = { Vec(start.x, start.y), cost, heuristic(start, end), nullptr };
	open.push(newNode);

	while (!open.empty()) {
		Node* current = open.top();
		//std::cout << "New node is (" << current.coords.x << ","
		//	<< current.coords.y << ")" << std::endl;
		open.pop();

		if (current->coords == end) { // we arrived at the dest!
			// rebuild path
			std::vector<Node*> path;
			path.reserve(open.size());
			Node* node = current;
			while (node) {
				path.push_back(node);
				node = node->parent;
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		// Check all neighbouring tiles

		for (const auto& dpair : { std::pair
			{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }, //prioritize diagonal moves
			{ 1,0 }, { -1,0 }, { 0,-1 }, { 0,1 } }
			)
		{
			const Vec newPoint = current->coords + dpair;

			if (!isValid(newPoint))
			{
				continue;
			}

			if (visited.count(newPoint))
			{
				continue;
			}
			visited.insert(newPoint);

			double newCost = current->cost + 1.0;
			Node* neighbour = nodePool.acquire();
			*neighbour = { newPoint, newCost, heuristic(newPoint, end), current };
			open.push(neighbour);
		}
	}

	return {};
}

void Pathfinder::resetPool() {
	nodePool.releaseAll();
}
