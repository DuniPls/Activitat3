/* ========================================================================
   File: GridUtils.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "GridUtils.h"
using namespace std;

Node Node_Create(int x, int y, float weight)
{
	Node node = {};
	node.position = {(float)x, (float)y};
	node.weight = weight;
	return node;
}

void Node_Reset(Node* node)
{
	node->costSoFar = -1.0f;
	node->priority = 0.0f;
	node->heuristic = 0.0f;
	node->cameFrom = nullptr;
	node->alreadyUsed = false;
}

int Node_GetDirection(Node* from, Node* to)
{
	if (to->position.x < from->position.x && to->position.y == from->position.y)
	{
		return 2;
	}
	else if (to->position.x > from->position.x && to->position.y == from->position.y)
	{
		return 0;
	}
	else if (to->position.x == from->position.x && to->position.y < from->position.y)
	{
		return 3;
	}
	else if (to->position.x == from->position.x && to->position.y > from->position.y)
	{
		return 1;
	}
	else if (to->position.x > from->position.x && to->position.y > from->position.y)
	{
		return 4;
	}
	else if (to->position.x < from->position.x && to->position.y > from->position.y)
	{
		return 5;
	}
	else if (to->position.x < from->position.x && to->position.y < from->position.y)
	{
		return 6;
	}

	return 7;
}

void Grid_Init(Grid* grid)
{
	for (int i = 0; i < grid->width; ++i)
	{
		for (int j = 0; j < grid->height; ++j)
		{
			grid->array[i][j] = Node_Create(i, j, 0.0f);
		}
	}
}

vector<Node*> Grid_GetNeighbors(Grid* grid, Node* node, bool allowDiagonal)
{
	int numberOfDirections = 4;

	Vector2D directions[8] = { { 1.0f, 0.0f },{ 0.0f, 1.0f },
								{ -1.0f, 0.0f },{ 0.0f, -1.0f },
								{ -1.0f, -1.0f },{ 1.0f, -1.0f },
								{ -1.0f, 1.0f },{ 1.0f, 1.0f } };
	if (allowDiagonal == true)
	{
		numberOfDirections = 8;
	}
	
	vector<Node*> result;
	Vector2D currentDirection;
	for (int i = 0; i < numberOfDirections; ++i)
	{
		currentDirection = directions[i];

		int neighborX = node->position.x + currentDirection.x;
		int neighborY = node->position.y + currentDirection.y;

		if (0 <= neighborX && neighborX < grid->width
			&& 0 <= neighborY && neighborY < grid->height)
		{
			if (grid->array[neighborX][neighborY].isWall == false)
			{
				result.push_back(&grid->array[neighborX][neighborY]);
			}
		}
	}
	return result;
}

void Grid_InsertWall(Grid* grid, int x, int y)
{
	grid->array[x][y].isWall = true;
	grid->wallNodes.push_back(&grid->array[x][y]);
}

void Grid_DeleteWall(Grid* grid, int x, int y)
{
	grid->array[x][y].isWall = false;
	auto it = std::find(grid->wallNodes.begin(), grid->wallNodes.end(),
		&grid->array[x][y]);
	if (it != grid->wallNodes.end())
	{
		grid->wallNodes.erase(it);
	}
}

bool Grid_CollisionWithWall(Grid* grid, Vector2D start, Vector2D end)
{
	bool doesIntersect = false;

	start *= grid->cellSize;
	end *= grid->cellSize;
	start += Vector2D(grid->cellSize / 2.0f, grid->cellSize / 2.0f);
	end += Vector2D(grid->cellSize / 2.0f, grid->cellSize / 2.0f);

	// For each wall in grid
	for (int i = 0; i < (int)grid->wallNodes.size(); ++i)
	{
		Node* currentNode = grid->wallNodes[i];
		// For each side of the wall
		for (int j = 0; j < 4; ++j)
		{
			Vector2D wallStart = currentNode->position * grid->cellSize;
			Vector2D wallEnd = wallStart + Vector2D(GRID_SIZE, 0.0f);
			if (Vector2DUtils::SegmentSegmentIntersection(start,
				end,
				wallStart,
				wallEnd, false) == true)
			{
				doesIntersect = true;
				break;
			}
			wallStart = currentNode->position * grid->cellSize;
			wallEnd = wallStart + Vector2D(0.0f, GRID_SIZE);
			if (Vector2DUtils::SegmentSegmentIntersection(start,
				end,
				wallStart,
				wallEnd, false) == true)
			{
				doesIntersect = true;
				break;
			}
			wallStart = currentNode->position * grid->cellSize + Vector2D(GRID_SIZE, 0.0f);
			wallEnd = wallStart + Vector2D(0.0f, GRID_SIZE);
			if (Vector2DUtils::SegmentSegmentIntersection(start,
				end,
				wallStart,
				wallEnd, false) == true)
			{
				doesIntersect = true;
				break;
			}
			wallStart = currentNode->position * grid->cellSize + Vector2D(0.0f, GRID_SIZE);
			wallEnd = wallStart + Vector2D(GRID_SIZE, 0.0f);
			if (Vector2DUtils::SegmentSegmentIntersection(start,
				end,
				wallStart,
				wallEnd, false) == true)
			{
				doesIntersect = true;
				break;
			}

		}
		if (doesIntersect == true)
		{
			break;
		}
	}

	return doesIntersect;
}

// Functions for Simple Dijkstra
float Grid_GetCost(Grid* grid, Node* to)
{
	return to->weight;
}

void Grid_ChangeWeight(Grid* grid, int x, int y)
{
	grid->array[x][y].weight += 1;
	if (grid->array[x][y].weight > MAX_WEIGHT)
	{
		grid->array[x][y].weight = 1.0f;
	}
}

void Grid_Render(Grid* grid, SDL_Renderer* renderer, Vector2D gridOrigin, Uint32 gridColor)
{

	/*for (int i = 0; i <= grid->width; ++i)
	{
		lineColor(renderer, gridOrigin.x + i * grid->cellSize, gridOrigin.y,
			gridOrigin.x + i * grid->cellSize, gridOrigin.y + grid->height * grid->cellSize, gridColor);
	}

	for (int j = 0; j <= grid->height; ++j)
	{
		lineColor(renderer, gridOrigin.x, gridOrigin.y + j * grid->cellSize,
			gridOrigin.x + grid->width * grid->cellSize, gridOrigin.y + j * grid->cellSize, gridColor);
	}*/
	for (int i = 0; i < GRID_WIDTH; ++i)
	{
		for (int j = 0; j < GRID_HEIGHT; ++j)
		{
			int currentWeight = grid->array[i][j].weight;
			Uint32 colorToUse;
			switch (currentWeight) {
			case 0:
				colorToUse = Colors::EMERALD;
				break;
			case 1:
				colorToUse = Colors::CARROT;
				break;
			case 2:
				colorToUse = Colors::MIDNIGHT_BLUE;
				break;
			case 3:
				colorToUse = Colors::BELIZE_HOLE;
				break;
			case 4:
				colorToUse = Colors::WISTERIA;
				break;
			case 9:
				colorToUse = Colors::WHITE;
				break;
			}
			boxColor(renderer, gridOrigin.x + i * GRID_SIZE, gridOrigin.y + j * GRID_SIZE,
				gridOrigin.x + i * GRID_SIZE + GRID_SIZE, gridOrigin.y + j * GRID_SIZE + GRID_SIZE, colorToUse);
		}
	}
}

