/* ========================================================================
   File: GraphUtils.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "GraphUtils.h"
#include "PathfindingUtils.h"

void GraphNode::Reset()
{
	costSoFar = -1.0f;
	priority = 0.0f;
	heuristic = 0.0f;
	cameFromIndex = -1;
}

float Connection::GetCost()
{
	return weight;
}

int Connection::GetFromNodeIndex()
{
	return fromNodeIndex;
}

int Connection::GetToNodeIndex()
{
	return toNodeIndex;
}

std::vector<int> Graph::GetConnections(int nodeIndex)
{
	return connectionLUT[nodeIndex];
}

int Graph::AddNode(Vector2D position)
{
	GraphNode currentNode = {};

	int currentIndex = nodes.size();
	currentNode.index = currentIndex;
	currentNode.position = position;
	nodes.push_back(currentNode);

	return currentIndex;
}

void Graph::RemoveLastNode()
{
	nodes.pop_back();
}

int Graph::AddConnection(int fromIndex, int toIndex, float weight)
{
	Connection currentConnection = {};

	if (weight == -1.0f)
	{
		weight = HeuristicUtils::EuclideanDistance(nodes[fromIndex].position, nodes[toIndex].position);
	}

	int currentIndex = connections.size();
	currentConnection.index = currentIndex;
	currentConnection.fromNodeIndex = fromIndex;
	currentConnection.toNodeIndex = toIndex;
	currentConnection.weight = weight;
	connections.push_back(currentConnection);
	connectionLUT[fromIndex].push_back(currentIndex);

	return currentIndex;
}

void Graph::RemoveLastConnection(int fromIndex)
{
	connections.pop_back();
	connectionLUT[fromIndex].pop_back();
}

void Graph::AddBidirectionalConnection(int indexA, int indexB, float weight)
{
	if (weight == -1.0f)
	{
		weight = HeuristicUtils::EuclideanDistance(nodes[indexA].position, nodes[indexB].position);
	}

	AddConnection(indexA, indexB, weight);
	AddConnection(indexB, indexA, weight);
}

void Graph::ResetNodes()
{
	for (int i = 0; i < (int)nodes.size(); ++i)
	{
		nodes[i].Reset();
	}
}

void Graph::Render(SDL_Renderer* renderer, Vector2D graphOrigin, Uint32 graphColor)
{
	for (int i = 0; i < (int)connections.size(); ++i)
	{
		GraphNode* nodeFrom = &nodes[connections[i].GetFromNodeIndex()];
		GraphNode* nodeTo = &nodes[connections[i].GetToNodeIndex()];

		thickLineColor(renderer, graphOrigin.x + nodeFrom->position.x, 
			graphOrigin.y + nodeFrom->position.y,
			graphOrigin.x + nodeTo->position.x, 
			graphOrigin.y + nodeTo->position.y, 
			2.0f, graphColor);

		filledCircleColor(renderer, graphOrigin.x + nodeFrom->position.x,
			graphOrigin.y + nodeFrom->position.y, 10.0f, Colors::CARROT);

		filledCircleColor(renderer, graphOrigin.x + nodeTo->position.x,
			graphOrigin.y + nodeTo->position.y, 10.0f, Colors::CARROT);
	}
}

void Graph::DrawPath(SDL_Renderer* renderer, Vector2D graphOrigin, std::vector<int> path, Uint32 graphColor)
{
	for (int i = 0; i < (int)path.size() - 1; ++i)
	{
		GraphNode* nodeFrom = &nodes[path[i]];
		GraphNode* nodeTo = &nodes[path[i+1]];

		thickLineColor(renderer, graphOrigin.x + nodeFrom->position.x,
			graphOrigin.y + nodeFrom->position.y,
			graphOrigin.x + nodeTo->position.x,
			graphOrigin.y + nodeTo->position.y,
			5.0f, graphColor);

		Uint32 colorToUse = Colors::WET_ASPHALT;
		if (i == 0)
		{
			colorToUse = Colors::POMEGRANATE;
			filledCircleColor(renderer, graphOrigin.x + nodeFrom->position.x,
				graphOrigin.y + nodeFrom->position.y, 10.0f, colorToUse);
			filledCircleColor(renderer, graphOrigin.x + nodeTo->position.x,
				graphOrigin.y + nodeTo->position.y, 10.0f, Colors::WET_ASPHALT);
		}
		else if (i == (int)path.size() - 2)
		{
			colorToUse = Colors::AMETHYST;
			filledCircleColor(renderer, graphOrigin.x + nodeFrom->position.x,
				graphOrigin.y + nodeFrom->position.y, 10.0f, Colors::WET_ASPHALT);
			filledCircleColor(renderer, graphOrigin.x + nodeTo->position.x,
				graphOrigin.y + nodeTo->position.y, 10.0f, colorToUse);
		}
		else
		{
			filledCircleColor(renderer, graphOrigin.x + nodeFrom->position.x,
				graphOrigin.y + nodeFrom->position.y, 10.0f, Colors::WET_ASPHALT);
			filledCircleColor(renderer, graphOrigin.x + nodeTo->position.x,
				graphOrigin.y + nodeTo->position.y, 10.0f, Colors::WET_ASPHALT);
		}
	}
}


