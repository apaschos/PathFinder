#include <iostream>
#include <random>
#include <chrono>

#include <SFML/Graphics.hpp>
#include "PathFinder.hpp"


constexpr int CELL_SIZE = 50;
constexpr int GRID_SIZE = 50;
constexpr int GRID_WIDTH = GRID_SIZE;
constexpr int GRID_HEIGHT = GRID_SIZE;

void drawGrid(sf::RenderWindow& window, const Grid& grid, const std::vector<Node>& path) {
	sf::RectangleShape cell(sf::Vector2f(static_cast<float>(CELL_SIZE - 2), static_cast<float>(CELL_SIZE - 2)));

	for (int y = 0; y < GRID_HEIGHT; ++y) {
		for (int x = 0; x < GRID_WIDTH; ++x) {
			if (grid[y][x] == 1)
				cell.setFillColor(sf::Color::Black); // wall
			else
				cell.setFillColor(sf::Color::White); // empty

			cell.setPosition(sf::Vector2f(static_cast<float>(x * CELL_SIZE), static_cast<float>(y * CELL_SIZE)));
			window.draw(cell);
		}
	}

	// Draw path
	for (const auto& n : path) {
		cell.setFillColor(sf::Color::Green);
		cell.setPosition(sf::Vector2f(static_cast<float>(n.coords.x * CELL_SIZE), static_cast<float>(n.coords.y * CELL_SIZE)));
		window.draw(cell);
	}
}

int main()
{
	// Generate a map with obstacles.
	std::cout << "Setting up the map.." << std::endl;
	Grid grid(GRID_HEIGHT);
	for (int i = 0; i < grid.size(); ++i)
	{
		grid[i] = std::vector<int>(GRID_WIDTH);
		for (int j = 0; j < grid[i].size(); ++j)
		{
			grid[i][j] = 0;// distrib(gen);
		}
	}

	//Add lines of obstacles
	for (int i = 1; i < grid.size(); i += 4)
	{
		for (int j = 0; j < grid[1].size() - 1; ++j)
		{
			grid[i][j] = 1;
		}
	}
	for (int j = 0; j < grid[1].size() - 1; ++j)
	{
		grid[grid.size() - 1][j] = 1;
	}
	for (int i = 3; i < grid.size() - 1; i += 4)
	{
		for (int j = 1; j < grid[1].size(); ++j)
		{
			grid[i][j] = 1;
		}
	}
	// Remove some obstacles randomly.
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> distrib(2, GRID_SIZE - 2);

		constexpr int NUM_OBSTACLES = 25;
		for (int i = 0; i < NUM_OBSTACLES; ++i)
		{
			grid[distrib(gen)][distrib(gen)] = 0;
		}
	}
	std::cout << "Map ready.." << std::endl;

#if 0
	Pathfinder pathfinder(grid);
	auto start = std::chrono::high_resolution_clock::now();
	auto path = pathfinder.findPath(Vec(0, 0), Vec(GRID_HEIGHT - 1, GRID_WIDTH - 1));
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << "Time: " << elapsed.count() << " seconds\n";

	sf::RenderWindow window(sf::VideoMode(GRID_WIDTH * CELL_SIZE, GRID_HEIGHT * CELL_SIZE), "Pathfinding");

	while (window.isOpen()) {
		sf::Event event;
		while (window.pollEvent(event)) {
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear(sf::Color::White);
		drawGrid(window, grid, path);
		window.display();
	}
#else
	constexpr int MAX_RUNS = 10000;
	std::chrono::duration<double> elapsed(0);
	std::chrono::steady_clock::time_point start, end;
	Pathfinder pathfinder(grid);
	for (int run = 0; run < MAX_RUNS; ++run)
	{
		start = std::chrono::high_resolution_clock::now();
		pathfinder.findPath(Vec(0, 0), Vec(GRID_HEIGHT - 1, GRID_WIDTH - 1));
		end = std::chrono::high_resolution_clock::now();
		elapsed += end - start;
	}
	std::cout << std::setprecision(4) << std::fixed << "Time: " << elapsed.count() / MAX_RUNS * 1000000 << " microsecs\n";
#endif

	return 0;
}
