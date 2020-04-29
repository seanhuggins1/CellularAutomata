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

	initBorder();

	generateRoads();

	this->neighborModel.push_back(Point({ -2,-2 }));
	this->neighborModel.push_back(Point({ -2,-1 }));
	this->neighborModel.push_back(Point({ -2, 0 }));
	this->neighborModel.push_back(Point({ -2, 1 }));
	this->neighborModel.push_back(Point({ -2, 2 }));

	this->neighborModel.push_back(Point({ -1,-2 }));
	this->neighborModel.push_back(Point({-1,-1}));
	this->neighborModel.push_back(Point({-1, 0}));
	this->neighborModel.push_back(Point({-1, 1}));
	this->neighborModel.push_back(Point({ -1,2 }));

	this->neighborModel.push_back(Point({ 0,-2 }));
	this->neighborModel.push_back(Point({ 0,-1}));
	this->neighborModel.push_back(Point({ 0, 1}));
	this->neighborModel.push_back(Point({ 0, 2 }));

	this->neighborModel.push_back(Point({ 1,-2 }));
	this->neighborModel.push_back(Point({ 1,-1}));
	this->neighborModel.push_back(Point({ 1, 0}));
	this->neighborModel.push_back(Point({ 1, 1}));
	this->neighborModel.push_back(Point({ 1, 2 }));

	this->neighborModel.push_back(Point({ 2,-2 }));
	this->neighborModel.push_back(Point({ 2,-1 }));
	this->neighborModel.push_back(Point({ 2, 0 }));
	this->neighborModel.push_back(Point({ 2, 1 }));
	this->neighborModel.push_back(Point({ 2, 2 }));



	this->smoothModel.push_back(Point({ -1, 0 }));
	this->smoothModel.push_back(Point({ 0,-1 }));
	this->smoothModel.push_back(Point({ 0, 1 }));
	this->smoothModel.push_back(Point({ 1, 0 }));
}

void CellularAutomata::reset()
{
	this->iterations = 0;
	for (int i = 0; i < area; i++) {
		this->cellMap[i]->setState(0);
		this->cellsToUpdate.push(i);
	}
	initBorder();
	generateRoads();
	
	//for (int i = 0; i < k; i++) {
	//	this->cellMap[hashPoint(rand() % w / 2 + (w / 4), rand() % h / 2 + (h / 4))]->setState(1);
	//}
}

void CellularAutomata::drawBorder()
{
	for (int i = 0; i < this->mapBorder.size(); i++)
	{
		this->cellMap[this->mapBorder.at(i)]->setState(2);
	}
}

void CellularAutomata::generateRoads()
{
	//pick random spot on map border
	int startPoint = this->mapBorder.at(rand() % this->mapBorder.size());
	int borderType = this->cellMap[startPoint]->getState();
	this->cellMap[startPoint]->setState(7);
	Point roadPoint = unhashPoint(startPoint);

	//TOP
	if (borderType == 2) {
		roadPoint.y += 2;
	}
	//RIGHT
	if (borderType == 3) {
		roadPoint.x -= 2;
	}
	//BOTTOM
	if (borderType == 4) {
		roadPoint.y -= 2;
	}
	//LEFT
	if (borderType == 5) {
		roadPoint.x += 2;
	}
	this->cellMap[hashPoint(roadPoint.x, roadPoint.y)]->setState(8);

	for (int i = 0; i < 8; i++) {
		//TOP
		if (borderType == 2) {
			roadPoint.y += 3;
		}
		//RIGHT
		if (borderType == 3) {
			roadPoint.x -= 3;
		}
		//BOTTOM
		if (borderType == 4) {
			roadPoint.y -= 3;
		}
		//LEFT
		if (borderType == 5) {
			roadPoint.x += 3;
		}

		this->cellMap[hashPoint(roadPoint.x, roadPoint.y)]->setState(8);
	}




	
}



void CellularAutomata::smooth()
{
	std::queue<int> cellsUnflipped;
	for (int i = 0; i < area; i++) {
		int cellState = this->cellMap[i]->getState();
		if (cellState == 1) {
			int flippedNeighbors = 0;

			Point offset;
			for (int j = 0; j < this->smoothModel.size(); j++)
			{
				offset = this->smoothModel.at(j);
				int neighborState = this->getNeighborState(i, offset.x, offset.y);
				if (neighborState >= 0) {
					if (neighborState == 1) {
						flippedNeighbors++;
					}
				}
			}

			if (flippedNeighbors < 3) {
				this->cellMap[i]->setNextState(0);
				cellsUnflipped.push(i);
			}
		}
	}

	while (!cellsUnflipped.empty()) {
		this->cellMap[cellsUnflipped.front()]->setState(
			this->cellMap[cellsUnflipped.front()]->getNextState()
		);
		cellsUnflipped.pop();
	}
}

void CellularAutomata::update()
{
	if (this->iterations < this->maxIterations) {
		std::queue<int> cellsFlipped;

		int numberOfCellsToUpdate = this->cellsToUpdate.size();

		//std::cout << numberOfCellsToUpdate << std::endl;
		for (int i = 0; i < numberOfCellsToUpdate; i++) {
			//this->cellMap[i]->update();	//update the cell

			int indexToUpdate = this->cellsToUpdate.front();

			int cellState = this->cellMap[indexToUpdate]->getState();

			if (cellState == 0) {
				int flippedNeighbors = 0;

				std::queue<int> neighborsToUpdateNext;

				Point offset;
				for (int i = 0; i < this->neighborModel.size(); i++)
				{
					offset = this->neighborModel.at(i);
					int neighborState = this->getNeighborState(indexToUpdate, offset.x, offset.y);
					if (neighborState >= 0) {
						if (neighborState == 1) {
							flippedNeighbors++;
						}
						else {
							neighborsToUpdateNext.push(indexToUpdate + offset.x + w * offset.y);
						}
					}
				}




				//check if the number of flipped neighbors is >= 2;
				if (flippedNeighbors >= N) {
					this->cellMap[indexToUpdate]->setNextState(1);

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
			this->cellMap[cellsFlipped.front()]->setState(
				this->cellMap[cellsFlipped.front()]->getNextState()
			);

			cellsFlipped.pop();
		}
	}

	this->iterations++;

}

void CellularAutomata::render(SDL_Renderer* renderer)
{
	for (int i = 0; i < area; i++) {
		this->cellMap[i]->render(renderer);
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



