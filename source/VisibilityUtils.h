/* ========================================================================
   File: VisibilityUtils.h
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#pragma once

#include "Common/GameFramework.h"
#include <vector>

struct VisibilityPolygon
{
	std::vector<Vector2D> perimeter;
	std::vector<std::vector<Vector2D>> holes;

	bool AreInLineOfSight(Vector2D start, Vector2D end);
};

namespace VisibilityUtils
{
	static bool IsVertexConcave(std::vector<Vector2D> vertices, int vertex, bool isHole = false)
	{
		Vector2D current = vertices[vertex];
		Vector2D next = vertices[(vertex + 1) % vertices.size()];
		Vector2D previous = vertices[vertex == 0 ? vertices.size() - 1 : vertex - 1];

		Vector2D left = Vector2D(current.x - previous.x, current.y - previous.y);
		Vector2D right = Vector2D(next.x - current.x, next.y - current.y);

		float cross = (left.x * right.y) - (left.y * right.x);

		if (isHole == false)
		{
			return cross > 0;
		}
		else
		{
			return cross < 0;
		}
	}

	static bool PointInPolygon(std::vector<Vector2D> points, Vector2D point) 
	{
		// Code to solve the Point In Polygon Problem
		// Source and explanation: https://www.ecse.rpi.edu/Homepages/wrf/Research/Short_Notes/pnpoly.html
		// Literature: https://en.wikipedia.org/wiki/Point_in_polygon
		int i, j, nvert = points.size();
		bool c = false;

		for (i = 0, j = nvert - 1; i < nvert; j = i++) 
		{
			if (((points[i].y >= point.y) != (points[j].y >= point.y)) &&
				(point.x <= (points[j].x - points[i].x) * (point.y - points[i].y) 
					/ (points[j].y - points[i].y) + points[i].x)
				)
				c = !c;
		}

		return c;
	}

	static Vector2D ConvertPointForRadius(Vector2D originalPoint,
		Vector2D previousPoint, Vector2D nextPoint, float radius, bool isHole = false)
	{
		Vector2D convertedPoint;
		Vector2D nextSegment = nextPoint - originalPoint;
		Vector2D previousSegment = originalPoint - previousPoint;

		// Calculate previous and next segment angles
		float nextAngle = atan2f(nextSegment.y, nextSegment.x);
		float prevAngle = atan2f(previousSegment.y, previousSegment.x);

		// Calculate minimum distance between segment angles.
		float distance = nextAngle - prevAngle;
		if (abs(distance) > M_PI)
		{
			distance -= distance > 0 ? 2 * M_PI: -2 * M_PI;
		}

		// Calculate left perpendicular to average angle.
		float angle = prevAngle + (distance / 2) + M_PI/2.0f;
		SDL_Log("Angle = %f", angle * RAD2DEG);
		Vector2D normal = Vector2D(cosf(angle), sinf(angle));

		if (isHole == false)
		{
			convertedPoint = originalPoint - normal * radius;
		}
		else
		{
			convertedPoint = originalPoint + normal * radius;
		}
		return convertedPoint;
	}
}
