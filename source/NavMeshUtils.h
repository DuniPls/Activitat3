/* ========================================================================
   File: NavMeshUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

struct NavMeshCell
{
	int nodeIndex = 0;
	Vector2D vertex[3];
	Vector2D center;
	NavMeshCell* neighbors[3] = { nullptr, nullptr, nullptr };

	void Render(SDL_Renderer* renderer, Uint32 color);
	void CalculateCenter();
};
