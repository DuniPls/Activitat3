/* ========================================================================
   File: AdvancedPathfindingUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"

#include "GraphUtils.h"

#include <vector>
#include <queue>

struct CoolGraphPriorityQueue
{
	typedef std::pair<float, GraphNode*> PQElement;
	std::priority_queue < PQElement, std::vector<PQElement>,
		std::greater < PQElement >> elements;

	inline bool empty()
	{
		return elements.empty();
	}

	inline void put(GraphNode* item, float priority)
	{
		elements.emplace(priority, item);
	}

	inline GraphNode* get()
	{
		GraphNode* best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

namespace AdvancedPathfindingUtils
{
	static std::vector<int> PathfindAStar(Graph* graph, GraphNode* start, GraphNode* end, HeuristicFunction heuristicFunction = &HeuristicUtils::EuclideanDistance)
	{
		// Declare result path
		std::vector<int> path;

		// Initialize Start node cost
		start->costSoFar = 0.0f;
		start->priority = 0.0f;

		// Initialize the open and closed lists
		CoolGraphPriorityQueue frontier;
		frontier.put(start, start->priority);

		GraphNode* current = nullptr;

		// Iterate processing each node
		while (frontier.empty() == false)
		{
			// Get current Node to process
			current = frontier.get();

			// If it is the goal node, then terminate
			if (current == end)
			{
				break;
			}

			// Otherwise, get its outgoing connections
			std::vector<int> neighborConnectionsIndex = graph->GetConnections(current->index);
			// Loop through each connection in turn
			for (int i = 0; i < (int)neighborConnectionsIndex.size(); ++i)
			{
				Connection* currentConnection = &graph->connections[neighborConnectionsIndex[i]];
				GraphNode* neighborNode = &graph->nodes[currentConnection->GetToNodeIndex()];

				// Calculate new cost to get to this neighbor
				float newCost = current->costSoFar + currentConnection->GetCost();

				float currentCostSoFar = neighborNode->costSoFar;
				// If no stored cost or newCost is less than current
				if (currentCostSoFar == -1.0f || newCost < currentCostSoFar)
				{
					// Update Cost because we found a better path
					neighborNode->costSoFar = newCost;

					// Calculate heuristic and update priority
					float heuristicValue = heuristicFunction(end->position, neighborNode->position);
					float priority = newCost + heuristicValue;

					neighborNode->heuristic = heuristicValue;
					neighborNode->priority = priority;
					// Update connection
					neighborNode->cameFromIndex = currentConnection->index;

					//Add neighbor to the priority queue
					frontier.put(neighborNode, neighborNode->priority);
				}
			}
		}

		if (current != end)
		{
			SDL_Log("No path found!");
			return path;
		}

		path.push_back(current->index);

		while (current != start)
		{
			current = &graph->nodes[graph->connections[current->cameFromIndex].GetFromNodeIndex()];
			path.push_back(current->index);
		}
		std::reverse(path.begin(), path.end());

		return path;
	}
}
