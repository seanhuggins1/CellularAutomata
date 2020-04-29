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
	this->queuedForUpdate = false;

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
	if (this->state == 2) {
		SDL_SetRenderDrawColor(renderer,
			0x00,
			0x22,
			0x55,
			0xFF
		);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}
	if (this->state == 3) {
		SDL_SetRenderDrawColor(renderer,
			0x22,
			0x55,
			0x77,
			0xFF
		);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}
	if (this->state == 4) {
		SDL_SetRenderDrawColor(renderer,
			0x55,
			0x88,
			0x99,
			0xFF
		);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}
	if (this->state == 5) {
		SDL_SetRenderDrawColor(renderer,
			0x88,
			0xBB,
			0xBB,
			0xFF
		);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}
	if (this->state == 7) {
		SDL_SetRenderDrawColor(renderer,
			0x00,
			0xFF,
			0x00,
			0xFF
		);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}
	if (this->state == 8) {
		SDL_SetRenderDrawColor(renderer,
			0xff,
			0xff,
			0x00,
			0xFF
		);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0x00);
	}
	//SDL_RenderDrawPoint(renderer, x, y);


	
	
}

bool CellAgent::isQueuedForUpdate()
{
	return this->queuedForUpdate;
}

void CellAgent::setQueuedForUpdate(bool queued)
{
	this->queuedForUpdate = queued;
}
