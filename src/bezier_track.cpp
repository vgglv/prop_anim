#include <prop_anim/bezier_track.hpp>
#include <prop_anim/curve.hpp>
#include <any>

namespace prop_anim {
	BezierTrack::BezierTrack() = default;

	std::unique_ptr<BezierTrack> BezierTrack::create() {
		return std::make_unique<BezierTrack>();
	}

	void BezierTrack::update(float t) {
		if (!_binding) {
			return;
		}
		CurveSample sample = get_curve(_keys_vec, t);
		if (sample.is_exact) {
			_binding->set(sample.from->value);
		} else {
			auto key_left = static_cast<BezierKey*>(sample.from);
			auto key_right = static_cast<BezierKey*>(sample.to);
			Point p0 = { key_left->time, std::any_cast<float>(sample.from->value) };
			Point p1 = { p0.x + key_left->out_handle.x, p0.y + key_left->out_handle.y };
			Point p3 = { key_right->time, std::any_cast<float>(sample.to->value) };
			Point p2 = { p3.x + key_right->in_handle.x, p3.y + key_right->in_handle.y };

			float value = evaluate_bezier(t, p0, p1, p2, p3);
			_binding->set(value);
		}
	}

	void BezierTrack::set_property_binding(std::unique_ptr<PropertyBinding> binding) {
		_binding = std::move(binding);
	}

	Point BezierTrack::quad_bezier(float t, Point p0, Point p1, Point p2) {
		//formula: Q0(t) = (1-t)^2 * p0 + 2 * (1 - t) * t * p1 + t^2 * p2

		float dt = (1 - t);
		float x = (p0.x * (dt * dt)) + (p1.x * (2 * dt * t)) + (p2.x * (t * t));
		float y = (p0.y * (dt * dt)) + (p1.y * (2 * dt * t)) + (p2.y * (t * t));
		return Point{x, y};
	}

	Point BezierTrack::cubic_bezier(float t, Point p0, Point p1, Point p2, Point p3) {
		//formula: C0(t) = (1-t)^3*P0 + 3 * (1-t)^2 * t * P1 + 3 * (1 - t) * t^2 * P2 + t^3 * P3

		//(1-t):
		float ot = (1.f - t);
		float x = (p0.x * (ot * ot * ot)) + (p1.x * (3.f * ot * ot * t)) + (p2.x * (3.f * ot * t * t)) + (p3.x * (t * t * t));
		float y = (p0.y * (ot * ot * ot)) + (p1.y * (3.f * ot * ot * t)) + (p2.y * (3.f * ot * t * t)) + (p3.y * (t * t * t));

		return Point{x, y};
	}

	float BezierTrack::evaluate_bezier(float t, Point p0, Point p1, Point p2, Point p3) {
		float lo = 0.0f;
		float hi = 1.0f;

		// Find u such that B.x(u) ~= time.
		for (int i = 0; i < 20; ++i) {
			const float u = (lo + hi) * 0.5f;
			const float x = cubic_bezier(u, p0, p1, p2, p3).x;

			if (x < t)
				lo = u;
			else
				hi = u;
		}

		const float u = (lo + hi) * 0.5f;

		float v = cubic_bezier(u, p0, p1, p2, p3).y;
		//printf("evaluate_bezier=%.2f\n", v);
		return v;
	}
}