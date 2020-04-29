#include "CellularAutomata.h"
#include <ctime>
#include <iostream>
#include <stdio.h>

#include <queue>
#include <stdlib.h>
#include <vector>

CellularAutomata::CellularAutomata(int maxIterations, int k, int w, int h, int cellSize)
{
	this->iterations = 0;
	this->maxIterations = maxIterations;
	this->k = k;
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
	for (int i = 0; i < k; i++) {
		this->cellMap[rand() % this->area]->setState(1);
	}

	//this->neighborModel.push_back(NeighborOffset({ -2,-2 }));
	this->neighborModel.push_back(NeighborOffset({ -2,-1 }));
	this->neighborModel.push_back(NeighborOffset({ -2, 0 }));
	this->neighborModel.push_back(NeighborOffset({ -2, 1 }));
	//this->neighborModel.push_back(NeighborOffset({ -2, 2 }));

	this->neighborModel.push_back(NeighborOffset({ -1,-2 }));
	this->neighborModel.push_back(NeighborOffset({-1,-1}));
	this->neighborModel.push_back(NeighborOffset({-1, 0}));
	this->neighborModel.push_back(NeighborOffset({-1, 1}));
	this->neighborModel.push_back(NeighborOffset({ -1,2 }));

	this->neighborModel.push_back(NeighborOffset({ 0,-2 }));
	this->neighborModel.push_back(NeighborOffset({ 0,-1}));
	this->neighborModel.push_back(NeighborOffset({ 0, 1}));
	this->neighborModel.push_back(NeighborOffset({ 0, 2 }));

	this->neighborModel.push_back(NeighborOffset({ 1,-2 }));
	this->neighborModel.push_back(NeighborOffset({ 1,-1}));
	this->neighborModel.push_back(NeighborOffset({ 1, 0}));
	this->neighborModel.push_back(NeighborOffset({ 1, 1}));
	this->neighborModel.push_back(NeighborOffset({ 1, 2 }));

	//this->neighborModel.push_back(NeighborOffset({ 2,-2 }));
	this->neighborModel.push_back(NeighborOffset({ 2,-1 }));
	this->neighborModel.push_back(NeighborOffset({ 2, 0 }));
	this->neighborModel.push_back(NeighborOffset({ 2, 1 }));
	//this->neighborModel.push_back(NeighborOffset({ 2, 2 }));
}

void CellularAutomata::reset()
{
	this->iterations = 0;
	for (int i = 0; i < area; i++) {
		this->cellMap[i]->setState(0);
		this->cellMap[i]->setAge(0);
		this->cellsToUpdate.push(i);
	}

	//randomly flip k cells
	for (int i = 0; i < k; i++) {
		this->cellMap[rand() % this->area]->setState(1);
	}
}

struct NeighborPoint {
	int xoffset;
	int yoffset;
};

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

				NeighborOffset offset;
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
				if (flippedNeighbors >= 8) {
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



