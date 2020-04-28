#pragma once

#include <SDL.h>

#include "CellAgent.h"

#include <stdlib.h>
#include <map>
#include <queue>

class CellularAutomata
{
public:
	CellularAutomata(int iterations, int k, int w, int h, int cellSize);

	void update();

	void render(SDL_Renderer* renderer);

	void reset();

private:
	int iterations;
	int k;
	int w, h;
	int area;
	int cellSize;

	std::map<int, CellAgent*> cellMap;

	std::queue<int> cellsToUpdate;
};

