#pragma once

namespace prop_anim {
	template<typename T>
	T quad_bezier(float t, T p0, T p1, T p2) {
		//formula: Q0(t) = (1-t)^2 * p0 + 2 * (1 - t) * t * p1 + t^2 * p2

		float ot = (1 - t);
		return (p0 * (ot * ot))
			+ (p1 * (2 * ot * t))
			+ (p2 * (t * t));
	}

	template<typename T>
	T cubic_bezier(float t, T p0, T p1, T p2, T p3) {
		//formula: C0(t) = (1-t)^3*P0 + 3 * (1-t)^2 * t * P1 + 3 * (1 - t) * t^2 * P2 + t^3 * P3

		//(1-t):
		float ot = (1.f - t);

		return (p0 * (ot * ot * ot))
			+ (p1 * (3.f * ot * ot * t))
			+ (p2 * (3.f * ot * t * t))
			+ (p3 * (t * t * t));
	}
}