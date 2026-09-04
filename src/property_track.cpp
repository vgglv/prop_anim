#include <prop_anim/property_track.hpp>
#include <prop_anim/curve.hpp>

namespace prop_anim {
	PropertyTrack::PropertyTrack(TypeRegistry& registry) : _registry(registry) {}

	void PropertyTrack::update(float t) {
		if (!_binding) {
			return;
		}
		CurveSample sample = get_curve(_keys_vec, t);
		if (sample.is_exact) {
			_binding->set(sample.from->value);
		} else {
			auto value = _registry.interpolate(_binding->type_name, sample.from->value, sample.to->value, sample.t);
			_binding->set(value);
		}
	}

	void PropertyTrack::set_property_binding(std::unique_ptr<PropertyBinding> binding) {
		_binding = std::move(binding);
	}

	KeyUniqPtr PropertyTrack::create_key(float t) {
		auto key = std::make_unique<BaseKey>();
		key->time = t;
		return key;
	}
}