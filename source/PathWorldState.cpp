/* ========================================================================
   File: PathWorldState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "PathWorldState.h"
#include <time.h>
#define UP true
#define DOWN false

void PathWorldState::Init()
{
	srand(time(NULL));
	// Initialize Graph Texture
	graphTexture = {};
	StaticTexture_Init(&graphTexture, FIXED_WIDTH, FIXED_HEIGHT, display->renderer);
	StaticTexture_StartDrawing(&graphTexture);
	//Start
	
	grid = {};
	StartWalls();
	// Draw here i.e:
	/*boxColor(display->renderer, FIXED_WIDTH / 2 - FIXED_WIDTH / 4,
		FIXED_HEIGHT / 2 - FIXED_HEIGHT / 4,
		FIXED_WIDTH / 2 + FIXED_WIDTH / 4,
		FIXED_HEIGHT / 2 + FIXED_HEIGHT / 4, Colors::WISTERIA);*/
	StaticTexture_EndDrawing(&graphTexture);
}

void PathWorldState::Deinit()
{
	StaticTexture_Free(&graphTexture);
}

StateCode PathWorldState::HandleInput()
{
	if (InputSingleton::Instance()->doQuit)
	{
		return StateCode::QUIT;
	}
	// keyboard Input Handling
	else if (InputSingleton::Instance()->IsKeyDown(SDL_SCANCODE_ESCAPE))
	{
		return StateCode::MENU;
	}
	return StateCode::CONTINUE;
}

void PathWorldState::Update(float deltaTime)
{

}

void PathWorldState::Render()
{
	Grid_Render(&grid, display->renderer,0, Colors::ASBESTOS);
	StaticTexture_Render(&graphTexture, display->renderer, 0, 0);
}

void PathWorldState::StartWalls()
{
	for (int i = 0; i < GRID_WIDTH; i++)
	{
		grid.array[i][0].isWall = true;
		grid.array[i][0].weight = 9.0f;

		grid.array[i][GRID_HEIGHT - 1].isWall = true;
		grid.array[i][GRID_HEIGHT - 1].weight = 9.0f;
	}
	for (int j = 0; j < GRID_HEIGHT; j++)
	{
		grid.array[0][j].isWall = true;
		grid.array[0][j].weight = 9.0f;

		grid.array[GRID_WIDTH - 1][j].isWall = true;
		grid.array[GRID_WIDTH - 1][j].weight = 9.0f;
	}
	/*int _numWalls = GRID_WIDTH*GRID_HEIGHT / 5;
	for (int i = 0; i < _numWalls; i++)
	{
		
		int _1 = rand() % GRID_WIDTH;
		int _2 = rand() % GRID_HEIGHT;
		grid.array[_1][_2].isWall = true;
		grid.array[_1][_2].weight = 9.0f;
	}*/
	grid.array[1][0].weight = 1.0f;
	grid.array[1][0].isWall = false;
	grid.array[GRID_WIDTH-1][1].weight = 2.0f;
	grid.array[GRID_WIDTH-1][1].isWall = false;
	grid.array[0][GRID_HEIGHT-2].weight = 3.0f;
	grid.array[0][GRID_HEIGHT-2].isWall = false;
	grid.array[GRID_WIDTH-2][GRID_HEIGHT-1].weight = 4.0f;
	grid.array[GRID_WIDTH-2][GRID_HEIGHT-1].isWall = false;
	
	
	int _1 = rand() % (GRID_WIDTH - 2) + 2;
	int _2 = rand() % (GRID_HEIGHT - 2) + 2;

	createWall(_1, _2);
}

void PathWorldState::createWall(int _width, int _height)
{
	int currX = _width;
	int currY = _height;
	//int lastX = _width;
	//int lastY = _height;
	for (int i = 0; i < GRID_WIDTH; i++){
		if (grid.array[currX-1][currY].isWall){
			int _1 = rand() % (_width+1)+currX;
			grid.array[_1][currY].isWall = false;
			grid.array[_1][currY].weight = 0.0f;
			break;
		}
		else
		{
			grid.array[currX-1][currY].isWall = true;
			grid.array[currX-1][currY].weight = 9.0f;
			currX--;
			
			//currY = _height;
		}
	}
	currX = _width;
	currY = _height;
	for (int i = 0; i < GRID_WIDTH; i++) {
		if (grid.array[currX + 1][currY].isWall) {
			int _1 = rand() % (currX-_width-1)+ _width;
			grid.array[_1][currY].isWall = false;
			grid.array[_1][currY].weight = 0.0f;
			break;
		}
		else
		{
			grid.array[currX + 1][currY].isWall = true;
			grid.array[currX + 1][currY].weight = 9.0f;
			currX++;
			//currY = _height;
		}
	}
	currX = _width;
	currY = _height;
	for (int i = 0; i < GRID_HEIGHT; i++) {
		if (grid.array[currX][currY-1].isWall) {
			int _1 = rand() % (_height+1)+currY;
			grid.array[currX][_1].isWall = false;
			grid.array[currX][_1].weight = 0.0f;
			break;
		}
		else
		{
			grid.array[currX][currY-1].isWall = true;
			grid.array[currX][currY-1].weight = 9.0f;
			currY--;
			//currY = _height;
		}
	}
	currX = _width;
	currY = _height;
	for (int i = 0; i < GRID_HEIGHT; i++) {
		if (grid.array[currX][currY + 1].isWall) {
			int _1 = rand() % (currY - _height-1) + _height;
			grid.array[currX][_1].isWall = false;
			grid.array[currX][_1].weight = 0.0f;
			break;
		}
		else
		{
			grid.array[currX][currY + 1].isWall = true;
			grid.array[currX][currY + 1].weight = 9.0f;
			currY++;
			//currY = _height;
		}
	}
	grid.array[_width][_height].isWall = true;
	grid.array[_width][_height].weight = 9.0f;
}
