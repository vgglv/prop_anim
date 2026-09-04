#include <any>
#include <prop_anim/bezier_track.hpp>
#include <prop_anim/curve.hpp>
#include <prop_anim/bezier.hpp>

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

	KeyUniqPtr BezierTrack::create_key(float t) {
		auto key = std::make_unique<BezierKey>();
		key->time = t;
		return key;
	}
}