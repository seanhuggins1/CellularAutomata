#include "CellAgent.h"

CellAgent::CellAgent(int initialState, int x, int y, int cellSize)
{
	this->state = initialState;
	this->x = x;
	this->y = y;
	this->age = 0;
	this->rect.x = this->x + 2;
	this->rect.y = this->y + 2;
	this->rect.w = cellSize - 4;
	this->rect.h = cellSize - 4;

}

CellAgent::~CellAgent()
{
}

void CellAgent::setState(int state)
{
	this->state = state;
}

void CellAgent::setNextState(int nextState)
{
	this->nextState = nextState;
}

int CellAgent::getState()
{
	return this->state;
}

int CellAgent::getNextState()
{
	return this->nextState;
}

void CellAgent::setAge(int age)
{
	this->age = age;
}

void CellAgent::update()
{
	if (this->state == 1) {
		this->age += 3;
		if (this->age > 255) {
			this->age = 0;
			this->state = 0;
		}
	}
}

void CellAgent::render(SDL_Renderer* renderer)
{
	/*if (this->state == 0) {
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}*/
	if (this->state == 1) {
		SDL_SetRenderDrawColor(renderer, 
			0xFF - this->age,
			0xFF - this->age,
			0xFF - this->age,
			0xFF
		);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}

	//SDL_RenderDrawPoint(renderer, x, y);


	
	
}
