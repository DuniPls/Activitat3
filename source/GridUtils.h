/* ========================================================================
   File: GridUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include <vector>


#define GRID_SIZE 16 // Cell size
#define GRID_WIDTH FIXED_WIDTH/GRID_SIZE // Cell number
#define GRID_HEIGHT FIXED_HEIGHT/GRID_SIZE// Cell number
#define MAX_WEIGHT 5

struct Node
{
	Vector2D position = { 0.0f, 0.0f };
	bool isWall = false;
	float weight = 0.0f;
	float costSoFar = -1.0f;
	float priority = 0.0f;
	float heuristic = 0.0f;
	Node* cameFrom = nullptr;
	bool alreadyUsed = false;
};

struct LessThanByPriority
{
	bool operator()(Node* lhs, Node* rhs)
	{
		return lhs->priority > rhs->priority;
	}
};

Node Node_Create(int x, int y, float weight);
int Node_GetDirection(Node* from, Node* to);
void Node_Reset(Node* node);

struct Grid
{
	int width = GRID_WIDTH;
	int height = GRID_HEIGHT;
	int cellSize = GRID_SIZE;
	Node array[GRID_WIDTH][GRID_HEIGHT];
	std::vector<Node*> wallNodes;
};

void Grid_Init(Grid* grid);
std::vector<Node*> Grid_GetNeighbors(Grid* grid, Node* node, bool allowDiagonals = false);
void Grid_InsertWall(Grid* grid, int x, int y);
void Grid_DeleteWall(Grid* grid, int x, int y);
bool Grid_CollisionWithWall(Grid* grid, Vector2D start, Vector2D end);
float Grid_GetCost(Grid* grid, Node* to);
void Grid_ChangeWeight(Grid* grid, int x, int y);
void Grid_Render(Grid* grid, SDL_Renderer* rendeder, Vector2D origin, Uint32 gridColor);


