/* ========================================================================
   File: PathWorldState.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "PathWorldState.h"
#include <time.h>

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
	
	int _numWalls = GRID_WIDTH*GRID_HEIGHT / 5;
	for (int i = 0; i < _numWalls; i++)
	{
		
		int _1 = rand() % GRID_WIDTH;
		int _2 = rand() % GRID_HEIGHT;
		grid.array[_1][_2].isWall = true;
		grid.array[_1][_2].weight = 9.0f;
	}
	grid.array[0][0].weight = 1.0f;
	grid.array[0][0].isWall = false;
	grid.array[GRID_WIDTH-1][0].weight = 2.0f;
	grid.array[GRID_WIDTH-1][0].isWall = false;
	grid.array[0][GRID_HEIGHT-1].weight = 3.0f;
	grid.array[0][GRID_HEIGHT-1].isWall = false;
	grid.array[GRID_WIDTH-1][GRID_HEIGHT-1].weight = 4.0f;
	grid.array[GRID_WIDTH-1][GRID_HEIGHT-1].isWall = false;
}
