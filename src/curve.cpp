#include <prop_anim/curve.hpp>
#include <algorithm>

namespace prop_anim {
	CurveSample get_curve(const std::vector<KeyUniqPtr>& keys, float time) {
		if (keys.empty())
			return {};

		if (time <= keys.front()->time)
			return {keys.front().get(), keys.front().get(), 0.f, true};

		if (time >= keys.back()->time)
			return {keys.back().get(), keys.back().get(), 1.f, true};

		auto it = std::lower_bound(keys.begin(), keys.end(), time, [](const KeyUniqPtr& key, float t) {
			return key->time < t;
		});

		auto* cur = it->get();
		auto* prev = (it - 1)->get();

		float raw = (time - prev->time) / (cur->time - prev->time);

		return {prev, cur, raw, false};
	}
}