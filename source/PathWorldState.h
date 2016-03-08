/* ========================================================================
   File: PathWorldState.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "PathfindingUtils.h"
#include "GraphUtils.h"
#include "AdvancedPathfindingUtils.h"
#include "NavMeshUtils.h"
#include "VisibilityUtils.h"

class PathWorldState : public BaseState
{
public:
	virtual void Init() override;
	virtual void Deinit() override;
	virtual StateCode HandleInput() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;

	// State Data
	Grid grid;
	bool visited[GRID_WIDTH][GRID_HEIGHT]; // For BFS Flood Fill
	int currentX;
	int currentY;
	int startX = 5;
	int startY = 5;
	int endX = 10;
	int endY = 10;
	Node* endGoal = nullptr;
	bool endGoalFound = false;

	// Textures
	StaticTexture graphTexture;
	void StartWalls();
	void createWall(int _width, int _height);
};