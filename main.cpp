#include <chrono>
#include <iomanip>
#include <iostream>
#include <random>

//#define USE_GRAPHICS

#ifdef USE_GRAPHICS
#include <SFML/Graphics.hpp>
#endif
#include "PathFinder.hpp"


constexpr int CELL_SIZE = 20;
constexpr int GRID_SIZE = 50;
constexpr int GRID_WIDTH = GRID_SIZE;
constexpr int GRID_HEIGHT = GRID_SIZE;
constexpr int MAX_RUNS = 10000;


// Initialize grid.
// Add some obstacles.
void init_grid(Grid& grid)
{
	for (int i = 0; i < grid.rows(); ++i)
	{
		for (int j = 0; j < grid.cols(); ++j)
		{
			grid.at(i, j) = 0;// distrib(gen);
		}
	}

	//Add lines of obstacles
	for (int i = 1; i < grid.rows(); i += 4)
	{
		for (int j = 0; j < grid.cols() - 1; ++j)
		{
			grid.at(i, j) = 1;
		}
	}
	for (int j = 0; j < grid.cols() - 1; ++j)
	{
		grid.at(grid.rows() - 1, j) = 1;
	}
	for (int i = 3; i < grid.rows() - 1; i += 4)
	{
		for (int j = 1; j < grid.cols(); ++j)
		{
			grid.at(i, j) = 1;
		}
	}
	// Remove some obstacles randomly. Or semi-randomly
	{
#define FIXED_SEED 25U
#ifdef FIXED_SEED
		std::mt19937 gen(FIXED_SEED);
#else
		std::random_device rd;
		std::mt19937 gen(rd());
#endif
		std::uniform_int_distribution<> distrib(2, GRID_SIZE - 2);

		constexpr int NUM_OBSTACLES = 25;
		for (int i = 0; i < NUM_OBSTACLES; ++i)
		{
			grid.at(distrib(gen), distrib(gen)) = 0;
		}
	}
}

#ifdef USE_GRAPHICS
void drawGrid(sf::RenderWindow& window, const Grid& grid, const std::vector<Node*>& path)
{
	sf::RectangleShape cell(sf::Vector2f(static_cast<float>(CELL_SIZE - 2), static_cast<float>(CELL_SIZE - 2)));

	for (int x = 0; x < grid.rows(); ++x) {
		for (int y = 0; y < grid.cols(); ++y) {
			if (grid.at(x, y) == 1)
				cell.setFillColor(sf::Color::Black); // wall
			else
				cell.setFillColor(sf::Color::White); // empty

			//cell.setPosition(sf::Vector2f(static_cast<float>(x * CELL_SIZE), static_cast<float>(y * CELL_SIZE)));
			cell.setPosition(sf::Vector2f(static_cast<float>(y * CELL_SIZE), static_cast<float>(x * CELL_SIZE)));
			window.draw(cell);
		}
	}

	// Draw path
	for (const auto& node : path) {
		cell.setFillColor(sf::Color::Green);
		//cell.setPosition(sf::Vector2f(static_cast<float>(node->coords.x * CELL_SIZE), static_cast<float>(node->coords.y * CELL_SIZE)));
		cell.setPosition(sf::Vector2f(static_cast<float>(node->coords.y * CELL_SIZE), static_cast<float>(node->coords.x * CELL_SIZE)));
		window.draw(cell);
	}
}
#endif


int main()
{
	// Generate a map with obstacles.
	Grid grid(GRID_HEIGHT, GRID_WIDTH);

	//std::cout << "Setting up the map.." << std::endl;
	init_grid(grid);
	//std::cout << "Map ready.." << std::endl;

	// Run the path finding algorithm.
#ifdef USE_GRAPHICS
	Pathfinder pathfinder(grid);
	auto start = std::chrono::high_resolution_clock::now();
	auto path = pathfinder.findPath(Vec(0, 0), Vec(grid.rows() - 1, grid.cols() - 1));
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> elapsed = end - start;
	std::cout << std::setprecision(4) << std::fixed << "Time: " << elapsed.count() << " secs\n";

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
	std::chrono::duration<double> elapsed(0);
	std::chrono::high_resolution_clock::time_point start, end;
	Pathfinder pathfinder(grid);
	for (int run = 0; run < MAX_RUNS; ++run)
	{
		start = std::chrono::high_resolution_clock::now();
		pathfinder.findPath(Vec(0, 0), Vec(GRID_HEIGHT - 1, GRID_WIDTH - 1));
		end = std::chrono::high_resolution_clock::now();
		elapsed += end - start;
		pathfinder.resetPool();
	}
	std::cout << std::setprecision(4) << std::fixed << "Time: " << elapsed.count() / MAX_RUNS * 1000000 << " microsecs\n";
#endif

	return 0;
}
