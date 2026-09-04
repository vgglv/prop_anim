#include <prop_anim/key.hpp>

namespace {
	uint32_t last_index = 0;
}

namespace prop_anim {
	BaseKey::BaseKey() : idx(++last_index) {}

	KeyUniqPtr BaseKey::create(float t) {
		auto key = std::make_unique<BaseKey>();
		key->time = t;
		return key;
	}

	std::unique_ptr<BezierKey> BezierKey::create(float t) {
		auto key = std::make_unique<BezierKey>();
		key->time = t;
		return key;
	}
}
