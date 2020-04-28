#pragma once
#include <SDL.h>

class CellAgent
{
public:
	CellAgent(int initialState, int x, int y, int cellSize);
	~CellAgent();
	void setState(int state);
	void setNextState(int nextState);
	int getState();
	int getNextState();

	void setAge(int age);

	void update();

	void render(SDL_Renderer* renderer);



private:
	int state;
	int age;
	int nextState;
	int x, y;
	SDL_Rect rect;
};

