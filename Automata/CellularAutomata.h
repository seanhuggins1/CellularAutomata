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

struct LineSegment {
	Point p1, p2;
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
	void clearState(int state);
	void replaceState(int targetState, int replacementState);

	void floodFillStack(int startCell, int targetState, int replacementState);
	void floodFillQueue(int startCell, int targetState, int replacementState);

	void fastAccrete(int targetState, int replacementState);

	void generatePoints(int targetState, int replacementState, int k );
	void generateEntryRoad(int state);

	int hashPoint(int x, int y);
	Point unhashPoint(int p);

	void reset();
	void smooth(int targetState, int replacementState);

private:
	int iterations;
	int maxIterations;
	int k;
	int N;
	int w, h;
	int area;
	int cellSize;

	bool isPointInBounds(int x, int y);

	void newRoad(std::vector<int> roadLengths, Point roadPoint, char direction, int roadSpawnRate, int roadDepth);

	std::vector<Point> neighborModel;
	std::vector<Point> smoothModel;

	std::vector<LineSegment> roads;

	std::map<int, CellAgent*> cellMap;

	std::vector<int> mapBorder;

	std::queue<int> cellsToUpdate;
};

