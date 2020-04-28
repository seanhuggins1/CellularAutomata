#include "CellularAutomata.h"
#include <ctime>
#include <iostream>
#include <stdio.h>

#include <queue>
#include <stdlib.h>

CellularAutomata::CellularAutomata(int iterations, int k, int w, int h, int cellSize)
{
	this->iterations = iterations;
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

}

void CellularAutomata::reset()
{
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


void CellularAutomata::update()
{

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

			//TODO general formula for neighbors
			int leftNeighbor = indexToUpdate - 1;
			if (indexToUpdate % w != 0) {
				if (this->cellMap[leftNeighbor]->getState() == 1) { 
					flippedNeighbors++; 
				}
				else {
					neighborsToUpdateNext.push(leftNeighbor);
				}
			}

			int rightNeighbor = indexToUpdate + 1;
			if (rightNeighbor % w != 0) {
				if (this->cellMap[rightNeighbor]->getState() == 1) { 
					flippedNeighbors++; 
				}
				else {
					neighborsToUpdateNext.push(rightNeighbor);
				}
			}

			int upNeighbor = indexToUpdate - w;
			if (upNeighbor >= 0) {
				if (this->cellMap[upNeighbor]->getState() == 1) { 
					flippedNeighbors++; 
				}
				else {
					neighborsToUpdateNext.push(upNeighbor);
				}
			}

			int downNeighbor = indexToUpdate + w;
			if (downNeighbor < area) {
				if (this->cellMap[downNeighbor]->getState() == 1) { 
					flippedNeighbors++; 
				}
				else {
					neighborsToUpdateNext.push(downNeighbor);
				}
			}

			//the unflipped neighbors of a cell that should be flipped, that are cells that should be checked NEXT!


			//check if the number of flipped neighbors is >= 2;
			if (flippedNeighbors >= 2) {
				this->cellMap[indexToUpdate]->setNextState(1);

				//if this cell has been flipped, its unflipped neighbors must be queued for the next update
				//std::cout << neighborsToUpdateNext.size() << std::endl;
				while(!neighborsToUpdateNext.empty()) {
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

void CellularAutomata::render(SDL_Renderer* renderer)
{
	for (int i = 0; i < area; i++) {
		this->cellMap[i]->render(renderer);
	}

}


