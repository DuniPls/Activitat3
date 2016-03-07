/* ========================================================================
   File: StaticTexture.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "StaticTexture.h"


void StaticTexture_Init(StaticTexture* texture, int width, int height, SDL_Renderer* renderer)
{
	texture->width = width;
	texture->height = height;
	texture->renderer = renderer;

	Texture_CreateBlank(&texture->targetedTexture, texture->renderer, texture->width, texture->height, SDL_TEXTUREACCESS_TARGET);
	SDL_SetTextureBlendMode(texture->targetedTexture.texture, SDL_BLENDMODE_BLEND);
}

void StaticTexture_Free(StaticTexture* texture)
{
	texture->renderer = nullptr;
	Texture_Free(&texture->targetedTexture);
}

void StaticTexture_StartDrawing(StaticTexture* texture)
{
	//Set self as render target
	Texture_SetAsRenderTarget(&texture->targetedTexture, texture->renderer);

	//Clear screen
	SDL_SetRenderDrawColor(texture->renderer, 0xFF, 0xFF, 0xFF, 0x00);
	SDL_RenderClear(texture->renderer);
}

void StaticTexture_EndDrawing(StaticTexture* texture)
{
	//Set self as render target
	Texture_SetDefaultTarget(texture->renderer);
}

void StaticTexture_Render(StaticTexture* texture, SDL_Renderer* renderer, int x, int y)
{
	Texture_Render(&texture->targetedTexture, renderer, x, y);
}
