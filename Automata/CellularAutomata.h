#pragma once

#include <SDL.h>

#include "CellAgent.h"

#include <stdlib.h>
#include <map>
#include <queue>
#include <vector>

struct Point {
	int x;
	int y;
};

class CellularAutomata
{
public:
	CellularAutomata(int iterations, int k, int N, int w, int h, int cellSize);

	void update();

	void render(SDL_Renderer* renderer);

	int getNeighborState(int index, int xoffset, int yoffset);

	void initBorder();
	void drawBorder();

	void generateRoads();

	int hashPoint(int x, int y);
	Point unhashPoint(int p);

	void reset();
	void smooth();

private:
	int iterations;
	int maxIterations;
	int k;
	int N;
	int w, h;
	int area;
	int cellSize;

	std::vector<Point> neighborModel;
	std::vector<Point> smoothModel;

	std::map<int, CellAgent*> cellMap;

	std::vector<int> mapBorder;

	std::queue<int> cellsToUpdate;
};

