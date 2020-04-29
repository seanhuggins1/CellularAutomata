#pragma once

#include <SDL.h>

#include "CellAgent.h"

#include <stdlib.h>
#include <map>
#include <queue>
#include <vector>

struct NeighborOffset {
	int x;
	int y;
};

class CellularAutomata
{
public:
	CellularAutomata(int iterations, int k, int w, int h, int cellSize);

	void update();

	void render(SDL_Renderer* renderer);

	int getNeighborState(int index, int xoffset, int yoffset);

	void reset();

private:
	int iterations;
	int maxIterations;
	int k;
	int w, h;
	int area;
	int cellSize;

	std::vector<NeighborOffset> neighborModel;

	std::map<int, CellAgent*> cellMap;

	std::queue<int> cellsToUpdate;
};

