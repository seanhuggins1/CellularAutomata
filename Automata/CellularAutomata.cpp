#include "CellularAutomata.h"
#include <ctime>
#include <iostream>
#include <stdio.h>

#include <queue>
#include <stdlib.h>
#include <vector>

int CellularAutomata::hashPoint(int x, int y) {
	return x + y * this->w;
}

Point CellularAutomata::unhashPoint(int p)
{
	return Point({p % w, (int)floor(p / w)});
}

CellularAutomata::CellularAutomata(int maxIterations, int k, int N, int w, int h, int cellSize)
{
	this->iterations = 0;
	this->maxIterations = maxIterations;
	this->k = k;
	this->N = N;
	this->w = w;
	this->h = h;
	this->area = (this->w * this->h);
	this->cellSize = cellSize;

	srand(time(NULL));

	//add all cells to updatenext
	for (int i = 0; i < area; i++) {
		this->cellMap[i] = new CellAgent(0, (i % w) * cellSize, floor(i/w) * cellSize, cellSize);
		this->cellsToUpdate.push(i);
	}

	//randomly flip k cells
	//for (int i = 0; i < k; i++) {
	//	this->cellMap[hashPoint(rand() % w / 2 + (w / 4), rand() % h / 2 + (h / 4))]->setState(1);
	//}

	//initBorder();

	

	//this->neighborModel.push_back(Point({ -2,-2 }));
	//this->neighborModel.push_back(Point({ -2,-1 }));
	//this->neighborModel.push_back(Point({ -2, 0 }));
	//this->neighborModel.push_back(Point({ -2, 1 }));
	//this->neighborModel.push_back(Point({ -2, 2 }));

	//this->neighborModel.push_back(Point({ -1,-2 }));
	this->neighborModel.push_back(Point({-1,-1}));
	this->neighborModel.push_back(Point({-1, 0}));
	this->neighborModel.push_back(Point({-1, 1}));
	//this->neighborModel.push_back(Point({ -1,2 }));

	//this->neighborModel.push_back(Point({ 0,-2 }));
	this->neighborModel.push_back(Point({ 0,-1}));
	this->neighborModel.push_back(Point({ 0, 1}));
	//this->neighborModel.push_back(Point({ 0, 2 }));

	//this->neighborModel.push_back(Point({ 1,-2 }));
	this->neighborModel.push_back(Point({ 1,-1}));
	this->neighborModel.push_back(Point({ 1, 0}));
	this->neighborModel.push_back(Point({ 1, 1}));
	//this->neighborModel.push_back(Point({ 1, 2 }));

	//this->neighborModel.push_back(Point({ 2,-2 }));
	//this->neighborModel.push_back(Point({ 2,-1 }));
	//this->neighborModel.push_back(Point({ 2, 0 }));
	//this->neighborModel.push_back(Point({ 2, 1 }));
	//this->neighborModel.push_back(Point({ 2, 2 }));


	//this->smoothModel.push_back(Point({ -1,-1 }));
	this->smoothModel.push_back(Point({ -1, 0 }));
	//this->smoothModel.push_back(Point({ -1, 1 }));
	this->smoothModel.push_back(Point({ 0,-1 }));
	this->smoothModel.push_back(Point({ 0, 1 }));
	//this->smoothModel.push_back(Point({ 1,-1 }));
	this->smoothModel.push_back(Point({ 1, 0 }));
	//this->smoothModel.push_back(Point({ 1, 1 }));



	reset();
}

void CellularAutomata::reset()
{
	this->iterations = 0;
	for (int i = 0; i < area; i++) {
		this->cellMap[i]->setState(0);
		this->cellsToUpdate.push(i);
	}
	//initBorder();
	//generateRoads();
	generateEntryRoad(1);
	generatePoints(0, 1, 25);

	for (int i = 0; i < 7; i++) {
		fastAccrete(0, 1);
	}


	floodFillQueue(hashPoint(0, (h / 2)), 1, 1000);
	clearState(1);
	replaceState(1000, 1);


	for (int i = 0; i < area; i++) {
		this->cellsToUpdate.push(i);
	}

	generatePoints(1, 2, 25);
	for (int i = 0; i < 3; i++) {
		fastAccrete(1, 2);
	}

	for (int i = 0; i < area; i++) {
		this->cellsToUpdate.push(i);
	}
	generatePoints(1, 4, 30);
	for (int i = 0; i < 6; i++) {
		fastAccrete(1, 4);
	}
	//smooth(4, 1);
	smooth(4, 1);
	smooth(4, 1);

	generateEntryRoad(3);
}

void CellularAutomata::generatePoints(int targetState, int replacementState, int k)
{
	for (int i = 12; i < w -12; i++) {
		for (int j = 12; j < h-12; j++) {
			if (this->cellMap[hashPoint(i,j)]->getState() == targetState) {
				if ((rand() % 100) <= k) {
					this->cellMap[hashPoint(i, j)]->setState(replacementState);
				}
			}
		}
	}
}

void CellularAutomata::generateEntryRoad(int state)
{
	//for (int i = 0; i < k; i++) {
	//	this->cellMap[hashPoint(rand() % w * 0.70 + w * 0.15, rand() % h * 0.70 + h * 0.15)]->setState(1);
	//}
	//for (int i = 5; i < w - 5; i++) {
	//	this->mapBorder.push_back(hashPoint(i, 0));
	//	//set state to 'BORDER_TOP'
	//	this->cellMap[hashPoint(i, 0)]->setState(2);
	//}

	for (int i = 0; i < w*0.30; i++) {
		this->cellMap[hashPoint(i, (h / 2) - 1)]->setState(state);
		this->cellMap[hashPoint(i, (h / 2))]->setState(state);
		this->cellMap[hashPoint(i, (h / 2) + 1)]->setState(state);
	}
}



void CellularAutomata::drawBorder()
{
	for (int i = 0; i < this->mapBorder.size(); i++)
	{
		this->cellMap[this->mapBorder.at(i)]->setState(2);
	}
}

bool CellularAutomata::isPointInBounds(int x, int y)
{
	return (
		x < w &&
		x >= 0 &&
		y < h &&
		y >= 0
		);
}

//int quadratic(int maxProb, int roadSegment, int roadLengthChoice) {
//	double C = (double) roadLengthChoice - 1.0;
//	double p = (double) maxProb;
//	double a = (4 * p) / (double)pow((double)C, 2);
//
//	return (int)(-a * pow((roadSegment - C / 2.0), 2) + p);
//}

//int easeIn(int maxProb, int roadSegment, int roadLengthChoice) {
//	double C = (double)roadLengthChoice - 1.0;
//	double p = (double)maxProb;
//	double a = (p) / (double)pow((double)C, 2);
//
//	return (int)(-a * pow((roadSegment - C), 2) + p);
//}

//void CellularAutomata::newRoad(std::vector<int> roadLengths, Point roadPoint, char direction, int roadSpawnRate, int roadDepth) {
//	if ((rand() % 100) <= roadSpawnRate && roadDepth <= 5) {
//		Point rp = roadPoint;
//		Point roadStartPoint = rp;
//
//		int roadLengthChoice = roadLengths.at(rand() % roadLengths.size());
//		int myRoadDepth = roadDepth + 1;
//
//		std::vector<int> newRoadLengths;
//
//
//
//		for (int i = 0; i < roadLengthChoice; i++) {
//			newRoadLengths.push_back(7 - myRoadDepth);
//			newRoadLengths.push_back(9 - myRoadDepth);
//
//			int spread = 3;
//			//TOP
//			if (direction == 'D') {
//				if (!isPointInBounds(rp.x, rp.y + spread)) break;
//				if (this->cellMap[hashPoint(rp.x, rp.y + spread)]->getState() == 8) break;
//				//newRoad(newRoadLengths, rp, 'L', easeIn(10 / myRoadDepth, i,roadLengthChoice - 1), myRoadDepth);
//				//newRoad(newRoadLengths, rp, 'R', easeIn(10 / myRoadDepth, i, roadLengthChoice - 1), myRoadDepth);
//				rp.y += spread;
//			}
//			//RIGHT
//			if (direction == 'L') {
//				if (!isPointInBounds(rp.x - spread, rp.y)) break;
//				if (this->cellMap[hashPoint(rp.x - spread, rp.y)]->getState() == 8) break;
//				//newRoad(newRoadLengths, rp, 'U', easeIn(10 / myRoadDepth, i, roadLengthChoice - 1), myRoadDepth);
//				//newRoad(newRoadLengths, rp, 'D', easeIn(10 / myRoadDepth, i, roadLengthChoice - 1), myRoadDepth);
//				rp.x -= spread;
//			}
//			//BOTTOM
//			if (direction == 'U') {
//				if (!isPointInBounds(rp.x, rp.y - spread)) break;
//				if (this->cellMap[hashPoint(rp.x, rp.y - spread)]->getState() == 8) break;
//				//newRoad(newRoadLengths, rp, 'L', easeIn(10 / myRoadDepth, i, roadLengthChoice - 1), myRoadDepth);
//				//newRoad(newRoadLengths, rp, 'R', easeIn(10 / myRoadDepth, i, roadLengthChoice - 1), myRoadDepth);
//				rp.y -= spread;
//			}
//			//LEFT
//			if (direction == 'R') {
//				if (!isPointInBounds(rp.x + spread, rp.y)) break;
//				if (this->cellMap[hashPoint(rp.x + spread, rp.y)]->getState() == 8) break;
//				//newRoad(newRoadLengths, rp, 'U', easeIn(10 / myRoadDepth, i, roadLengthChoice - 1), myRoadDepth);
//				//newRoad(newRoadLengths, rp, 'D', easeIn(10 / myRoadDepth, i, roadLengthChoice - 1), myRoadDepth);
//				rp.x += spread;
//			}
//			this->cellMap[hashPoint(rp.x, rp.y)]->setState(8);
//		}
//
//		Point roadEndPoint = rp;
//		LineSegment road = { roadStartPoint, roadEndPoint };
//		this->roads.push_back(road);
//	}
//
//
//}

//void CellularAutomata::generateRoads()
//{
//	this->roads.clear();
//	//pick random spot on map border
//	int startPoint = this->mapBorder.at(rand() % this->mapBorder.size());
//	int borderType = this->cellMap[startPoint]->getState();
//	this->cellMap[startPoint]->setState(7);
//	Point roadPoint = unhashPoint(startPoint);
//
//	std::vector<int> roadLengths;
//	roadLengths.push_back(25);
//	roadLengths.push_back(30);
//	//TOP
//	if (borderType == 2) {
//		roadPoint.y += 2;
//		newRoad(roadLengths, roadPoint, 'D', 100, 0);
//	}
//	//RIGHT
//	if (borderType == 3) {
//		roadPoint.x -= 2;
//		newRoad(roadLengths, roadPoint, 'L', 100, 0);
//	}
//	//BOTTOM
//	if (borderType == 4) {
//		roadPoint.y -= 2;
//		newRoad(roadLengths, roadPoint, 'U', 100, 0);
//	}
//	//LEFT
//	if (borderType == 5) {
//		roadPoint.x += 2;
//		newRoad(roadLengths, roadPoint, 'R', 100, 0);
//	}
//	this->cellMap[hashPoint(roadPoint.x, roadPoint.y)]->setState(8);
//
//}






void CellularAutomata::smooth(int targetState, int replacementState)
{
	std::queue<int> cellsUnflipped;
	for (int i = 0; i < area; i++) {
		int cellState = this->cellMap[i]->getState();
		if (cellState == targetState) {
			int flippedNeighbors = 0;

			Point offset;
			for (int j = 0; j < this->smoothModel.size(); j++)
			{
				offset = this->smoothModel.at(j);
				int neighborState = this->getNeighborState(i, offset.x, offset.y);
				if (neighborState >= 0) {
					if (neighborState == targetState) {
						flippedNeighbors++;
					}
				}
			}

			if (flippedNeighbors < 2) {
				cellsUnflipped.push(i);
			}
		}
	}

	while (!cellsUnflipped.empty()) {
		this->cellMap[cellsUnflipped.front()]->setState(replacementState);
		cellsUnflipped.pop();
	}
}


void CellularAutomata::clearState(int state)
{
	replaceState(state, 0);
}

void CellularAutomata::replaceState(int targetState, int replacementState)
{
	for (int i = 0; i < this->cellMap.size(); i++) {
		if (this->cellMap[i]->getState() == targetState) {
			this->cellMap[i]->setState(replacementState);
		}
	}
}

void CellularAutomata::floodFillStack(int startCell, int targetState, int replacementState)
{
	if (!isPointInBounds(unhashPoint(startCell).x, unhashPoint(startCell).y)) return;
	if (targetState == replacementState) return;
	else if (this->cellMap[startCell]->getState() != targetState) return;
	else (this->cellMap[startCell]->setState(replacementState));
	floodFillStack(hashPoint(unhashPoint(startCell).x - 1, unhashPoint(startCell).y),targetState,replacementState);
	floodFillStack(hashPoint(unhashPoint(startCell).x + 1, unhashPoint(startCell).y), targetState, replacementState);
	floodFillStack(hashPoint(unhashPoint(startCell).x, unhashPoint(startCell).y - 1), targetState, replacementState);
	floodFillStack(hashPoint(unhashPoint(startCell).x, unhashPoint(startCell).y + 1), targetState, replacementState);
}

void CellularAutomata::floodFillQueue(int startCell, int targetState, int replacementState)
{
	if (!isPointInBounds(unhashPoint(startCell).x, unhashPoint(startCell).y)) return;
	if (targetState == replacementState) return;
	else if (this->cellMap[startCell]->getState() != targetState) return;
	this->cellMap[startCell]->setState(replacementState);
	std::queue<int> cellQueue;
	cellQueue.push(startCell);
	while (!cellQueue.empty()) {
		int currentCell = cellQueue.front();
		cellQueue.pop();
		if (this->cellMap[hashPoint(unhashPoint(currentCell).x - 1, unhashPoint(currentCell).y)]->getState() == targetState) {
			this->cellMap[hashPoint(unhashPoint(currentCell).x - 1, unhashPoint(currentCell).y)]->setState(replacementState);
			cellQueue.push(hashPoint(unhashPoint(currentCell).x - 1, unhashPoint(currentCell).y));
		}
		if (this->cellMap[hashPoint(unhashPoint(currentCell).x + 1, unhashPoint(currentCell).y)]->getState() == targetState) {
			this->cellMap[hashPoint(unhashPoint(currentCell).x + 1, unhashPoint(currentCell).y)]->setState(replacementState);
			cellQueue.push(hashPoint(unhashPoint(currentCell).x + 1, unhashPoint(currentCell).y));
		}
		if (this->cellMap[hashPoint(unhashPoint(currentCell).x, unhashPoint(currentCell).y - 1 )]->getState() == targetState) {
			this->cellMap[hashPoint(unhashPoint(currentCell).x, unhashPoint(currentCell).y - 1)]->setState(replacementState);
			cellQueue.push(hashPoint(unhashPoint(currentCell).x, unhashPoint(currentCell).y - 1));
		}
		if (this->cellMap[hashPoint(unhashPoint(currentCell).x, unhashPoint(currentCell).y + 1)]->getState() == targetState) {
			this->cellMap[hashPoint(unhashPoint(currentCell).x, unhashPoint(currentCell).y + 1)]->setState(replacementState);
			cellQueue.push(hashPoint(unhashPoint(currentCell).x, unhashPoint(currentCell).y + 1));
		}
	}
	return;

}

void CellularAutomata::fastAccrete(int targetState, int replacementState)
{
	std::queue<int> cellsFlipped;
	int numberOfCellsToUpdate = this->cellsToUpdate.size();

	for (int i = 0; i < numberOfCellsToUpdate; i++) {

		int indexToUpdate = this->cellsToUpdate.front();

		int cellState = this->cellMap[indexToUpdate]->getState();

		if (cellState == targetState) {
			int flippedNeighbors = 0;

			std::queue<int> neighborsToUpdateNext;

			Point offset;
			for (int i = 0; i < this->neighborModel.size(); i++)
			{
				offset = this->neighborModel.at(i);
				int neighborState = this->getNeighborState(indexToUpdate, offset.x, offset.y);
				if (neighborState >= 0) {
					if (neighborState == replacementState) {
						flippedNeighbors++;
					}
					else {
						neighborsToUpdateNext.push(indexToUpdate + offset.x + w * offset.y);
					}
				}
			}




			//check if the number of flipped neighbors is >= 2;
			if (flippedNeighbors >= N) {

				//if this cell has been flipped, its unflipped neighbors must be queued for the next update
				//std::cout << neighborsToUpdateNext.size() << std::endl;
				while (!neighborsToUpdateNext.empty()) {
					if (!this->cellMap[neighborsToUpdateNext.front()]->isQueuedForUpdate()) {
						this->cellsToUpdate.push(neighborsToUpdateNext.front());	//push the unflipped neighbor IF it hasn't already been pushed 
						this->cellMap[neighborsToUpdateNext.front()]->setQueuedForUpdate(true);
					}
					neighborsToUpdateNext.pop();
				}
				cellsFlipped.push(indexToUpdate);
			}
		}

		this->cellsToUpdate.pop();
	}

	for (int i = 0; i < this->cellsToUpdate.size(); i++) {
		this->cellMap[this->cellsToUpdate.front()]->setQueuedForUpdate(false);
		this->cellsToUpdate.push(this->cellsToUpdate.front());
		this->cellsToUpdate.pop();
	}

	while (!cellsFlipped.empty()) {
		this->cellMap[cellsFlipped.front()]->setState(replacementState);
		cellsFlipped.pop();
	}
}

void CellularAutomata::update()
{

}

void CellularAutomata::render(SDL_Renderer* renderer)
{
	for (int i = 0; i < area; i++) {
		this->cellMap[i]->render(renderer);
	}

	for (int i = 0; i < this->roads.size(); i++) {
		LineSegment road = this->roads.at(i);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0x22, 0x22, 0xFF);
		SDL_RenderDrawLine(renderer, road.p1.x * this->cellSize, road.p1.y * this->cellSize, road.p2.x * this->cellSize, road.p2.y * this->cellSize);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}
}

int CellularAutomata::getNeighborState(int index, int xoffset, int yoffset)
{
	if (((index % w) + xoffset < w) &&
		((index % w) + xoffset >= 0) &&
		(index + w*yoffset >= 0) &&
		(index + w*yoffset < area) 
		)
	{
		return this->cellMap[index + xoffset + w*yoffset]->getState();
	}
	return -1;
}

void CellularAutomata::initBorder()
{
	this->mapBorder.clear();
	for (int i = 5; i < w - 5; i++) {
		this->mapBorder.push_back(hashPoint( i, 0 ));
		//set state to 'BORDER_TOP'
		this->cellMap[hashPoint(i, 0)]->setState(2);
	}
	for (int j = 5; j < h - 5; j++) {
		this->mapBorder.push_back(hashPoint(w - 1, j));
		this->cellMap[hashPoint(w - 1, j)]->setState(3);
	}
	for (int i = w - 6; i >= 5; i--) {
		this->mapBorder.push_back(hashPoint(i, h - 1));
		this->cellMap[hashPoint(i, h - 1)]->setState(4);
	}
	for (int j = h - 6; j >= 5; j--) {
		this->mapBorder.push_back(hashPoint(0 , j));
		this->cellMap[hashPoint(0, j)]->setState(5);
	}

}



