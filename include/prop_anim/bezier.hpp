#pragma once
#include "point.hpp"

namespace prop_anim {
	Point quad_bezier(float t, Point p0, Point p1, Point p2);
	Point cubic_bezier(float t, Point p0, Point p1, Point p2, Point p3);

	float evaluate_bezier(float t, Point p0, Point p1, Point p2, Point p3);
}