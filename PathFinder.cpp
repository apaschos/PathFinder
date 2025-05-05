#include <algorithm>
#include <cmath>
#include <iostream>

#include "PathFinder.hpp"


constexpr size_t POOL_SIZE = 1000000;


Pathfinder::Pathfinder(const Grid& grid)
	: grid_(grid)
	, nodePool_(POOL_SIZE)
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

void Pathfinder::checkNeighbour(Node* refNode, const Vec& newPoint, const Vec& dstPoint)
{
	if (!isValid(newPoint))
	{
		return;
	}

	if (visitedNodes_.contains(newPoint))
	{
		return;
	}
	visitedNodes_.insert(newPoint);

	double newCost = refNode->cost + 1.0;
	Node* neighbour = nodePool_.acquire();
	*neighbour = { newPoint, newCost, heuristic(newPoint, dstPoint), refNode };
	potentialNodes_.push(neighbour);
}

std::vector<Node*> Pathfinder::findPath(const Vec& start, const Vec& end)
{
	potentialNodes_ = NodeQueue();
	visitedNodes_.clear();

	double cost = 0.0;
	Node* newNode = nodePool_.acquire();
	*newNode = { Vec(start.x, start.y), cost, heuristic(start, end), nullptr };
	potentialNodes_.push(newNode);

	while (!potentialNodes_.empty()) {
		Node* current = potentialNodes_.top();
		potentialNodes_.pop();
		//std::cout << "New node is (" << current.coords.x << ","
		//	<< current.coords.y << ")" << std::endl;

		if (current->coords == end) { // We arrived at the dest!
			// Rebuild path
			std::vector<Node*> path(potentialNodes_.size());
			Node* node = current;
			int i = static_cast<int>(path.size()) - 1;
			while (node) {
				path[i--] = node;
				node = node->parent;
			}
			path = std::vector<Node*>(path.begin() + i + 1, path.end());
			return path;
		}

		// Check all neighbouring tiles
		for (const auto& dpair : { std::pair
			{ 1, 1 }, { 1, -1 }, { -1, 1 }, { -1, -1 }, //prioritize diagonal moves
			{ 1,0 }, { -1,0 }, { 0,-1 }, { 0,1 } }
			)
		{
			const Vec newNode = current->coords + dpair;
			checkNeighbour(current, newNode, end);
		}
	}

	return {};
}

void Pathfinder::resetPool() {
	nodePool_.releaseAll();
}
