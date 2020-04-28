#include "CellularAutomata.h"
#include <ctime>
#include <iostream>
#include <stdio.h>

CellularAutomata::CellularAutomata(int iterations, int k, int w, int h, int cellSize)
{
	this->iterations = iterations;
	this->k = k;
	this->w = w;
	this->h = h;
	this->area = (this->w * this->h);

	srand(time(NULL));

	for (int i = 0; i < area; i++) {
		this->cellMap[i] = new CellAgent(0, (i % w) * cellSize, floor(i/w) * cellSize, cellSize);
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
	}

	//randomly flip k cells
	for (int i = 0; i < k; i++) {
		this->cellMap[rand() % this->area]->setState(1);
	}
}


void CellularAutomata::update()
{
	//std::map <int, CellAgent*> checkNext;

	for (int i = 0; i < area; i++) {
		//this->cellMap[i]->update();	//update the cell

		int cellState = this->cellMap[i]->getState();

		if (cellState == 0) {
			int flippedNeighbors = 0;

			int leftNeighbor = i - 1;
			if (i % w != 0) {
				if (this->cellMap[leftNeighbor]->getState() == 1) { flippedNeighbors++; }
			}

			int rightNeighbor = i + 1;
			if (rightNeighbor % w != 0) {
				if (this->cellMap[rightNeighbor]->getState() == 1) { flippedNeighbors++; }
			}

			int upNeighbor = i - w;
			if (upNeighbor >= 0) {
				if (this->cellMap[upNeighbor]->getState() == 1) { flippedNeighbors++; }
			}

			int downNeighbor = i + w;
			if (downNeighbor < area) {
				if (this->cellMap[downNeighbor]->getState() == 1) { flippedNeighbors++; }
			}

			//the unflipped neighbors of a cell that should be flipped, that are cells that should be checked NEXT!


			//check if the number of flipped neighbors is >= 2;
			if (flippedNeighbors >= 2) {
				this->cellMap[i]->setNextState(1);
			}
			else {
				this->cellMap[i]->setNextState(0);
			}

		}
		else {
			this->cellMap[i]->setNextState(1);
		}
	}

	for (int i = 0; i < area; i++) {
		this->cellMap[i]->setState(this->cellMap[i]->getNextState());
	}
}

void CellularAutomata::render(SDL_Renderer* renderer)
{
	for (int i = 0; i < area; i++) {
		this->cellMap[i]->render(renderer);
	}
}


