#pragma once
#include "key.hpp"
#include <vector>

namespace prop_anim {
	struct CurveSample {
		AnimKey* from = nullptr;
		AnimKey* to = nullptr;
		float t = 0.f;
		bool is_exact = true;
	};

	CurveSample get_curve(const std::vector<KeyUniqPtr>& keys, float time);
}