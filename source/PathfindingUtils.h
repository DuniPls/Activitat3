/* ========================================================================
   File: PathfindingUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include "GridUtils.h"
#include <queue>
#include <functional>
#include <utility>

#define D_PARAM 1.0f
#define D2_PARAM 2.0f
#define P_PARAM 1.0f / 60.0f

struct CoolPriorityQueue 
{
	typedef std::pair<float, Node*> PQElement;
	std::priority_queue < PQElement, std::vector<PQElement>,
		std::greater < PQElement >> elements;

	inline bool empty()
	{
		return elements.empty(); 
	}

	inline void put(Node* item, float priority) 
	{
		elements.emplace(priority, item);
	}

	inline Node* get() 
	{
		Node* best_item = elements.top().second;
		elements.pop();
		return best_item;
	}
};

typedef float(*HeuristicFunction)(Vector2D a, Vector2D b);
namespace HeuristicUtils
{
	static float ManhattanDistance(Vector2D a, Vector2D b)
	{
		float dx = abs(a.x - b.x);
		float dy = abs(a.y - b.y);
		return D_PARAM * (dx + dy);
	}

	static float OctileDistance(Vector2D a, Vector2D b)
	{
		static float K_OCTILE_D = 1.0f;
		static float K_OCTILE_D2 = sqrt(2.0f);

		float dx = abs(a.x - b.x);
		float dy = abs(a.y - b.y);
		return K_OCTILE_D * (dx + dy) + (K_OCTILE_D2 - 2 * K_OCTILE_D) * FloatUtils::Min(dx, dy);
	}

	static float ChebyshevDistance(Vector2D a, Vector2D b)
	{
		static float K_CHEVISEV_D = 1.0f;
		static float K_CHEVISEV_D2 = 1.0f;

		float dx = abs(a.x - b.x);
		float dy = abs(a.y - b.y);
		return K_CHEVISEV_D * (dx + dy) + (K_CHEVISEV_D2 - 2 * K_CHEVISEV_D) * FloatUtils::Min(dx, dy);
	}

	static float DiagonalDistance(Vector2D a, Vector2D b)
	{
		float dx = abs(a.x - b.x);
		float dy = abs(a.y - b.y);
		return D_PARAM * (dx + dy) + (D2_PARAM - 2 * D_PARAM) * FloatUtils::Min(dx, dy);
	}

	static float EuclideanDistance(Vector2D a, Vector2D b)
	{
		float dx = abs(a.x - b.x);
		float dy = abs(a.y - b.y);
		return D_PARAM * sqrt(dx*dx + dy*dy);
	}
}

namespace PathfindingUtils
{
	static float StraightLineTieBreak(Vector2D a, Vector2D start, Vector2D end)
	{
		float dx1 = a.x - end.x;
		float dy1 = a.y - end.y;
		float dx2 = start.x - end.x;
		float dy2 = start.y - end.y;
		// Cross product will be smaller if vectors are nearly parallel.
		float cross = abs(dx1*dy2 - dx2*dy1);
		return cross * 0.001f;
	}

	static std::vector<Node*> PathfindAStarMultipleGoals(Grid* grid, Node* start, int endNumber, Node** endArray, HeuristicFunction heuristicFunction = &HeuristicUtils::ManhattanDistance, bool useBreakTie = false, bool allowDiagonals = false, float gFactor = 1.0f, float hFactor = 1.0f)
	{
		// Declare result path
		std::vector<Node*> path;

		CoolPriorityQueue frontier;
		// Initialize Start node cost
		start->costSoFar = 0.0f;
		start->priority = 0.0f;
		frontier.put(start, start->priority);

		Node* current = nullptr;

		while (frontier.empty() == false)
		{
			// Get current node to process
			// Node is taken out of queue automatically
			current = frontier.get();

			// Check if we found any of the end goals yet
			bool endNodeFound = false;
			for (int i = 0; i < endNumber; ++i)
			{
				Node* currentEndNode = endArray[i];
				if (currentEndNode == current)
				{
					endNodeFound = true;
					break;
				}
			}

			// Break if any end goal found
			if (endNodeFound)
			{
				break;
			}

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

					// Calculate minimum heuristic for any of the end goal nodes
					float minHeuristicValue = 99999999999.9f;
					for (int j = 0; j < endNumber; ++j)
					{
						Node* currentEndNode = endArray[j];
						float currentHeuristic = heuristicFunction(currentEndNode->position, neighbors[i]->position);
						currentHeuristic += StraightLineTieBreak(neighbors[i]->position,
							start->position,
							currentEndNode->position);
						if (currentHeuristic < minHeuristicValue)
						{
							minHeuristicValue = currentHeuristic;
						}
					}

					float priority = (gFactor * newCost) + (hFactor * minHeuristicValue);

					neighbors[i]->heuristic = minHeuristicValue;
					neighbors[i]->priority = priority;
					neighbors[i]->cameFrom = current;
					//Add neighbor to queue
					frontier.put(neighbors[i], priority);
				}
			}
		}

		path.push_back(current);

		while (current != start)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return path;
	}

	static std::vector<Node*> PathfindAStar(Grid* grid, Node* start, Node* end, HeuristicFunction heuristicFunction = &HeuristicUtils::ManhattanDistance, bool useBreakTie = false, bool allowDiagonals = false, float gFactor = 1.0f, float hFactor = 1.0f)
	{
		// Declare result path
		std::vector<Node*> path;

		CoolPriorityQueue frontier;
		// Initialize Start node cost
		start->costSoFar = 0.0f;
		start->priority = 0.0f;
		frontier.put(start, start->priority);

		Node* current = nullptr;

		while (frontier.empty() == false)
		{
			// Get current node to process
			// Node is taken out of queue automatically
			current = frontier.get();

			// Early exit if we found goal
			if (current == end)
			{
				break;
			}

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
					float heuristicValue = heuristicFunction(end->position, neighbors[i]->position);
					if (useBreakTie == true)
					{
						//heuristicValue *= (1.0f + P_PARAM);
						heuristicValue += StraightLineTieBreak(neighbors[i]->position, 
																start->position, 
																end->position);
					}
					float priority = (gFactor * newCost) + (hFactor * heuristicValue);

					neighbors[i]->heuristic = heuristicValue;
					neighbors[i]->priority = priority;
					neighbors[i]->cameFrom = current;
					//Add neighbor to queue
					frontier.put(neighbors[i], priority);
				}
			}
		}


		if (current != end)
		{
			SDL_Log("No path found!\n");
			return path;
		}

		path.push_back(current);

		while (current != start)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return path;
	}

	static std::vector<Node*> PathfindGreedyBestFirst(Grid* grid, Node* start, Node* end, HeuristicFunction heuristicFunction = &HeuristicUtils::ManhattanDistance, bool allowDiagonals = false)
	{
		// Declare result path
		std::vector<Node*> path;

		std::priority_queue<Node*, std::vector<Node*>, LessThanByPriority> frontier;
		frontier = {};
		frontier.push(start);

		Node* current = nullptr;

		while (frontier.empty() == false)
		{
			// Get current node to process
			current = frontier.top();

			// Take node out of queue
			frontier.pop();

			// Early exit if we found goal
			if (current == end)
			{
				break;
			}

			// Get node neighbors
			std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
			// For each neighbor
			for (int i = 0; i < (int)neighbors.size(); ++i)
			{
				// Calculate new cost to get to this neighbor
				float heuristicValue = heuristicFunction(end->position, neighbors[i]->position);
				// If neighbor has not been visited yet, put it in the queue and store
				// where we came from
				if (neighbors[i]->cameFrom == nullptr)
				{
					neighbors[i]->priority = heuristicValue;
					neighbors[i]->heuristic = heuristicValue;
					neighbors[i]->cameFrom = current;
					//Add neighbor to queue
					frontier.push(neighbors[i]);
				}
			}
		}


		if (current != end)
		{
			SDL_Log("No path found!\n");
			return path;
		}

		path.push_back(current);

		while (current != start)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return path;
	}

	static std::vector<Node*> PathfindDijkstra(Grid* grid, Node* start, Node* end, bool allowDiagonals = false)
	{
		// Declare result path
		std::vector<Node*> path;

		std::priority_queue<Node*, std::vector<Node*>, LessThanByPriority> frontier;
		frontier = {};
		frontier.push(start);

		// Initialize Start node cost
		start->costSoFar = 0.0f;

		Node* current = nullptr;

		while (frontier.empty() == false)
		{
			// Get current node to process
			current = frontier.top();

			// Take node out of queue
			frontier.pop();

			// Early exit if we found goal
			if (current == end)
			{
				break;
			}

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
					neighbors[i]->priority = newCost;
					neighbors[i]->cameFrom = current;
					//Add neighbor to queue
					frontier.push(neighbors[i]);
				}
			}
		}

		if (current != end)
		{
			SDL_Log("No path found!\n");
			return path;
		}

		path.push_back(current);

		while (current != start)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return path;
	}

	static std::vector<Node*> PathfindBreadthExit(Grid* grid, Node* start, Node* end, bool allowDiagonals = false)
	{
		// Declare result path
		std::vector<Node*> path;

		std::queue<Node*> frontier;
		frontier.push(start);

		Node* current = nullptr;

		while (frontier.empty() == false)
		{
			// Get current node to process
			current = frontier.front();
			// Take node out of queue
			frontier.pop();

			// Early exit if we found goal
			if (current == end)
			{
				break;
			}
			// Get node neighbors
			std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
			// For each neighbor
			for (int i = 0; i < (int)neighbors.size(); ++i)
			{
				// If neighbor has not been visited yet, put it in the queue and store
				// where we came from
				if (neighbors[i]->cameFrom == nullptr)
				{
					frontier.push(neighbors[i]);
					neighbors[i]->cameFrom = current;
				}
			}
		}

		if (current != end)
		{
			SDL_Log("No path found!\n");
			return path;
		}

		path.push_back(current);

		while (current != start)
		{
			current = current->cameFrom;
			path.push_back(current);
		}
		std::reverse(path.begin(), path.end());

		return path;
	}

	static void PathfindBreadthPath(Grid* grid, Node* start, bool allowDiagonals = false)
	{
		std::queue<Node*> frontier;
		frontier.push(start);

		while (frontier.empty() == false)
		{
			// Get current node to process
			Node* current = frontier.front();
			// Take node out of queue
			frontier.pop();
			// Get node neighbors
			std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
			// For each neighbor
			for (int i = 0; i < (int)neighbors.size(); ++i)
			{
				// If neighbor has not been visited yet, put it in the queue and store
				// where we came from
				if (neighbors[i]->cameFrom == nullptr)
				{
					frontier.push(neighbors[i]);
					neighbors[i]->cameFrom = current;
				}
			}
		}
	}

	static void PathfindBreadthFlood(Grid* grid, bool visited[GRID_WIDTH][GRID_HEIGHT], Node* start, bool allowDiagonals = false)
	{
		std::queue<Node*> frontier;
		frontier.push(start);

		while (frontier.empty() == false)
		{
			// Get current node to process
			Node* current = frontier.front();
			// Take node out of queue
			frontier.pop();
			// Get node neighbors
			std::vector<Node*> neighbors = Grid_GetNeighbors(grid, current, allowDiagonals);
			// For each neighbor
			for (int i = 0; i < (int)neighbors.size(); ++i)
			{
				// If neighbor has not been visited yet, add it to queue and flag it as visited.
				if (visited[(int)neighbors[i]->position.x][(int)neighbors[i]->position.y] == false)
				{
					frontier.push(neighbors[i]);
					visited[(int)neighbors[i]->position.x][(int)neighbors[i]->position.y] = true;
				}
			}
		}
	}

	static void RenderPathfindingPath(Grid* grid, SDL_Renderer* renderer, std::vector<Node*>& path, Uint32 pathColor,
		Vector2D gridOrigin)
	{
		for (int i = 0; i < (int)path.size() - 1; ++i)
		{
			thickLineColor(renderer, gridOrigin.x + path[i]->position.x * grid->cellSize + grid->cellSize / 2,
				gridOrigin.y + path[i]->position.y * grid->cellSize + grid->cellSize / 2,
				gridOrigin.x + path[i + 1]->position.x * grid->cellSize + grid->cellSize / 2,
				gridOrigin.y + path[i + 1]->position.y * grid->cellSize + grid->cellSize / 2, 4, pathColor);
		}
	}
}
