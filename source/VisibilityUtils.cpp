/* ========================================================================
   File: VisibilityUtils.cpp
   Revision: 0.1
   Creator: David Collado Ochoa
   Notice: (C) Copyright 2016 by David Collado Ochoa. All Rights Reserved.
   ======================================================================== */

#include "VisibilityUtils.h"

bool VisibilityPolygon::AreInLineOfSight(Vector2D start, Vector2D end)
{
	bool result = false;
	
	bool doesIntersect = false;

	// Intersection with perimeter
	int n = (int)perimeter.size();
	for (int i = 0; i < n; ++i)
	{
		if ((perimeter[i] == start && perimeter[(i + 1) % n] == end)
			|| (perimeter[i] == end && perimeter[(i + 1) % n] == start))
		{
			return true;
		}

		if (Vector2DUtils::SegmentSegmentIntersection(start,
			end,
			perimeter[i],
			perimeter[(i + 1) % n], false) == true)
		{
			doesIntersect = true;
			break;
		}
	}

	// Intersection with holes
	for (int i = 0; i < (int)holes.size(); ++i)
	{
		for (int j = 0; j < (int)holes[i].size(); ++j)
		{
			if ((holes[i][j] == start && holes[i][(j + 1) % n] == end)
				|| (holes[i][j] == end && holes[i][(j + 1) % n] == start))
			{
				return true;
			}

			n = (int)holes[i].size();
			if (Vector2DUtils::SegmentSegmentIntersection(start,
				end,
				holes[i][j],
				holes[i][(j + 1) % n], false) == true)
			{	
				doesIntersect = true;
				break;
			}
		}

		if (doesIntersect == true)
		{
			break;
		}
	}

	if (doesIntersect == false)
	{
		bool isOutsideExceptPerimeter = false;
		// Check point is inside perimeter
		isOutsideExceptPerimeter = VisibilityUtils::PointInPolygon(perimeter, (start + end) / 2.0f);
		// Check point is outside all holes
		for (int i = 0; i < (int)holes.size(); ++i)
		{
			isOutsideExceptPerimeter &= !VisibilityUtils::PointInPolygon(holes[i], (start + end) / 2.0f);
		}

		if (isOutsideExceptPerimeter == true)
		{
			result = true;
		}
	}

	return result;
}
