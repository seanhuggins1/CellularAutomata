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

	bool isQueuedForUpdate();
	void setQueuedForUpdate(bool queued);

private:
	int state;
	int age;
	int nextState;
	bool queuedForUpdate;
	int x, y;
	SDL_Rect rect;
};

