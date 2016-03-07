/* ========================================================================
   File: SteppedPathfindingUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "PathfindingUtils.h"
#include "GridUtils.h"
#include <queue>
#include <functional>
#include <utility>

namespace SteppedPathfindingUtils
{
	static Node* staticNodeStart;
	static Node* staticNodeEnd;

	static CoolPriorityQueue coolFrontier;
	static void InitSteppedPathAStart(Node* start, Node* end)
	{
		coolFrontier = {};
		staticNodeStart = start;
		staticNodeEnd = end;

		// Initialize Start node cost
		start->costSoFar = 0.0f;
		start->priority = 0.0f;

		coolFrontier.put(start, 0.0f);

	}
	static bool SteppedPathfindAStar(Grid* grid, std::vector<Node*>& path, HeuristicFunction heuristicFunction = &HeuristicUtils::ManhattanDistance, bool useBreakTie = false, bool allowDiagonals = false, float gFactor = 1.0f, float hFactor = 1.0f)
	{
		bool finished = false;
		Node* current = nullptr;

		if (coolFrontier.empty() == false)
		{
			// Get current node to process
			// Node is taken out of queue automatically
			current = coolFrontier.get();

			// Early exit if we found goal
			if (current != staticNodeEnd)
			{

				// Get node neighbors
				std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
				// For each neighbor
				for (int i = 0; i < (int)neighbors.size(); ++i)
				{
					// Calculate new cost to get to this neighbor
					float newCost = current->costSoFar + Grid_GetCost(grid, neighbors[i]);
					// Get current stored cost
					float currentCostSoFar = neighbors[i]->costSoFar;
					// If no stored cost or newCost is less than current
					if (currentCostSoFar == -1.0f || newCost < currentCostSoFar)
					{
						// Update Cost because we found a better path
						neighbors[i]->costSoFar = newCost;
						float heuristicValue = heuristicFunction(staticNodeEnd->position, neighbors[i]->position);
						if (useBreakTie == true)
						{
							//heuristicValue *= (1.0f + P_PARAM);
							heuristicValue += PathfindingUtils::StraightLineTieBreak(neighbors[i]->position,
								staticNodeStart->position,
								staticNodeEnd->position);
						}
						float priority = (gFactor * newCost) + (hFactor * heuristicValue);

						neighbors[i]->heuristic = heuristicValue;
						neighbors[i]->priority = priority;
						neighbors[i]->cameFrom	= current;

						//Add neighbor to queue
						coolFrontier.put(neighbors[i], priority);
					}
				}
				return finished;
			}
		}

		finished = true;
		if (current != staticNodeEnd)
		{
			SDL_Log("No path found!\n");
			return finished;
		}

		path.push_back(current);

		while (current != staticNodeStart)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return finished;
	}

	static std::priority_queue<Node*, std::vector<Node*>, LessThanByPriority> priorityFrontier;
	static void InitSteppedPathfindGreedy(Node* start, Node* end)
	{
		priorityFrontier = {};
		priorityFrontier.push(start);
		staticNodeStart = start;
		staticNodeEnd = end;
	}

	static bool SteppedPathfindGreedy(Grid* grid, std::vector<Node*>& path, HeuristicFunction heuristicFunction = &HeuristicUtils::ManhattanDistance, bool allowDiagonals = false)
	{
		bool finished = false;
		Node* current = nullptr;

		if (priorityFrontier.empty() == false)
		{
			// Get current node to process
			current = priorityFrontier.top();

			// Take node out of queue
			priorityFrontier.pop();

			// Early exit if we found goal
			if (current != staticNodeEnd)
			{
				// Get node neighbors
				std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
				// For each neighbor
				for (int i = 0; i < (int)neighbors.size(); ++i)
				{
					// Calculate new cost to get to this neighbor
					float heuristicValue = heuristicFunction(staticNodeEnd->position, neighbors[i]->position);
					// If neighbor has not been visited yet, put it in the queue and store
					// where we came from
					if (neighbors[i]->cameFrom == nullptr)
					{
						neighbors[i]->priority = heuristicValue;
						neighbors[i]->heuristic = heuristicValue;
						neighbors[i]->cameFrom = current;
						//Add neighbor to queue
						priorityFrontier.push(neighbors[i]);
					}
				}
				return finished;
			}
		}

		finished = true;
		if (current != staticNodeEnd)
		{
			SDL_Log("No path found!\n");
			return finished;
		}

		path.push_back(current);

		while (current != staticNodeStart)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return finished;
	}

	static void InitSteppedPathDijkstra(Node* start, Node* end)
	{
		priorityFrontier = {};
		priorityFrontier.push(start);
		staticNodeStart = start;
		staticNodeEnd = end;

		// Initialize Start node cost
		start->costSoFar = 0.0f;
	}

	static bool SteppedPathfindDijkstra(Grid* grid, std::vector<Node*>& path, bool allowDiagonals = false)
	{
		bool finished = false;
		Node* current = nullptr;

		if (priorityFrontier.empty() == false)
		{
			// Get current node to process
			current = priorityFrontier.top();

			// Take node out of queue
			priorityFrontier.pop();

			// Early exit if we found goal
			if (current != staticNodeEnd)
			{
				// Get node neighbors
				std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
				// For each neighbor
				for (int i = 0; i < (int)neighbors.size(); ++i)
				{
					// Calculate new cost to get to this neighbor
					float newCost = current->costSoFar + Grid_GetCost(grid, neighbors[i]);
					// Get current stored cost
					float currentCostSoFar = neighbors[i]->costSoFar;
					if (currentCostSoFar == -1.0f || newCost < currentCostSoFar)
					{
						// Update Cost because we found a better path
						neighbors[i]->costSoFar = newCost;
						neighbors[i]->priority = newCost;
						neighbors[i]->cameFrom = current;

						//Add neighbor to queue
						priorityFrontier.push(neighbors[i]);
					}
				}
				return finished;
			}
		}

		finished = true;
		if (current != staticNodeEnd)
		{
			SDL_Log("No path found!\n");
			return finished;
		}

		path.push_back(current);

		while (current != staticNodeStart)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return finished;
	}

	static std::queue<Node*> breadthFrontier;
	static void InitSteppedPathfindBreadth(Node* start, Node* end)
	{
		breadthFrontier = {};
		breadthFrontier.push(start);
		staticNodeStart = start;
		staticNodeEnd = end;
	}
	static bool SteppedPathfindBreadthExit(Grid* grid, std::vector<Node*>& path, bool allowDiagonals = false)
	{
		bool finished = false;
		Node* current = nullptr;

		if (breadthFrontier.empty() == false)
		{
			// Get current node to process
			current = breadthFrontier.front();
			// Take node out of queue
			breadthFrontier.pop();

			// Early exit if we found goal
			if (current != staticNodeEnd)
			{
				// Get node neighbors
				std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
				// For each neighbor
				for (int i = 0; i < (int)neighbors.size(); ++i)
				{
					// If neighbor has not been visited yet, put it in the queue and store
					// where we came from
					if (neighbors[i]->cameFrom == nullptr)
					{
						breadthFrontier.push(neighbors[i]);
						neighbors[i]->cameFrom = current;
					}
				}
				return finished;
			}
		}

		finished = true;

		if (current != staticNodeEnd)
		{
			SDL_Log("No path found!\n");
		}

		path.push_back(current);

		while (current != staticNodeStart)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return finished;
	}

	static bool SteppedPathfindBreadthPath(Grid* grid, bool allowDiagonals = false)
	{
		bool finished = false;
		if (breadthFrontier.empty() == false)
		{
			// Get current node to process
			Node* current = breadthFrontier.front();
			// Take node out of queue
			breadthFrontier.pop();
			// Get node neighbors
			std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
			// For each neighbor
			for (int i = 0; i < (int)neighbors.size(); ++i)
			{
				// If neighbor has not been visited yet, put it in the queue and store
				// where we came from
				if (neighbors[i]->cameFrom == nullptr)
				{
					breadthFrontier.push(neighbors[i]);
					neighbors[i]->cameFrom = current;
				}
			}
		}
		else
		{
			finished = true;
		}

		return finished;
	}

	static std::queue<Node*> floodFrontier;
	static void InitSteppedPathfindBreadthFlood(Node* start)
	{
		floodFrontier = {};
		floodFrontier.push(start);
	}
	static bool SteppedPathfindBreadthFlood(Grid* grid, bool visited[GRID_WIDTH][GRID_HEIGHT], bool allowDiagonals = false)
	{
		bool finished = false;
		if (floodFrontier.empty() == false)
		{
			// Get current node to process
			Node* current = floodFrontier.front();
			// Take node out of queue
			floodFrontier.pop();
			// Get node neighbors
			std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
			// For each neighbor
			for (int i = 0; i < (int)neighbors.size(); ++i)
			{
				// If neighbor has not been visited yet, add it to queue and flag it as visited.
				if (visited[(int)neighbors[i]->position.x][(int)neighbors[i]->position.y] == false)
				{
					floodFrontier.push(neighbors[i]);
					visited[(int)neighbors[i]->position.x][(int)neighbors[i]->position.y] = true;
				}
			}
		}
		else
		{
			finished = true;
		}

		return finished;
	}
}
