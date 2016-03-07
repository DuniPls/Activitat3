/* ========================================================================
   File: StaticTexture.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Texture.h"

struct StaticTexture
{
	int width = 0;
	int height = 0;
	Texture targetedTexture;
	SDL_Renderer* renderer = nullptr;
};

void StaticTexture_Init(StaticTexture* texture, int width, int height, SDL_Renderer* renderer);
void StaticTexture_Free(StaticTexture* texture);

void StaticTexture_StartDrawing(StaticTexture* texture);
void StaticTexture_EndDrawing(StaticTexture* texture);

void StaticTexture_Render(StaticTexture* texture, SDL_Renderer* renderer, int x, int y);