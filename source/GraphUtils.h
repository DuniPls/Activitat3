/* ========================================================================
   File: GraphUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include <vector>

#define K_MAX_NODES 500

struct GraphNode
{
	int index = 0;
	Vector2D position = { 0.0f, 0.0f };
	float priority = 0.0f;
	float heuristic = 0.0f; // For debug purposes
	float costSoFar = -1.0f;
	int cameFromIndex = -1;

	void Reset();

};

struct Connection
{
	int index = 0;
	int fromNodeIndex = 0;
	int toNodeIndex = 0;
	float weight = 0.0f;

	float GetCost();
	int GetFromNodeIndex();
	int GetToNodeIndex();
};

struct LessThanGraphNodeByPriority
{
	bool operator()(GraphNode* lhs, GraphNode* rhs)
	{
		return lhs->priority > rhs->priority;
	}
};

struct Graph
{
	std::vector<Connection> connections;
	std::vector<GraphNode> nodes;
	std::vector<int> connectionLUT[K_MAX_NODES]; // Lookup table for connections indexed by node
	std::vector<int> GetConnections(int nodeIndex);

	int AddNode(Vector2D position);
	void RemoveLastNode();
	int AddConnection(int fromIndex, int toIndex, float weight = -1.0f);
	void RemoveLastConnection(int fromIndex);
	void AddBidirectionalConnection(int indexA, int indexB, float weight = -1.0f);

	void ResetNodes();

	void Render(SDL_Renderer* renderer, Vector2D graphOrigin, Uint32 graphColor);
	void DrawPath(SDL_Renderer* renderer, Vector2D graphOrigin, std::vector<int> path, Uint32 graphColor);
};
