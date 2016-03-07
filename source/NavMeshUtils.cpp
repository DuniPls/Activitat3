/* ========================================================================
   File: NavMeshUtils.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "NavMeshUtils.h"

void NavMeshCell::Render(SDL_Renderer* renderer, Uint32 color)
{
	trigonColor(renderer, (int)vertex[0].x, (int)vertex[0].y,
		(int)vertex[1].x, (int)vertex[1].y,
		(int)vertex[2].x, (int)vertex[2].y,
		color);
}

void NavMeshCell::CalculateCenter()
{
	center.x = (vertex[0].x + vertex[1].x + vertex[2].x) / 3.0f;
	center.y = (vertex[0].y + vertex[1].y + vertex[2].y) / 3.0f;
}
