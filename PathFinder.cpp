#include <cmath>
#include <unordered_set>
#include <iostream>

#include "PathFinder.hpp"



Pathfinder::Pathfinder(const Grid& grid)
	: grid_(grid)
	, height_(static_cast<int>(grid.size()))
	, width_(static_cast<int>(grid[0].size()))
{
}

bool Pathfinder::isValid(const Vec& point) const
{
	return point.x >= 0 && point.y >= 0
		&& point.x < width_ && point.y < height_
		&& grid_[point.y][point.x] == 0;
}

double Pathfinder::heuristic(const Vec& a, const Vec& b)
{
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

	return std::sqrt(total);
}

std::vector<Node> Pathfinder::findPath(const Vec& start, const Vec& end)
{
	std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open;
	std::unordered_set<Vec> visited;

	double cost = 0.0;
	open.push({ Vec(start.x, start.y), cost, heuristic(start, end), nullptr });

	while (!open.empty()) {
		Node current = open.top();
		//std::cout << "New node is (" << current.coords.x << ","
		//	<< current.coords.y << ")" << std::endl;
		open.pop();

		if (current.coords == end) { // we arrived at the dest!
			std::vector<Node> path;
			Node* node = new Node(current); // rebuild path
			while (node) {
				path.push_back(*node);
				node = node->parent;
			}
			std::reverse(path.begin(), path.end());
			return path;
		}

		// Check all neighbouring tiles
		for (const auto& dpair : { std::pair{-1,0}, {1,0}, {0,-1}, {0,1} })
		{
			const Vec newPoint = current.coords + dpair;
			{
				if (visited.count(newPoint))
				{
					continue;
				}
				visited.insert(newPoint);
			}

			if (!isValid(newPoint))
			{
				continue;
			}

			double newCost = current.cost + 1.0;
			Node neighbor{ newPoint, newCost, heuristic(newPoint, end), new Node(current) };
			open.push(neighbor);
		}
	}

	return {};
}
